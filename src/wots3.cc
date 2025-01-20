// ----------------------------------------------------------------------------
//
//    Work In Progress converter for WotS 3 files.
//
//    Based on meshes using NiTriStrips nodes (ie. items, maps).
//    Characters are not supported.
//
//    Currently export triangular meshes with no material or additional
//    vertex attributes.
//
// ----------------------------------------------------------------------------

#include <cassert>
#include <cstdlib>
#include <cstddef>

#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif

// NIF importer.
#include "nif/niflib.h"
#include "nif/gen/Header.h"
#include "nif/obj/NiNode.h"
#include "nif/obj/NiMesh.h"

#include "nif/obj/NiAlphaProperty.h"
#include "nif/obj/NiPixelData.h"
#include "nif/obj/NiSpecularProperty.h"
#include "nif/obj/NiIntegerExtraData.h"
#include "nif/obj/NiStringExtraData.h"
#include "nif/obj/NiTexturingProperty.h"
#include "nif/obj/NiTriStrips.h"
#include "nif/obj/NiTriStripsData.h"
#include "nif/obj/NiVertexColorProperty.h"
#include "nif/obj/NiMaterialProperty.h"

// Convert float16 to float32.
#include "half/half.hpp"
using half_float::half;

// Decompress DXT1/DXT5.
#define S3TC_IMPLEMENTATION
#include "s3tc/s3tc.h"

// Save DDS.
#include "dds.h"

// STB image.
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// glTF exporter.
#define TINYGLTF_NOEXCEPTION
#define TINYGLTF_IMPLEMENTATION
#include "tinygltf/tiny_gltf.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

// ----------------------------------------------------------------------------

#define DOJIMA_LOG(x) std::cerr << x << std::endl

#ifndef NDEBUG
#define DOJIMA_QUIET_LOG(x)  DOJIMA_LOG(x)
#else
#define DOJIMA_QUIET_LOG(x)  if (static bool b=false; !b && (b=true)) DOJIMA_LOG("(quiet log) " << x)
#endif

// ----------------------------------------------------------------------------

namespace {

// Display NIF header information at loading.
static constexpr bool kDisplayHeader = true;

// Output external PNG for debugging.
static constexpr bool kDebugOutputPNG = true; //
static constexpr bool kDebugOutputDDS = false; //

// Output name for accessors.
static constexpr bool kNameAccessors = true;

// glTF output options.
static constexpr bool kGLTFEmbedImages = true;
static constexpr bool kGLTFEmbedBuffers = true;
static constexpr bool kGLTFPrettyPrint = true;
static constexpr bool kGLTFWriteBinary = false;

// OS specific path separator.
static constexpr char kSystemPathSeparator{
#if defined(WIN32) || defined(_WIN32)
  '\\'
#else
  '/'
#endif
};

enum class BufferId : uint8_t {
  INDEX,
  VERTEX,
  // IMAGE, //

  kCount
};

enum class AttributeId : uint8_t {
  Position,
  Texcoord,
  Normal,
  Tangent,
  Joints,
  Weights,
  Binormal, //

  kCount
};

// NIF node keys of interest.
static std::string const kNiDataStreamKey{ "NiDataStream" };
static std::string const kNiMeshKey{ "NiMesh" };
static std::string const kNiPixelDataKey{ "NiPixelData" };
// static std::string const kNiSourceTextureKey{ "NiSourceTexture" };

uint32_t GetPrimitiveType(Niflib::MeshPrimitiveType primType) {
  switch (primType) {
    case Niflib::MESH_PRIMITIVE_TRIANGLES:
    return TINYGLTF_MODE_TRIANGLES;

    case Niflib::MESH_PRIMITIVE_TRISTRIPS:
    return TINYGLTF_MODE_TRIANGLE_STRIP;

    case Niflib::MESH_PRIMITIVE_LINESTRIPS:
    return TINYGLTF_MODE_LINE_STRIP;

    case Niflib::MESH_PRIMITIVE_POINTS:
    return TINYGLTF_MODE_POINTS;

    default:
      DOJIMA_QUIET_LOG( "[Warning] " << __FUNCTION__ << " : primitive type " << primType << " is not supported." );
    return TINYGLTF_MODE_POINTS;
  };
}

bool IsHalfPrecision(Niflib::ComponentFormat format) {
  switch(format) {
    case Niflib::F_FLOAT16_2:
    case Niflib::F_FLOAT16_4:
    return true;

    default:
    return false;
  };
}

size_t SetAccessorFormat(Niflib::ComponentFormat format, tinygltf::Accessor *accessor) {

  // Notes :
  // glTF 2.0 does not support half precision floating point buffer natively so we
  // either have to define an extension / pack them as uint (lacking portability)
  // or we can just convert the buffer as 32bit floating point (at the cost of
  // doubling the buffer size).
  //
  // Here fp16 data are "wrongly" typed as fp32, while conversion checking
  // should be performed on the VertexBuffer's BufferView side.
  //
  //  We set an accessor type depending on the input format but which might
  //  not comply with glTF standard for a given attribute target, therefore it
  // might change afterwards when calling ::SetAttribute.
  //

  switch(format) {
    case Niflib::F_UINT16_1:
      accessor->componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
      accessor->type = TINYGLTF_TYPE_SCALAR;
    return 1 * sizeof(uint16_t);

    case Niflib::F_FLOAT16_2: //
    case Niflib::F_FLOAT32_2:
      accessor->componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessor->type = TINYGLTF_TYPE_VEC2;
    return 2 * sizeof(float);

    case Niflib::F_FLOAT16_4: //
    case Niflib::F_FLOAT32_4:
      accessor->componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessor->type = TINYGLTF_TYPE_VEC4;
    return 4 * sizeof(float);

    case Niflib::F_FLOAT32_3:
      accessor->componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessor->type = TINYGLTF_TYPE_VEC3;
    return 3 * sizeof(float);

    case Niflib::F_UINT8_4:
      accessor->componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      accessor->type = TINYGLTF_TYPE_VEC4;
    return 4 * sizeof(uint8_t);

    default:
      DOJIMA_QUIET_LOG( "[Warning] " << __FUNCTION__ << " : unsupported format " << format << "." );
    return 0;
  };
}

bool SetAttribute(Niflib::SemanticData cs, size_t accessorIndex, tinygltf::Primitive *prim, tinygltf::Accessor *accessor) {
  std::string const& name = cs.name;
  std::string const suffix{std::to_string(cs.index)};

  // DOJIMA_QUIET_LOG( "setting attribute " << name );

  if ((name == "POSITION") || (name == "POSITION_BP")) {
    prim->attributes["POSITION"] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC3;
    // [ should be defined for POSITION ]
    // @see https://www.khronos.org/registry/glTF/specs/2.0/glTF-2.0.html#accessors-bounds
    // auto &min_values = accessor->minValues;
    // min_values.resize(3);
    // auto &max_values = accessor->maxValues;
    // max_values.resize(3);
  }
  else if ((name == "NORMAL") || (name == "NORMAL_BP")) {
    prim->attributes["NORMAL"] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC3;
  }
  else if (name == "TEXCOORD") {
    prim->attributes["TEXCOORD_" + suffix] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC2;
  }
  else if (name == "BLENDINDICES") {
    prim->attributes["JOINTS_" + suffix] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC4;
  }
  else if (name == "BLENDWEIGHT") {
    prim->attributes["WEIGHTS_" + suffix] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC4;
  }
  // (invalid : glTF is expecting vec4 floating point for TANGENT where the buffer is vec3)
  else if ((name == "TANGENT") || (name == "TANGENT_BP")) {
    prim->attributes["TANGENT"] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC4;
    accessor->normalized = true; // (just here to prevent error display on validation)
  }
  // (not natively supported by glTF, here to prevent preprocessing the vertex buffer)
  else if ((name == "BINORMAL") || (name == "BINORMAL_BP")) {
    prim->attributes["_BINORMAL"] = accessorIndex;
  }
  else {
    DOJIMA_QUIET_LOG( "[Warning] " << __FUNCTION__ << " : " << name << " component not used." );
    return false;
  }

  return true;
}

} // namespace ""

// ----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
#if 0

  std::string const nifFilename{
    "../DATA/Way of the Samurai 3/Common/"

    // "Item/mdl/"
    // "ac_zingasa.nif" // 6 tri strips
    // "ac_makimono.nif" // 2 tri strips
    // "ac_zyuzu.nif" // 1 tri strips

    "Map/Scene/S3_syukuba_01/S3_syukuba_01_d.nif" // 25 tri strips
  };



  DOJIMA_QUIET_LOG( " >>> FORCED FILENAME :" << nifFilename );

#else

  if (argc < 2) {
    DOJIMA_LOG( "Usage : " << std::endl << argv[0] << " nif_file" );
    return EXIT_FAILURE;
  }
  std::string const nifFilename{ argv[1] };

#endif

  // --------------

  // Create an outputs directory for the transformed NIF file.
  std::string const outputDirectory{
    [](std::string s) -> std::string {
      return "_outputs" +
             std::string(&kSystemPathSeparator) +
             s.replace(s.begin() + s.find_last_of("."), s.end(), "")
              .substr(s.find_last_of(kSystemPathSeparator) + 1) +
             std::string(&kSystemPathSeparator);
    }(nifFilename)
  };
  std::filesystem::create_directories(outputDirectory);

  // --------------

  // Retrieve blocks / structure informations from the NIF header.
  Niflib::Header nifHeader = Niflib::ReadHeader(nifFilename);

  // Block type names.
  auto const &blockTypes = nifHeader.getBlockTypes();

  // Type index of each blocks.
  auto const &blockTypeIndex = nifHeader.getBlockTypeIndex();

  // Number of block of each type.
  std::vector<size_t> blockTypeCount(blockTypes.size(), 0);

  // List of node indices from their types.
  std::unordered_map< std::string, std::vector<size_t> > typeToListIndices;
  {
    int current_index = 0;
    for (auto const bti : blockTypeIndex) {
      ++blockTypeCount[bti];
      typeToListIndices[blockTypes[bti]].push_back(current_index++);
    }
  }

  // // Block type name to base index.
  // std::unordered_map<std::string, size_t> typeToIndex;
  // for (size_t i = 0; i < blockTypes.size(); ++i) {
  //   typeToIndex[blockTypes[i]] = i;
  // }


  // //  Return the number of block for a given block name.
  // auto getNumBlocks = [&](std::string const& key) -> size_t {
  //   auto it = typeToIndex.find(key);
  //   auto const count = (it == typeToIndex.end()) ? 0 : blockTypeCount.at( it->second );
  //   return count;
  // };

  // Return the list of node indices for a given block name.
  auto getTypeListIndices = [&](std::string const& key) -> std::vector<size_t> {
    auto it = typeToListIndices.find(key);
    return (it == typeToListIndices.end()) ? std::vector<size_t>() : it->second;
  };

  // Display header informations.
  if constexpr (kDisplayHeader) {
    std::cerr <<
      " * Header string : "  << nifHeader.getHeaderString() << std::endl <<
      " * Version : "        << std::hex << "0x" << nifHeader.getVersion() << std::dec << std::endl <<
      " * Endianness : "     << ((nifHeader.getEndianType() == Niflib::ENDIAN_BIG) ? "big" : "little") << std::endl <<
    "";

    std::cerr << " * Block types (" << blockTypes.size() << ") :" << std::endl;
    int bti = 0;
    for (auto const &bt : blockTypes) {
      std::cerr << "      + " << bti << " " << bt << " [" << blockTypeCount[bti] << "], \n";
      ++bti;
    }
  }

  if (nifHeader.getVersion() == 0x1e10003) {
    std::cerr << " [Error] This version only read Way Of The Samurai 3 Nif file." << std::endl;
    exit(-1);
  }

  // --------------------------------------------

  auto nifList = Niflib::ReadNifList(nifFilename);

  auto const& triStripsIndices = getTypeListIndices( "NiTriStrips" );
  auto const& triStripsDataIndices = getTypeListIndices( "NiTriStripsData" );

  // > TODO
  // The first version was made as if triStripsIndices size matched triStripsDataIndices
  // size, while the later instances can be shared by multiple instance of the former.
  //
  // Therefore the current bufferView (/ accessors ?) count is incorrect as we
  // need to share bufferViews per niTriStripsData instances.
  //
  // assert( triStripsIndices.size() == triStripsDataIndices.size() );

  uint32_t const kNumMeshes = triStripsIndices.size();
  uint32_t const kNumBuffers = static_cast<uint32_t>(BufferId::kCount);
  uint32_t const kNumPrimitives{ [&](){
    uint32_t total = 0u;
    for (uint mesh_id = 0; mesh_id < kNumMeshes; ++mesh_id) {
      auto const nifGeo = Niflib::StaticCast<Niflib::NiTriStrips>(
        nifList.at(triStripsIndices.at(mesh_id))
      );
      auto const meshStripData{
        Niflib::StaticCast<Niflib::NiTriStripsData>(nifGeo->GetData())
      };
      total += meshStripData->GetStripCount(); // XXX (should not be count twice with shared data)
    }
    return total;
  }() };

  DOJIMA_QUIET_LOG( "num ~meshes : " <<  kNumMeshes );
  DOJIMA_QUIET_LOG( "num ~primitives : " <<  kNumPrimitives );

  // Index to available buffer slot.
  size_t current_buffer_view = 0;
  size_t current_accessor = 0;
  // size_t current_material = 0; //

  // Offset to current buffers' end, used to build buffer viewers.
  size_t indexBufferOffset = 0;
  size_t vertexBufferOffset = 0;


  // --------------------------------------------


  tinygltf::Model data;
  {
    data.asset.version = "2.0";
    data.asset.generator = "nif2gltf";
  }

  data.meshes.resize(kNumMeshes);
  data.buffers.resize(kNumBuffers);
  data.bufferViews.resize((kNumBuffers - 1) * kNumMeshes + kNumPrimitives); //
  data.accessors.resize(kNumBuffers * kNumPrimitives); //

  tinygltf::Node *root_node_ptr = nullptr;

  auto &Nodes = data.nodes;

  auto &VertexBuffer = data.buffers[(int)BufferId::VERTEX];
  VertexBuffer.name = "Vertices";

  auto &IndexBuffer = data.buffers[(int)BufferId::INDEX];
  IndexBuffer.name = "Indices";

  // -- Meshes

  // Associate a mesh name to its internal id.
  std::unordered_map<std::string, uint32_t> mapMeshNameToId{};

  for (uint32_t mesh_id = 0u; mesh_id < kNumMeshes; ++mesh_id) {
    auto &mesh = data.meshes.at(mesh_id);

    auto const nifGeo = Niflib::StaticCast<Niflib::NiTriStrips>(
      nifList.at(triStripsIndices.at(mesh_id))
    );

    // Name.
    mesh.name = nifGeo->GetName();
    mapMeshNameToId[mesh.name] = mesh_id;

    auto const nifGeoData = nifGeo->GetData();
    auto vertices = nifGeoData->GetVertices();

    size_t const bytesize = vertices.size() * sizeof(vertices[0]);
    VertexBuffer.data.insert(VertexBuffer.data.end(), (unsigned char*)vertices.data(), (unsigned char*)vertices.data() + bytesize);

    // -- VERTEX POSITION BUFFER VIEW.
    int const position_BufferViewIndex = current_buffer_view++;
    {
      auto &bufferView = data.bufferViews.at(position_BufferViewIndex);

      bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
      bufferView.buffer = (int)BufferId::VERTEX;
      bufferView.byteLength = bytesize;
      bufferView.byteOffset = vertexBufferOffset;
      vertexBufferOffset += bufferView.byteLength;

      // Calculate the interleaved attributes bytestride.
      tinygltf::Accessor dummy_accessor{};
      size_t const byteSize = SetAccessorFormat(Niflib::F_FLOAT32_3, &dummy_accessor);
      bufferView.byteStride = byteSize; // [as fp32 values]
    }

    // TODO
    // NIFLIB_API vector<Vector3> GetNormals() const;
    // NIFLIB_API vector<TexCoord> GetUVSet( int index ) const;

    // Handle primitives.
    {
      // auto const meshStripData = Niflib::StaticCast<Niflib::NiTriStripsData>(
      //   nifList[triStripsDataIndices.at(mesh_id)]
      // );
      auto const meshStripData{
        Niflib::StaticCast<Niflib::NiTriStripsData>(nifGeo->GetData())
      };

      uint32_t const numPrimitives = meshStripData->GetStripCount();
      mesh.primitives.resize(numPrimitives); //

      for (uint32_t submesh_id = 0u; submesh_id < numPrimitives; ++submesh_id) {
        auto &prim = mesh.primitives.at(submesh_id);
        auto const& indices = meshStripData->GetStrip(submesh_id);

        prim.mode = TINYGLTF_MODE_TRIANGLE_STRIP;

        // Material (same as current niMesh).
        // prim.material = meshMaterialIndex;

        size_t const bytesize = indices.size() * sizeof(indices[0]);
        IndexBuffer.data.insert(IndexBuffer.data.end(), (unsigned char*)indices.data(), (unsigned char*)indices.data() + bytesize);

        // -- INDEX BUFFER VIEW.
        int const index_BufferViewIndex = current_buffer_view++;
        {
          auto &bufferView = data.bufferViews.at(index_BufferViewIndex);

          bufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
          bufferView.buffer = (int)BufferId::INDEX;
          bufferView.byteOffset = indexBufferOffset;
          bufferView.byteLength = bytesize;
          bufferView.byteStride = sizeof(indices[0]); //

          indexBufferOffset += bufferView.byteLength;
        }

        // Accessors
        {
          // -- Vertex attributes (only POSITION)
          // size_t vertexBaseOffset = region.startIndex * bufferView.byteStride;
          // for (int comp_id = 0; comp_id < md.numComponents; ++comp_id)
          {
            int const accessorIndex = current_accessor++;
            auto &acc = data.accessors[accessorIndex];

            // Set acc default params depending on format.
            auto const format = Niflib::F_FLOAT32_3; //formats[comp_id];
            size_t const byteSize = SetAccessorFormat(format, &acc);

            // Set the primitive attribute accessor.
            // auto const& sem = md.componentSemantics[comp_id];
            Niflib::SemanticData sem;
            sem.name = "POSITION";
            SetAttribute(sem, accessorIndex, &prim, &acc);

            if (kNameAccessors) {
              acc.name = sem.name;
            }
            acc.bufferView = position_BufferViewIndex;
            acc.byteOffset = 0; // vertexBaseOffset;

            auto const&bv = data.bufferViews[acc.bufferView];
            acc.count = bv.byteLength / bv.byteStride; //
            // vertexBaseOffset += byteSize; //
          }

          // -- Indices
          {
            // Set the primitive indices accessor.
            prim.indices = current_accessor++;
            auto &acc = data.accessors[prim.indices];

            size_t const byteSize = SetAccessorFormat(Niflib::F_UINT16_1, &acc); //

            if (kNameAccessors) {
              acc.name = "INDEX";
            }
            acc.bufferView = index_BufferViewIndex;
            acc.normalized = false;
            acc.byteOffset = 0; //region.startIndex * byteSize;
            acc.count = indices.size();//region.numIndices;
          }

        } // -- end accessors --
      } // -- end submeshes --
    }
  }  // -- end meshes --

  assert( data.accessors.size() == current_accessor );
  // data.accessors.resize(current_accessor);


  // --------------------------------------------


  // -- Node hierarchy.

  // Detects if the NIF file is a pack.
  uint32_t numSubParts = 0u;
  if (auto firstNode = Niflib::DynamicCast<Niflib::NiIntegerExtraData>(nifList[0]); firstNode) {
    numSubParts = firstNode->GetData() / 2u;
    DOJIMA_QUIET_LOG( "Detected Nif subparts : " << numSubParts );
  }

  // (only use a common root on NifPack)
  bool kUseRootNode = false && (numSubParts > 0u);

  // Determine the total number of hierarchical nodes.
  size_t totalNodeCount = kUseRootNode ? 1 : 0;
  for (auto const& n : nifList) {
    auto ptr = Niflib::DynamicCast<Niflib::NiAVObject>(n);
    totalNodeCount += (ptr != nullptr) ? 1 : 0;
  }

  Nodes.reserve(totalNodeCount);

  if (kUseRootNode) {
    Nodes.push_back({});
    root_node_ptr = &Nodes[0];
    root_node_ptr->name = "root";
  }


  // [lambda] recursive function to fill the node hierarchy.
  std::function<void(tinygltf::Node*const, Niflib::NiAVObject *const)> fillNodes{
    [&mapMeshNameToId, &Nodes, &fillNodes](tinygltf::Node *const parent, Niflib::NiAVObject *const nifAVO) -> void {
      // [ test, skip empty nodes ]
      // if (auto nifMesh = Niflib::DynamicCast<Niflib::NiMesh>(nifAVO); !nifMesh) {
      //   return;
      // }

      // Get a reference to a new node.
      Nodes.push_back( tinygltf::Node() );
      auto &node = Nodes.back();

      // Set it as its parent's children.
      if (parent) {
        int const nodeIndex = static_cast<int>(Nodes.size()) - 1;
        parent->children.push_back(nodeIndex);
      }

      node.name = nifAVO->GetName();

      auto const& qRotation = nifAVO->GetLocalRotation().AsQuaternion();
      node.rotation = { (double)qRotation.x, (double)qRotation.y, (double)qRotation.z, (double)qRotation.w };

      auto const fScale = static_cast<double>(nifAVO->GetLocalScale());
      node.scale = { fScale, fScale, fScale };

      auto const &vTranslation = nifAVO->GetLocalTranslation();
      node.translation = { (double)vTranslation.x, (double)vTranslation.y, (double)vTranslation.z };

      if (auto it = mapMeshNameToId.find(node.name); it != mapMeshNameToId.end()) {
        node.mesh = it->second; //
      }
      // if (auto nifMesh = Niflib::DynamicCast<Niflib::NiTriStrips>(nifAVO); nifMesh) {
      // }

      // [ TODO ] Detect meshLOD node (nifSwitchNode).
      // if (auto nifSwitchNode = Niflib::DynamicCast<Niflib::NiSwitchNode>(nifAVO); nifSwitchNode) {}

      // [ TODO ] Handle extra datas.
      // auto const& listXD = nifAVO->GetExtraData();
      // node.extras;

      // Continue processing if the nif object is a pure node (and not only a leaf).
      if (auto nifNode = Niflib::DynamicCast<Niflib::NiNode>(nifAVO); nifNode) {
        if (nifNode->IsSkeletonRoot()) {
          // node.skin;
        }
        for (auto &child : nifNode->GetChildren()) {
          fillNodes(&node, child);
        }
      }
    }
  };


  if (auto firstNode = Niflib::DynamicCast<Niflib::NiNode>(nifList[0]); firstNode) {
    // -- NIF Part.
    DOJIMA_QUIET_LOG(">> nif part");

    fillNodes(nullptr, firstNode);
  } else if (auto firstNode = Niflib::DynamicCast<Niflib::NiPixelData>(nifList[0]); firstNode) {
    // -- NIF PixelData.
    // WotS4 PixelData-only NIF files are encoded as BIG_ENDIAN and supposedly from a
    // version unsupported by Niflib so we can't read them as is.
    // Consequently we never reach this branch as Niflib::ReadHeader will crash before.
    DOJIMA_QUIET_LOG(">> NiPixelData TODO");
  }


  // // --------------

  // // std::cerr << "Nodes usage : " << Nodes.size() << " / " << Nodes.capacity()
  // //           << std::endl;

  // // Remove excess size (happens when meshes do not have joints attributes).
  // Accessors.resize(current_accessor);


  // tinygltf::Model data{};
  // {
  //   data.asset.generator = "nif2gltf";
  //   data.asset.version = "2.0";

  //   data.meshes = std::move( Meshes );
  //   data.materials = std::move( Materials );
  //   data.accessors = std::move( Accessors );
  //   data.bufferViews = std::move( BufferViews );
  //   data.buffers = std::move( Buffers );
  //   data.nodes = std::move( Nodes );

  //   if constexpr(false) {
  //     data.images = std::move( Images );
  //     data.textures = std::move( Textures );
  //     data.samplers = std::move( Samplers );
  //   }

  //   //data.skins;
  //   //data.animations;
  //   //data.cameras;
  //   //data.lights;

  //   tinygltf::Scene scene;
  //   scene.nodes.push_back(0);
  //   data.scenes.push_back(scene);
  // }


    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    data.scenes.push_back(scene);

  // -------------------------------------------

  std::string const gltfFilename{
    [](std::string s, std::string const& ext) -> std::string {
      return s.replace(
        s.begin() + s.find_last_of(".") + 1,
        s.end(),
        ext
      ).substr(s.find_last_of(kSystemPathSeparator) + 1);
    }(nifFilename, kGLTFWriteBinary ? "glb" : "gltf")
  };

  if (tinygltf::TinyGLTF gltf; !gltf.WriteGltfSceneToFile(
      &data, outputDirectory + gltfFilename, kGLTFEmbedImages, kGLTFEmbedBuffers, kGLTFPrettyPrint, kGLTFWriteBinary
    ))
  {
    DOJIMA_LOG( "[Error] glTF file writing failed." );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
