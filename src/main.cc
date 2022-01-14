#include <cassert>
#include <cstdlib>
#include <cstddef>

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "nif/niflib.h"

#include "nif/gen/Header.h"
#include "nif/obj/NiDataStream.h"
#include "nif/obj/NiMesh.h"
#include "nif/obj/NiSourceTexture.h"
#include "nif/obj/NiNode.h"

#include "nif/obj/NiIntegerExtraData.h"
#include "nif/obj/NiStringExtraData.h"
#include "nif/obj/NiPixelData.h"
#include "nif/obj/NiTexturingProperty.h"

#define TINYGLTF_NOEXCEPTION
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tinygltf/tiny_gltf.h"

#ifndef FP_FAST_FMA
#define FP_FAST_FMA 0
#endif
#define HALF_ERRHANDLING_ERRNO 0
#define HALF_ERRHANDLING_FENV 0
#define HALF_ERRHANDLING_FLAGS 0
#define HALF_ERRHANDLING_OVERFLOW_TO_INEXACT 0
#define HALF_ERRHANDLING_UNDERFLOW_TO_INEXACT 0
#define HALF_ERRHANDLING_THROWS 0
#define HALF_ENABLE_F16C_INTRINSICS 0
// #define HALF_ARITHMETIC_TYPE double
// #define HALF_ROUND_STYLE 1
#include "half/half.hpp"

using half_float::half;

// ----------------------------------------------------------------------------

#define DOJIMA_LOG(x) std::cerr << x << std::endl

#ifndef NDEBUG
#define DOJIMA_QUIET_LOG(x)  DOJIMA_LOG(x)
#else
#define DOJIMA_QUIET_LOG(x)  if (static bool b=false; !b && (b=true)) DOJIMA_LOG("(quiet log) " << x)
#endif

// ----------------------------------------------------------------------------

// Display NIF header information at loading.
static constexpr bool kDisplayHeader = false;

// Force the output filename to "out.gltf".
static constexpr bool kDebugOutput = true;

// Output name for accessors.
static constexpr bool kNameAccessors = false;

// ----------------------------------------------------------------------------

enum class BufferId : uint8_t {
  INDEX,
  VERTEX,

  kCount
};

enum class AttributeId : uint8_t {
  Position,
  Texcoord,
  Normal,
  Binormal,
  Tangent,
  Joints,
  Weights,
  
  kCount
};

// ----------------------------------------------------------------------------

// NIF node keys of interest.
std::string const kNiDataStreamKey{ "NiDataStream" };
std::string const kNiMeshKey{ "NiMesh" };
std::string const kNiSourceTextureKey{ "kNiSourceTextureKey" };

// OS specific path separator.
static constexpr char kSystemPathSeparator{
#if defined(WIN32) || defined(_WIN32)
  '\\'
#else
  '/'
#endif
};

// ----------------------------------------------------------------------------

static uint32_t GetPrimitiveType(Niflib::MeshPrimitiveType primType) {
  switch (primType) {
    case Niflib::MESH_PRIMITIVE_TRIANGLES:
    // DOJIMA_LOG( "Primitive : Triangles" );
    return TINYGLTF_MODE_TRIANGLES;

    case Niflib::MESH_PRIMITIVE_TRISTRIPS:
    // DOJIMA_LOG( "Primitive : Triangle Strips" );
    return TINYGLTF_MODE_TRIANGLE_STRIP;

    case Niflib::MESH_PRIMITIVE_LINESTRIPS:
    // DOJIMA_LOG( "Primitive : LineStrips" );
    return TINYGLTF_MODE_LINE_STRIP;
    
    case Niflib::MESH_PRIMITIVE_POINTS:
    // DOJIMA_LOG( "Primitive : Points" );
    return TINYGLTF_MODE_POINTS;

    default:
      DOJIMA_QUIET_LOG( "[Warning] " << __FUNCTION__ << " : primitive type " << primType << " is not supported." ); 
    return TINYGLTF_MODE_POINTS;
  };
}

static bool IsHalfPrecision(Niflib::ComponentFormat format) {
  switch(format) {
    case Niflib::F_FLOAT16_2:
    case Niflib::F_FLOAT16_4:
    return true;

    default:
    return false;
  };  
}

static size_t SetAccessorFormat(Niflib::ComponentFormat format, tinygltf::Accessor *accessor) {

  // Notes :
  // glTF 2.0 does not support half precision floating point buffer natively so we
  // either have to define an extension / pack them as uint (lacking portability)
  // or we can just convert the buffer as 32bit floating point (at the cost of
  // doubling the buffer size).
  //
  // Here fp16 data are "wrongly" typed as fp32, while conversion checking
  // should be perform on the VertexBuffer's BufferView side.
  //
  //  We set an accessor type depending on the input format but which can
  //  not comply with glTF standard for a given accessor type, therefore it
  // can be changed afterwards when calling SetAttribute.
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

static bool SetAttribute(Niflib::SemanticData cs, size_t accessorIndex, tinygltf::Primitive *prim, tinygltf::Accessor *accessor) {
  std::string const& name = cs.name;
  std::string const suffix{std::to_string(cs.index)};

  if ((name == "POSITION") || (name == "POSITION_BP")) {
    prim->attributes["POSITION"] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC3;
    accessor->normalized = false;
  } 
  else if ((name == "NORMAL") || (name == "NORMAL_BP")) {
    prim->attributes["NORMAL"] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC3;
    accessor->normalized = true;
  } 
  else if (name == "TEXCOORD") {
    prim->attributes["TEXCOORD_" + suffix] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC2;
    accessor->normalized = false;
  } 
  else if (name == "BLENDINDICES") {
    prim->attributes["JOINTS_" + suffix] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC4;
    accessor->normalized = false;
  } 
  else if (name == "BLENDWEIGHT") {
    prim->attributes["WEIGHTS_" + suffix] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC4;
    accessor->normalized = false;
  } 
  else if ((name == "TANGENT") || (name == "TANGENT_BP")) {
    prim->attributes["TANGENT"] = accessorIndex;
    accessor->type = TINYGLTF_TYPE_VEC4;
    accessor->normalized = true;
  }
  // (not natively supported by glTF)
  else if ((name == "BINORMAL") || (name == "BINORMAL_BP")) {
    prim->attributes["BINORMAL"] = accessorIndex;
    accessor->normalized = true;
  }
  else {
    DOJIMA_QUIET_LOG( "[Warning] " << __FUNCTION__ << " : " << name << " component not used." ); 
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
  if (argc < 2) {
    DOJIMA_LOG( "Usage : " << std::endl << argv[0] << " nif_file" );
    return EXIT_FAILURE;
  }
  std::string const nifFilename{ argv[1] };
  
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

  // Block type name to base index.
  std::unordered_map<std::string, size_t> typeToIndex;
  for (size_t i = 0; i < blockTypes.size(); ++i) {
    typeToIndex[blockTypes[i]] = i;
  }

  // Indices to differents datastreams types.
  std::vector<size_t> dataStreamIndices;
  for (auto const& [key, indices] : typeToListIndices) {
    if (key.find(kNiDataStreamKey) != std::string::npos) {
      dataStreamIndices.insert(dataStreamIndices.end(), indices.cbegin(), indices.cend());
    }
  }

  // --------------

  //  Return the number of block for a given block name.
  auto getNumBlocks = [&](std::string const& key) -> size_t {
    auto it = typeToIndex.find(key);
    auto const count = (it == typeToIndex.end()) ? 0 : blockTypeCount.at( it->second );
    return count;
  };

  // Return the list of node indices for a given block name.
  auto getTypeListIndices = [&](std::string const& key) -> std::vector<size_t> {
    auto it = typeToListIndices.find(key);
    return (it == typeToListIndices.end()) ? std::vector<size_t>() : it->second; 
  };

  // --------------

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

    // std::cerr << " * Blocks type index (" << blockTypeIndex.size() << ") :" << std::endl;
    // for (auto const &bti : blockTypeIndex) {
    //   std::cerr << bti << ", ";
    // }
    // std::cerr << std::endl;
  }

  // --------------

  // WotS4 has 3 data streams per Mesh :
  //    * USAGE_VERTEX_INDEX  : (INDEX)
  //    * USAGE_VERTEX        : (Texcoord, Position, Normal, Binormal, Tangent, BlendIndice, BlendWeight)
  //    * USAGE_USER          : (BONE_PALETTE)
  //
  //  Process :
  //
  // 1) We create 2 buffers for indices and vertices. 
  //       (we don't use BONE_PALETTE for now)
  //
  // 2) For each NiMesh we create :
  //    * 2 buffer_views for indices and vertices (one per used datastreams).
  //
  // 3) For each mesh's submeshes we create :
  //    * 1 accessors for indices.
  //    * 7 accessors for vertices.
  //
  // [ better approach ] use one buffer per LODs.
  //
  auto nifList = Niflib::ReadNifList(nifFilename);

  // Determine the bytesize of the global Index & Vertex buffers.
  size_t indicesBytesize = 0;
  size_t verticesBytesize = 0;
  for (auto index : dataStreamIndices) {
    auto nifDataStream = Niflib::StaticCast<Niflib::NiDataStream>(nifList[index]);
    auto const usage = nifDataStream->GetUsage();
    size_t const bytesize = nifDataStream->GetNumBytes();

    if (usage == Niflib::USAGE_VERTEX_INDEX) {
      indicesBytesize += bytesize;
    } else if (usage == Niflib::USAGE_VERTEX) {
      // Note : if the buffer contains *only* half floating point values its size
      //        should theorically be doubled for conversion.
      //        (as we used this info only as memory preallocation it's okay)
      verticesBytesize += bytesize; //
    }
  }

  // Create the base Index & Vertex buffers object.
  std::vector< tinygltf::Buffer > Buffers(static_cast<size_t>(BufferId::kCount));
  
  auto &IndexBuffer = Buffers[(int)BufferId::INDEX];
  IndexBuffer.name = "Indices";
  IndexBuffer.data.reserve(indicesBytesize);

  auto &VertexBuffer = Buffers[(int)BufferId::VERTEX];
  VertexBuffer.name = "Vertices";
  VertexBuffer.data.reserve(verticesBytesize); //
  
  // Offset to current buffers' end, used to build buffer viewers.
  size_t indexBufferOffset = 0;
  size_t vertexBufferOffset = 0;

  // Retrieve the number of NIF mesh nodes.
  size_t const kNiMeshCount{ getNumBlocks(kNiMeshKey) };
  
  // Calculate the total number of submeshes.
  size_t totalSubmeshesCount{ 0 };
  {
    auto const& meshIndices = getTypeListIndices( kNiMeshKey );
    for (auto mesh_id : meshIndices) {
      Niflib::NiObject *obj = nifList[mesh_id];
      Niflib::NiMesh* nifMesh = (Niflib::NiMesh*)obj;
      totalSubmeshesCount += nifMesh->GetNumSubmeshes();
    }
  }

  // Meshes buffers.
  std::vector< tinygltf::Mesh > Meshes( kNiMeshCount );
  std::vector< tinygltf::BufferView > BufferViews( Buffers.size() * kNiMeshCount );

  // Submeshes buffers.
  size_t const kNumAttributes = static_cast<size_t>(AttributeId::kCount); // (sometime less)
  size_t constexpr kNumAccessorPerSubmesh{ 1 + kNumAttributes }; 
  std::vector< tinygltf::Accessor > Accessors( totalSubmeshesCount * kNumAccessorPerSubmesh );

  // Materials buffer.
  // Depends on textures, MaterialData & NiMaterialProperty (size might be reduced)  
  std::vector< tinygltf::Material > Materials( kNiMeshCount ); //

  // Textures buffers.
  //    For packed NIF there is actually more NiPixelData than NiSourceTexture
  //    (one more per parts) but they seem redundant and we do not load them.
  size_t const kNumTextures{ getNumBlocks(kNiSourceTextureKey) };
  std::vector< tinygltf::Image > Images( kNumTextures ); // (NiPixelData)
  std::vector< tinygltf::Texture > Textures( kNumTextures ); // (NiSourceTexture)
  std::unordered_map< std::string, size_t > mapTextureNameToIndex;

  // TODO
  // Samplers buffer.
  std::vector< tinygltf::Sampler > Samplers( 1 );
  // std::map< uint16_t, size_t > mapTexDescFlagToSamplerIndex;
  // (also, check NiTextureEffect)
  
  // Index to available buffer slot.
  size_t current_buffer_view = 0;
  size_t current_accessor = 0;
  size_t current_material = 0; //
  size_t current_teximage = 0; //

  // --------------

  // -- Meshes

  // (notes on materials)
  // WotS NiMesh have the following properties :
  //    * NiMaterialProperty (with non PBR values),
  //    * NiAlphaProperty
  //    * NiSpecularProperty
  //    * NiVertexColorProperty
  //    * NiTexturingProperty, with 3 NiSourceTexture, each with a NiPixelData(to check with other files) :
  //          + a Base texture (FMT_DXT3)
  //          + a Detail Texture (FMT_DXT1)
  //          + a Normal Texture (FMT_DXT1)
  //
  //  After a "part" node is defined, a NiPixelData contains another texture, supposedely
  //  the diffuse texture for this part mesh.

  // Associate a mesh name to its internal id.
  std::unordered_map<std::string, size_t> mapMeshNameToId;

  // Setup Meshes.
  auto const& meshIndices = getTypeListIndices( kNiMeshKey );
  for (size_t mesh_id = 0; mesh_id < meshIndices.size(); ++mesh_id) {
    auto const nifMesh = Niflib::StaticCast<Niflib::NiMesh>(nifList[meshIndices[mesh_id]]);
    auto const primType = GetPrimitiveType(nifMesh->GetPrimitiveType());
    auto &mesh = Meshes.at(mesh_id);
    
    // Name.
    mesh.name = nifMesh->GetName();
    mapMeshNameToId[mesh.name] = mesh_id;


// ------------------------ WIP
    // Material.
    size_t const meshMaterialIndex = current_material++; //
    tinygltf::Material &material = Materials[meshMaterialIndex];

    // (we should use the SpecularGlossiness extensions)
    // https://kcoley.github.io/glTF/extensions/2.0/Khronos/KHR_materials_pbrSpecularGlossiness/

    // NiMesh properties.
    // Material, Alpha, Specular, VertexColor. // TODO
    
    // WIP
    // NiMesh properties : Image / Texture.
    if (auto prop = nifMesh->GetPropertyByType(Niflib::NiTexturingProperty::TYPE); prop) {
      auto texProp = Niflib::StaticCast<Niflib::NiTexturingProperty>(prop);
   
      if (texProp->HasBaseTexture()) {
        auto const& texDesc = texProp->GetBaseTexture();

        // retrieve sampler from texdesc flag and samplerMap.
        // todo

        // Get texture index / Check if texture already loaded.
        // todo

        auto const& srcTex = texDesc.source;
        auto const& texFilename = srcTex->GetTextureFileName();
        
        if (srcTex->IsTextureExternal()) {
          // DOJIMA_LOG( texFilename << " external" );
        } else {
          // DOJIMA_LOG( texFilename << " internal" );
        }

        // Handle texture transforms.
        // todo

        // Set the texture to the textureMap.
        // todo
      }

      // if (texProp->HasDetailTexture()) {
      //   auto const& texDesc = texProp->GetDetailTexture();
      // }
      // if (texProp->HasNormalTexture()) {
      //   auto const& texDesc = texProp->GetNormalTexture();
      // }
    } 
// ------------------------


    // Primitives / submeshes.
    uint32_t const nsubmeshes = nifMesh->GetNumSubmeshes();
    mesh.primitives.resize(nsubmeshes);

    // Create a buffer_view for each used datastream.
    uint32_t numMeshdata = 0;
    for (auto const& md : nifMesh->GetMeshDatas()) {
      auto const& stream = md.stream;
      auto const& usage = stream->GetUsage();

      // Bypass unused meshData.
      if ((usage == Niflib::USAGE_SHADER_CONSTANT) ||
          (usage == Niflib::USAGE_USER)) {
        // DOJIMA_QUIET_LOG( "[Warning] A " << usage << " MeshData was not used." );
        continue;
      }

      if (++numMeshdata > static_cast<uint32_t>(BufferId::kCount)) {
        DOJIMA_QUIET_LOG( "[Warning] An exceeding amount of internal buffers has been found." );
        break;
      }

      // -- BUFFER VIEW.

      int const bufferViewIndex = current_buffer_view++;
      auto &bufferView = BufferViews.at(bufferViewIndex);

      bufferView.byteLength = stream->GetNumBytes();
      auto const& data = stream->GetData();
      auto const& formats = stream->GetComponentFormats();

      switch (usage) {
        case Niflib::USAGE_VERTEX_INDEX:
          bufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
          bufferView.buffer = (int)BufferId::INDEX;
          bufferView.byteOffset = indexBufferOffset;
          bufferView.byteStride = 0;
          
          IndexBuffer.data.insert(IndexBuffer.data.end(), data.cbegin(), data.cend());
          indexBufferOffset += bufferView.byteLength;
        break;

        case Niflib::USAGE_VERTEX:
        {
          bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
          bufferView.buffer = (int)BufferId::VERTEX;
          bufferView.byteOffset = vertexBufferOffset;
          bufferView.byteStride = 0;
          
          // Calculate the interleaved attributes bytestride.
          tinygltf::Accessor dummy_accessor;
          int numHalfPrecisionBuffers = 0;
          for (auto const format : formats) {
            numHalfPrecisionBuffers += IsHalfPrecision(format) ? 1 : 0;
            size_t const byteSize = SetAccessorFormat(format, &dummy_accessor);
            bufferView.byteStride += byteSize; // [as fp32 values]
          }

          if (numHalfPrecisionBuffers == 0) {
            VertexBuffer.data.insert(VertexBuffer.data.end(), data.cbegin(), data.cend());
          } else if (numHalfPrecisionBuffers == md.numComponents) {
            // We handle half precision buffer conversion only when all the attributes
            // in the buffer are half precision.

            // Converts the half-precision floating point buffer to single precision.
            half const* fp16_buffer = reinterpret_cast<half const*>(data.data());
            std::vector<float> fp32_buffer(data.size() / 2);
            for (size_t i = 0; i < fp32_buffer.size(); ++i) {
              fp32_buffer[i] = static_cast<float>(fp16_buffer[i]);
            }
            auto bytes = reinterpret_cast<uint8_t const*>(fp32_buffer.data());
            VertexBuffer.data.insert(VertexBuffer.data.end(), bytes, bytes + sizeof(float) * fp32_buffer.size());
            
            bufferView.byteLength *= 2;
          } else {
            DOJIMA_QUIET_LOG( "[Warning] Sparsed half precision buffers are unsupported ." );
          }

          vertexBufferOffset += bufferView.byteLength;
        }
        break;

        default:
        break;
      };

      // -- Submeshes ACCESSORs.
      
      auto const& regions = stream->GetRegions();
      
      for (uint32_t submesh_id = 0; submesh_id < nsubmeshes; ++submesh_id) {
        auto &prim = mesh.primitives[submesh_id];
        prim.mode = primType;

        // Material (same as current niMesh).
        prim.material = meshMaterialIndex;

        // Creates accessors for each submeshes. 
        uint16_t const regionMapIndex = md.submeshToRegionMap[submesh_id];
        auto const& region = regions[regionMapIndex];
        
        if (usage == Niflib::USAGE_VERTEX_INDEX) {
          // -- Indices

          // Set the primitive indices accessor.
          prim.indices = current_accessor++;
          
          auto &acc = Accessors[prim.indices];
          size_t const byteSize = SetAccessorFormat(formats[0], &acc);

          if (kNameAccessors) acc.name = "INDEX";
          acc.bufferView = bufferViewIndex;
          acc.normalized = false;
          acc.byteOffset = region.startIndex * byteSize;
          acc.count = region.numIndices;
        } else {
          // -- Vertices
          
          size_t vertexBaseOffset = region.startIndex * bufferView.byteStride;
          for (int comp_id = 0; comp_id < md.numComponents; ++comp_id) {
            auto const format = formats[comp_id];
            auto const& sem = md.componentSemantics[comp_id];

            int const accessorIndex = current_accessor;

            //--------------------------
            // Set acc default params depending on format.
            auto &acc = Accessors[current_accessor++];
            size_t const byteSize = SetAccessorFormat(format, &acc);
            
            // Set the primitive attribute accessor.
            SetAttribute(sem, accessorIndex, &prim, &acc);
            //--------------------------

            if (kNameAccessors) acc.name = sem.name;
            acc.bufferView = bufferViewIndex;
            acc.byteOffset = vertexBaseOffset;
            acc.count = region.numIndices;

            vertexBaseOffset += byteSize; //
          }
        }
      } // -- end submeshes --
    } // -- end meshdatas --

    // TODO : NiSkinningMeshModifier.
    //mesh.weights;

    // TODO : extra datas (NiFloatExtraData, NiColorExtraData).
    //mesh.extra;
  } // -- end meshes --

  // ---------------------------------

  // Node hierarchy.

  // Determine the total number of hierarchical nodes.
  size_t totalNodeCount = 0;
  for (auto const& n : nifList) {
    auto ptr = Niflib::DynamicCast<Niflib::NiAVObject>(n);
    totalNodeCount += (ptr != nullptr) ? 1 : 0;
  }

  std::vector< tinygltf::Node > Nodes;
  Nodes.reserve(totalNodeCount);

  // Define a recursive lambda function to fill the node hierarchy.
  std::function<void(tinygltf::Node*const, Niflib::NiAVObject *const)> fillNodes;
  
  fillNodes = [&mapMeshNameToId, &Nodes, &fillNodes](tinygltf::Node *const parent, Niflib::NiAVObject *const nifAVO) -> void {
    // Get a reference to a new node.
    Nodes.push_back( tinygltf::Node() );
    auto &node = Nodes.back();
    
    // Set it as its parent's children.
    if (parent) {
      int const nodeIndex = static_cast<int>(Nodes.size() - 1);
      parent->children.push_back(nodeIndex);
    }

    node.name = nifAVO->GetName();

    auto const& qRotation = nifAVO->GetLocalRotation().AsQuaternion();
    node.rotation = { qRotation.x, qRotation.y, qRotation.z, qRotation.w };

    auto const fScale = nifAVO->GetLocalScale();
    node.scale = { fScale, fScale, fScale };

    auto const &vTranslation = nifAVO->GetLocalTranslation();
    node.translation = { vTranslation.x, vTranslation.y, vTranslation.z };

    if (auto nifMesh = Niflib::DynamicCast<Niflib::NiMesh>(nifAVO); nifMesh) {
      node.mesh = mapMeshNameToId[node.name];
    }

    // TODO
    // Detect meshLOD node (nifSwitchNode).
    // if (auto nifSwitchNode = Niflib::DynamicCast<Niflib::NiSwitchNode>(nifAVO); nifSwitchNode) {}

    // TODO
    // Handle extra datas.
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
  };


  // (Characters Pack)
  //
  //  The first node is a "NiIntegerExtraData" and contains the number of
  //   "NiStringExtraData" on the upper root.
  //
  // Then for each NiStringExtraData, if the following object has a consecutive index
  // it contains the path for that part's data and is alternatively a NiNode or a NiPixelData
  // (the NiPixelData representing only the diffuse texture).
  //
  // Supposedely, apart for the NiNode, the other datas are not that useful because 
  // they're already contained in it. So for now we retrieve their index but
  // don't do anything with it.
  //

  // Detects if the NIF file is a pack.
  uint32_t numSubParts = 0u;
  if (auto first = Niflib::DynamicCast<Niflib::NiIntegerExtraData>(nifList[0]); first) {
    numSubParts = first->GetData() / 2u;
  }

  if (numSubParts > 0u) {
    // -- Pack

    struct PartIndices_t {
      int path_id = -1; // index to a NiStringExtraData containing a path.
      int data_id = -1; // index to either a NiNode or a NiPixelData.
    };
    
    std::vector<PartIndices_t> nodeParts;
    nodeParts.reserve(numSubParts);

    std::vector<PartIndices_t> pixelParts;
    pixelParts.reserve(numSubParts);

    // Retrieve each part indices.    
    for (int i = 0; i < (int)nifList.size(); ++i) {
      auto n0 = nifList[i];
      if (auto nifString = Niflib::DynamicCast<Niflib::NiStringExtraData>(n0); nifString) {
        auto n1 = nifList[i+1];
        if (auto nifNode = Niflib::DynamicCast<Niflib::NiNode>(n1); nifNode && (nifNode->GetName() == "SceneNode")) {
          nodeParts.push_back( {i, i+1} );
          ++i; continue;
        }
        if (auto nifPixelData = Niflib::DynamicCast<Niflib::NiPixelData>(n1); nifPixelData) {
          pixelParts.push_back( {i, i+1} );
          ++i; continue;
        }
      }
    }

    // Append the node part to the nodes hierarchy.
    for (size_t i = 0; i < nodeParts.size(); ++i) {
      auto const nodeId = nodeParts[i].data_id;
      auto upperNode = Niflib::DynamicCast<Niflib::NiNode>(nifList[nodeId]);
      fillNodes(nullptr, upperNode);

      // TODO (optional) :
      //  * set part's mesh path.
      //  * set part's texture path.
    }
  } else if (auto upperNode = Niflib::DynamicCast<Niflib::NiNode>(nifList[0]); upperNode) {
    // -- Part's hierarchy.
    fillNodes(nullptr, upperNode);
  } else if (auto upperNode = Niflib::DynamicCast<Niflib::NiPixelData>(nifList[0]); upperNode) {
    // -- Single Texture Data.
  }

  // --------------

  // Remove excess size (happens when meshes does not have joints attributes).
  Accessors.resize(current_accessor);

  // --------------

  tinygltf::Model data{};
  {
    data.asset.generator = "nif2gltf";
    data.asset.version = "2.0";

    data.meshes = std::move( Meshes );
    data.materials = std::move( Materials );
    data.accessors = std::move( Accessors );
    data.bufferViews = std::move( BufferViews );
    data.buffers = std::move( Buffers );
    data.nodes = std::move( Nodes );

    data.images = std::move( Images );
    data.textures = std::move( Textures );
    data.samplers = std::move( Samplers );

    //data.skins;
    //data.animations;

    //data.cameras;
    //data.lights;
    
    // ----

    tinygltf::Scene scene;
    if constexpr(true) {
      scene.nodes.push_back(0);
    } else {
      scene.nodes.resize(data.nodes.size());
      std::iota(scene.nodes.begin(), scene.nodes.end(), 0);
    }
    data.scenes.push_back(scene);
  }

  // ------

  constexpr bool bEmbedImages = true;
  constexpr bool bEmbedBuffers = true;
  constexpr bool bPrettyPrint = true;
  constexpr bool bWriteBinary = false;

  std::string const gltfFilename{
    (kDebugOutput) ? "out.gltf" :
    [](std::string s, std::string const& ext) -> std::string {
      return s.replace(
        s.begin() + s.find_last_of(".") + 1, 
        s.end(), 
        ext
      ).substr(s.find_last_of(kSystemPathSeparator) + 1);
    }(nifFilename, bWriteBinary ? "glb" : "gltf")
  };

  if (tinygltf::TinyGLTF gltf; !gltf.WriteGltfSceneToFile(
      &data, gltfFilename, bEmbedImages, bEmbedBuffers, bPrettyPrint, bWriteBinary
    )) 
  {
    DOJIMA_LOG( "[Error] glTF file writing failed." );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}