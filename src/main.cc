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
#include "nif/obj/NiMesh.h"
#include "nif/obj/NiNode.h"
#include "nif/obj/NiDataStream.h"

// #define CGLTF_IMPLEMENTATION
// #define CGLTF_WRITE_IMPLEMENTATION
// #include "cgltf/cgltf_write.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tinygltf/tiny_gltf.h"

// ----------------------------------------------------------------------------

static constexpr bool kDisplayHeader = false;

// ----------------------------------------------------------------------------

// Key of interests.
std::string const kNiDataStreamKey{ "NiDataStream" };
std::string const kNiMeshKey{ "NiMesh" };
std::string const kNiNodeKey{ "NiNode" };

// ----------------------------------------------------------------------------

// Globals buffers type output in the GLTF.
enum class BufferType : uint8_t {
  INDEX,
  VERTEX,

  kCount
};

// Attributes output in the GLTF file.
enum class AttributeId : uint8_t {
  Position,
  Texcoord,
  Normal,
  Binormal, //
  Tangent,
  Joints,
  Weights,
  
  kCount
};

// ----------------------------------------------------------------------------

static bool SetAttribute(Niflib::SemanticData cs, size_t accessorIndex, tinygltf::Primitive *prim, bool *bNormalized) {
  std::string const name = cs.name;

  if (cs.index > 0) {
    std::cerr << "[Warning] " << __FUNCTION__ << " : extra attribute not used." << std::endl; 
    return false;
  }

  if ((name == "POSITION") || (name == "POSITION_BP")) {
    prim->attributes["POSITION"] = accessorIndex;
    *bNormalized = false;
  } 
  else if ((name == "NORMAL") || (name == "NORMAL_BP")) {
    prim->attributes["NORMAL"] = accessorIndex;
    *bNormalized = true;
  } 
  else if (name == "TEXCOORD") {
    prim->attributes["TEXCOORD_0"] = accessorIndex;
    // *bNormalized = true;
  } 
  else if (name == "BLENDINDICES") {
    prim->attributes["JOINTS_0"] = accessorIndex;
    *bNormalized = false;
  } 
  else if (name == "BLENDWEIGHT") {
    prim->attributes["WEIGHTS_0"] = accessorIndex;
    *bNormalized = true;
  } 
  else if (name == "TANGENT_BP") {
    prim->attributes["TANGENT"] = accessorIndex;
    *bNormalized = true;
  }
  else if ((name == "BINORMAL") || (name == "BINORMAL_BP")) {
    prim->attributes["BINORMAL"] = accessorIndex;
    *bNormalized = true;
  }
  else {
#ifndef NDEBUG
    std::cerr << "[Warning] " << __FUNCTION__ << " : " << name << " component not used." << std::endl; 
#endif
    return false;
  }

  return true;
}

static uint32_t GetPrimitiveType(Niflib::MeshPrimitiveType primType) {
  switch (primType) {
    case Niflib::MESH_PRIMITIVE_TRIANGLES:
    return TINYGLTF_MODE_TRIANGLES;

    case Niflib::MESH_PRIMITIVE_TRISTRIPS:
    return TINYGLTF_MODE_TRIANGLE_STRIP;

    case Niflib::MESH_PRIMITIVE_LINESTRIPS:
    return TINYGLTF_MODE_LINE_STRIP;

    default: 
    case Niflib::MESH_PRIMITIVE_POINTS:
    return TINYGLTF_MODE_POINTS;
  };
}

static size_t SetAccessorFormat(Niflib::ComponentFormat format, tinygltf::Accessor &accessor) {
  switch(format) {
    case Niflib::F_UINT16_1:
      accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
      accessor.type = TINYGLTF_TYPE_SCALAR;
    return 1 * sizeof(uint16_t);

    case Niflib::F_FLOAT32_2:
      accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessor.type = TINYGLTF_TYPE_VEC2;
    return 2 * sizeof(float);
    
    case Niflib::F_FLOAT32_3:
      accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
      accessor.type = TINYGLTF_TYPE_VEC3;
    return 3 * sizeof(float);

    case Niflib::F_UINT8_4:
      accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      accessor.type = TINYGLTF_TYPE_VEC4;
    return 4 * sizeof(uint8_t);

    default:
#ifndef NDEBUG
    std::cerr << "[Warning] " << __FUNCTION__ << " : " << format << " not found." << std::endl; 
#endif
    return 0;
  };
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr <<"Usage : " << std::endl << argv[0] << " nif_file" << std::endl;
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
  std::vector<uint32_t> blockTypeCount(blockTypes.size(), 0);
  // List of node indices from their types.
  std::unordered_map< std::string, std::vector<uint32_t> > typeToListIndices;
  {
    int current_index = 0;
    for (auto const bti : blockTypeIndex) {
      ++blockTypeCount[bti];
      typeToListIndices[blockTypes[bti]].push_back(current_index++);
    }
  }

  // Block type name to base index.
  std::unordered_map<std::string, uint32_t> typeToIndex;
  for (uint32_t i = 0; i < blockTypes.size(); ++i) {
    typeToIndex[blockTypes[i]] = i;
  }

  // Indices to differents datastreams types.
  std::vector<uint32_t> dataStreamIndices;
  for (auto const& [key, indices] : typeToListIndices) {
    if (key.find(kNiDataStreamKey) != std::string::npos) {
      dataStreamIndices.insert(dataStreamIndices.end(), indices.cbegin(), indices.cend());
    }
  }

  // --------------

  //  Return the number of block for a given block name.
  auto getNumBlocks = [&](std::string const& key) -> uint32_t {
    auto it = typeToIndex.find(key);
    auto const count = (it == typeToIndex.end()) ? 0 : blockTypeCount.at( it->second ); 
    // std::cerr << key << " found : " << count << std::endl;
    return count;
  };

  // Return the list of node indices for a given block name.
  auto getTypeListIndices = [&](std::string const& key) -> std::vector<uint32_t> {
    auto it = typeToListIndices.find(key);
    return (it == typeToListIndices.end()) ? std::vector<uint32_t>() : it->second; 
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

    std::cerr << " * Blocks type index (" << blockTypeIndex.size() << ") :" << std::endl;
    for (auto const &bti : blockTypeIndex) {
      std::cerr << bti << ", ";
    }
    std::cerr << std::endl;
  }

  // --------------

  // WotS models have different LOD level Mesh.
  // Some levels mesh's datastream can link to the same data (accessor / buffer)
  // (Nodes from differents LOD can use the same datas)

  // WOTS4 has 3 data streams per Mesh :
  //    * USAGE_VERTEX_INDEX  : (INDEX)
  //    * USAGE_VERTEX        : (Texcoord, Position, Normal, Binormal, Tangent, BlendIndice, BlendWeight)
  //    * USAGE_USER          : (BONE_PALETTE)

  auto nifList = Niflib::ReadNifList(nifFilename);
  auto nifTree = Niflib::ReadNifTree(nifFilename);

  // --------------

  // -- Let's say --
  //
  // 1) We create 2 buffers for indices and vertices. 
  //       (we don't use BONE_PALETTE for now)
  //
  // 2) For each NiMesh we create :
  //    * 2 buffer_views for indices and vertices (one per used datastreams).
  //
  // 3) For each mesh' submeshes we create :
  //    * 1 accessors for indices.
  //    * 7 accessors for vertices.
  //    * 7 attributes for vertices.
  //
  // [ better approach ] use one buffer per LODs.
  //

  // Determine the bytesize of the global Index and Vertices buffers.
  size_t indicesBytesize = 0;
  size_t verticesBytesize = 0;
  for (auto index : dataStreamIndices) {
    Niflib::NiObject *obj = nifList[index];
    Niflib::NiDataStream* nifDataStream = (Niflib::NiDataStream*)obj;
    auto const usage = nifDataStream->GetUsage();
    
    if (usage == Niflib::USAGE_VERTEX_INDEX) {
      indicesBytesize += nifDataStream->GetNumBytes();
    } else if (usage == Niflib::USAGE_VERTEX) {
      verticesBytesize += nifDataStream->GetNumBytes();
    }
  }

  // Create the base Index & Vertex buffers object.
  std::vector< tinygltf::Buffer > Buffers(static_cast<size_t>(BufferType::kCount));
  
  auto &IndexBuffer = Buffers[(int)BufferType::INDEX];
  IndexBuffer.name = "Indices";
  IndexBuffer.data.reserve(indicesBytesize);

  auto &VertexBuffer = Buffers[(int)BufferType::VERTEX];
  VertexBuffer.name = "Vertices"; //
  VertexBuffer.data.reserve(verticesBytesize);
  
  // Current offset to buffers' end, used to build buffer_viewers.
  size_t indexBufferOffset = 0;
  size_t vertexBufferOffset = 0;

  // Retrieve the number of NIF mesh nodes.
  uint32_t const kNiMeshCount{ getNumBlocks(kNiMeshKey) };
  uint32_t submeshesCount{ 0 };
  {
    auto const& meshIndices = getTypeListIndices( kNiMeshKey );
    for (auto mesh_id : meshIndices) {
      Niflib::NiObject *obj = nifList[mesh_id];
      Niflib::NiMesh* nifMesh = (Niflib::NiMesh*)obj;
      submeshesCount += nifMesh->GetNumSubmeshes();
    }
  }

  // Meshes buffer.
  std::vector< tinygltf::Mesh > Meshes( kNiMeshCount );
  std::vector< tinygltf::BufferView > BufferViews( Buffers.size() * kNiMeshCount );

  // Submeshes buffers.
  std::vector< tinygltf::Accessor > Accessors( (1 + static_cast<size_t>(AttributeId::kCount)) * submeshesCount );
  std::vector< tinygltf::Material > Materials( 1 ); // TODO

  // Index to available buffer slot.
  size_t current_buffer_view = 0;
  size_t current_accessor = 0;
  size_t current_material = 0;

  // --------------

  // Associate a mesh name to its internal id.
  std::unordered_map<std::string, size_t> mapMeshNameToId;

  // Setup Meshes.
  auto const& meshIndices = getTypeListIndices( kNiMeshKey );
  for (size_t mesh_id = 0; mesh_id < meshIndices.size(); ++mesh_id) {
    Niflib::NiObject *obj = nifList[meshIndices[mesh_id]];
    Niflib::NiMesh* nifMesh = (Niflib::NiMesh*)obj;

    auto const& meshDatas = nifMesh->GetMeshDatas();
    auto const primType = GetPrimitiveType(nifMesh->GetPrimitiveType());

    auto &mesh = Meshes.at(mesh_id);
    mesh.name = nifMesh->GetName();
    mapMeshNameToId[mesh.name] = mesh_id;

    // primitives / submeshes.
    uint32_t const nsubmeshes = nifMesh->GetNumSubmeshes();
    mesh.primitives.resize(nsubmeshes);

    // Create a buffer_view for each used datastream.
    uint32_t numMeshdata = 0;
    for (auto const& md : meshDatas) {
      auto const& stream = md.stream;
      auto const& usage = stream->GetUsage();

      // Bypass unused meshData.
      if ((usage == Niflib::USAGE_SHADER_CONSTANT) ||
          (usage == Niflib::USAGE_USER)) {
        std::cerr << "[Warning] " << usage << " MeshData not used." << std::endl;
        continue;
      }

      if (++numMeshdata > static_cast<uint32_t>(BufferType::kCount)) {
        std::cerr << "[Warning] An exceeding amount of internal buffers has been found." << std::endl;
        break;
      }

      // -- BUFFER VIEW.

      int const bufferViewIndex = current_buffer_view++;
      auto &bufferView = BufferViews.at(bufferViewIndex);

      bufferView.byteLength = stream->GetNumBytes(); // 
      auto const& data = stream->GetData();
      auto const& formats = stream->GetComponentFormats();

      switch (usage) {
        case Niflib::USAGE_VERTEX_INDEX:
          bufferView.buffer = (int)BufferType::INDEX;
          bufferView.byteOffset = indexBufferOffset;
          bufferView.byteStride = 0;
          bufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
          indexBufferOffset += bufferView.byteLength;

          IndexBuffer.data.insert(IndexBuffer.data.end(), data.cbegin(), data.cend());
        break;

        case Niflib::USAGE_VERTEX:
          bufferView.buffer = (int)BufferType::VERTEX;
          bufferView.byteOffset = vertexBufferOffset;
          bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
          vertexBufferOffset += bufferView.byteLength;

          bufferView.byteStride = 0;
          for (int comp_id = 0; comp_id < md.numComponents; ++comp_id) {
            tinygltf::Accessor dummy_accessor;
            size_t const byteSize = SetAccessorFormat(formats[comp_id], dummy_accessor);
            bufferView.byteStride += byteSize;
          }

          VertexBuffer.data.insert(VertexBuffer.data.end(), data.cbegin(), data.cend());
        break;

        default:
        break;
      };

      // -- ACCESSOR.
      
      auto const& regions = stream->GetRegions();
      
      for (unsigned int submesh_id = 0; submesh_id < nsubmeshes; ++submesh_id) {
        auto &prim = mesh.primitives[submesh_id];
        prim.mode = primType;

        // Creates accessors for each submeshes. 
        uint16_t const regionMap = md.submeshToRegionMap[submesh_id];
        auto const& region = regions[regionMap];
        
        if (usage == Niflib::USAGE_VERTEX_INDEX) {
          // -- Indices.

          // Set the primitive indices accessor.
          prim.indices = current_accessor++;
          
          auto &acc = Accessors[prim.indices];
          size_t const byteSize = SetAccessorFormat(formats[0], acc);

          acc.name = "INDEX"; //
          acc.byteOffset = region.startIndex * byteSize; //
          acc.count = region.numIndices;
          acc.normalized = false;
          acc.bufferView = bufferViewIndex;
        } else {
          // -- Vertex Attributes.
          
          size_t vertexBaseOffset = region.startIndex * bufferView.byteStride;
          for (int comp_id = 0; comp_id < md.numComponents; ++comp_id) {
            auto const& sem = md.componentSemantics[comp_id];
            
            // Set the primitive attribute accessor.
            bool bNormalized = false;
            SetAttribute(sem, current_accessor, &prim, &bNormalized);

            auto &acc = Accessors[current_accessor++];
            size_t const byteSize = SetAccessorFormat(formats[comp_id], acc);

            acc.name = sem.name; //
            acc.byteOffset = vertexBaseOffset; //
            acc.count = region.numIndices;
            acc.normalized = bNormalized;
            acc.bufferView = bufferViewIndex;

            vertexBaseOffset += byteSize;
          }
        }

        // TODO : material.
        {
          // auto &mat = Materials.at(0);
          prim.material = 0;
        }

      } // -- end submeshes
    } // -- end meshdatas

    // TODO : NiSkinningMeshModifier & extra properties.
    //mesh.weights;
    //mesh.extra;
  }

  // --------------

  std::vector< tinygltf::Node > Nodes;

  if (auto nifRoot = Niflib::DynamicCast<Niflib::NiNode>(nifTree); nullptr != nifRoot) {

    // Determine the total number of subnodes.
    size_t numNiAVObject = 0;
    for (auto const& n : nifList) {
      auto ptr = Niflib::DynamicCast<Niflib::NiAVObject>(n);
      numNiAVObject += (ptr != nullptr) ? 1 : 0;
    }
    Nodes.reserve( numNiAVObject );

    // Define a recursive lambda function to fill the node hierarchy.
    std::function<void(tinygltf::Node*, Niflib::NiAVObject *const)> fillNodes;
    
    fillNodes = [&mapMeshNameToId, &Nodes, &fillNodes](tinygltf::Node *parent, Niflib::NiAVObject *const nifAVO) -> void {
      // Get reference to a new node.
      Nodes.push_back( tinygltf::Node() );
      auto &node = Nodes.back();
      
      // Set node as its parent's children.
      if (parent) {
        int const nodeIndex = static_cast<int>(Nodes.size() - 1);
        parent->children.push_back(nodeIndex);
      }

      node.name = nifAVO->GetName();
      // std::cerr << node.name << std::endl;      
      
      auto const& qRotation = nifAVO->GetLocalRotation().AsQuaternion();
      node.rotation = { qRotation.w, qRotation.x, qRotation.y, qRotation.z };

      auto const fScale = nifAVO->GetLocalScale();
      node.scale = { fScale, fScale, fScale };
      
      auto const &vTranslation = nifAVO->GetLocalTranslation();
      node.translation = { vTranslation.x, vTranslation.y, vTranslation.z };

      if (auto nifMesh = Niflib::DynamicCast<Niflib::NiMesh>(nifAVO); nifMesh) {
        node.mesh = mapMeshNameToId[node.name];
      }

      // Detect mesh LOD node.
      // if (auto nifSwitchNode = Niflib::DynamicCast<Niflib::NiSwitchNode>(nifAVO); nifSwitchNode) {}

      // auto const& listXD = nifAVO->GetExtraData();
      // node.extras; // TODO    

      // Continue processing if the nif object is a node (and not a leaf).
      if (auto nifNode = Niflib::DynamicCast<Niflib::NiNode>(nifAVO); nifNode) {
        if (nifNode->IsSkeletonRoot()) {
          // node.skin;
        }

        for (auto &child : nifNode->GetChildren()) {
          fillNodes(&node, child);
        }
      }
    };
    fillNodes(nullptr, nifRoot);
  }

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

    //data.images;
    //data.textures;
    //data.samplers;
    //data.skins;
    //data.animations;

    //data.cameras;
    //data.lights;
    
    // ----

    data.nodes = std::move( Nodes );
    
    tinygltf::Scene dummy_scene;
    dummy_scene.nodes.push_back(0);

    // dummy_scene.nodes.resize(data.nodes.size());
    // std::iota(dummy_scene.nodes.begin(), dummy_scene.nodes.end(), 0);

    data.scenes.push_back(dummy_scene);
  }

  // ------

  constexpr bool bEmbedImages = true;
  constexpr bool bEmbedBuffers = true;
  constexpr bool bPrettyPrint = true;
  constexpr bool bWriteBinary = false;
  
  if (tinygltf::TinyGLTF gltf; 
    !gltf.WriteGltfSceneToFile(
      &data, "out.gltf", bEmbedImages, bEmbedBuffers, bPrettyPrint, bWriteBinary
    )) 
  {
    std::cerr << "File writing failed." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}