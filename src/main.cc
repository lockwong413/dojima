// ----------------------------------------------------------------------------
//
// [ Roadmap - TODO ]
//
//      * Fix mesh attributes buffers to remove NIF's BINORMAL and
//  recompute TANGENT to be true VEC4 (this will requiered to change bufferView and accessors too).
//
//      * Export materials with diffuse texture properly.
//
//      * Export normal textures.
//
//
// Use a glTF validator to check errors and warnings.
//  ( https://github.khronos.org/glTF-Validator/ )
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
#endif

// NIF importer.
#include "nif/niflib.h"
#include "nif/gen/Header.h"
#include "nif/obj/NiNode.h"
#include "nif/obj/NiMesh.h"
#include "nif/obj/NiDataStream.h"
#include "nif/obj/NiSourceTexture.h"
#include "nif/obj/NiPixelData.h"
#include "nif/obj/NiStringExtraData.h"
#include "nif/obj/NiIntegerExtraData.h"
#include "nif/obj/NiTexturingProperty.h"
#include "nif/obj/NiAlphaProperty.h"
#include "nif/obj/NiMaterialProperty.h"
#include "nif/obj/NiStencilProperty.h"

// Convert float16 to float32.
#include "half/half.hpp"
using half_float::half;

// Decompress DXT1/DXT5.
#define S3TC_IMPLEMENTATION
#include "s3tc/s3tc.h"

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

  // --------------

  //
  // WotS4 has 3 data streams per Mesh :
  //    * USAGE_VERTEX_INDEX  : (INDEX)
  //    * USAGE_VERTEX        : (Texcoord, Position, Normal, Binormal, Tangent, BlendIndice, BlendWeight)
  //    * USAGE_USER          : (BONE_PALETTE)
  //
  //  Mesh processing :
  //
  // 1) We create 2 buffers for indices and vertices (we don't use BONE_PALETTE for now).
  //     [ better approach : use one buffer per LODs ]
  //
  // 2) For each NiMesh we create :
  //    * 2 buffer_views for indices and vertices (one per used datastreams).
  //
  // 3) For each mesh's submeshes we create :
  //    * 1 accessors for indices.
  //    * 7 accessors for vertices.
  //
  // Note that "Binormal" are not supported by glTF 2.0 as they can be retrieved
  // from normal + tangent but to avoid further buffer reparameterization we
  // provide them anyways. 
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
      verticesBytesize += bytesize; //
    }
  }

  // Note : 
  //  When the buffer contains *only* half floating point values its size
  //  should theorically be doubled for converting it to float32.
  //  We use this only as a preallocation scheme, but as for WotS4
  //  the biggest files are float16 maps anyways we always allocate enough.
  verticesBytesize *= 2;

  // *Theorical* uncompressed total bytelength for images.
  size_t constexpr kRawPixelsDefaultSize{ 4 * 1024 * 1024 }; //
  size_t const kNumTextures{ getNumBlocks(kNiPixelDataKey) };
  size_t const imagesBytesize = kRawPixelsDefaultSize * kNumTextures;

  // Create the base glTF buffers.
  std::vector<tinygltf::Buffer> Buffers(static_cast<size_t>(BufferId::kCount));
  
  auto &IndexBuffer = Buffers[(int)BufferId::INDEX];
  IndexBuffer.name = "Indices";
  IndexBuffer.data.reserve(indicesBytesize);

  auto &VertexBuffer = Buffers[(int)BufferId::VERTEX];
  VertexBuffer.name = "Vertices";
  VertexBuffer.data.reserve(verticesBytesize);

  // (wip)
  // auto &ImageBuffer = Buffers[(int)BufferId::IMAGE];
  // ImageBuffer.name = "Images";
  // ImageBuffer.data.reserve(imagesBytesize); //
  
  // Offset to current buffers' end, used to build buffer viewers.
  size_t indexBufferOffset = 0;
  size_t vertexBufferOffset = 0;
  size_t imageBufferOffset = 0;

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
  std::vector<tinygltf::Mesh> Meshes( kNiMeshCount );
  
  // BufferViews buffers.
  size_t const kNumBufferViews = Buffers.size() * kNiMeshCount; //
  std::vector<tinygltf::BufferView> BufferViews( kNumBufferViews );

  // Submeshes buffers.
  size_t const kNumAttributes = static_cast<size_t>(AttributeId::kCount); // (might be lower)
  size_t constexpr kNumAccessorPerSubmesh{ 1 + kNumAttributes }; 
  std::vector<tinygltf::Accessor> Accessors( totalSubmeshesCount * kNumAccessorPerSubmesh );

  // Materials buffer.
  // Depends on textures, MaterialData & NiMaterialProperty. 
  std::vector<tinygltf::Material> Materials( kNiMeshCount ); // (might be lower)

  // [ TODO ]
  // Samplers buffer.
  std::vector<tinygltf::Sampler> Samplers( 1 ); //
  // std::map< uint16_t, size_t > mapTexDescFlagToSamplerIndex;
  
  // Index to available buffer slot.
  size_t current_buffer_view = 0;
  size_t current_accessor = 0;
  size_t current_material = 0; //

  // -- Texture extraction helpers.

  // Textures buffers.
  // (There are approximatively 4 textures per mesh part or 1 per NiPixelData, but we don't
  // use all of them).
  std::vector<tinygltf::Image> Images;
  std::vector<tinygltf::Texture> Textures;
  Images.reserve( kNumTextures );
  Textures.reserve( kNumTextures );

  // Link a texture's path/name to its Image-Texture index.
  std::unordered_map< std::string, size_t > mapTextureNameToIndex;

  // Buffer to convert DXT compressed texture to raw pixels.
  std::vector<uint8_t> rawPixels(kRawPixelsDefaultSize);

  // Helper to build and uncompress DDS from compressed bytes.
  // DDS dds;

  // [lambda] Extract an internal DDS texture and set texture image buffers.
  auto processInternalTexture{[&](Niflib::Ref<Niflib::NiPixelData> const& nifPixelData, std::string const& texFilename, size_t* texIndex) -> size_t {
    // Check the texture has not been loaded yet.
    if (auto it = mapTextureNameToIndex.find(texFilename); it != mapTextureNameToIndex.end()) {
      DOJIMA_QUIET_LOG(texFilename << " already loaded.");
      *texIndex = it->second;
      return true;
    }

    auto const fmt = nifPixelData->GetPixelFormat(); 
    if ((Niflib::PX_FMT_DXT1 != fmt) && 
        (Niflib::PX_FMT_DXT5 != fmt)) {
      DOJIMA_QUIET_LOG( texFilename << " [" << fmt << "] : non DXT1 / DXT5 textures are not supported yet." );
      return false;
    }
    auto decompressDXT = (Niflib::PX_FMT_DXT1 == fmt) ? s3tc::DecompressDXT1 
                                                      : s3tc::DecompressDXT5;

    // Add a new texture data.
    Images.push_back( tinygltf::Image() );
    auto &img = Images.back();
    
    Textures.push_back( tinygltf::Texture() );
    auto &tex = Textures.back();

    // Update the Texture LUT with the new image/texture index using its full name.
    *texIndex = Images.size() - 1;
    mapTextureNameToIndex[texFilename] = *texIndex;

    // Extract the texture's basename, removing its path and extension.
    auto const texName{ 
      [](std::string const& fn, size_t pos) -> std::string { 
        size_t startIndex = (pos != std::string::npos) ? pos + 1 : 0;
        return fn.substr(startIndex, fn.find_last_of('.')); 
      }(texFilename, texFilename.find_last_of('/'))
    };

    img.name = texName;
    img.width = static_cast<uint32_t>(nifPixelData->GetWidth());
    img.height = static_cast<uint32_t>(nifPixelData->GetHeight());
    img.component = 4;
    img.bits = 8;
    img.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;

    // Transform the DDS compressed data to PNG for gltf 2.0 compatibility.
    {
      // (all mipmaps are flatten on the first level)
      auto const& pixelBytes = nifPixelData->GetMipMaps()[0];

      // [optional] Reconstruct the DDS file from data.
      // uint32_t const levels = nifPixelData->GetNumMipMaps(); //
      // uint32_t const mask[4]{ 
      //   0x000000ff, // nifPixelData->GetRedMask(), 
      //   0x0000ff00, // nifPixelData->GetGreenMask(), 
      //   0x00ff0000, // nifPixelData->GetBlueMask(), 
      //   0xff000000, // nifPixelData->GetAlphaMask(), 
      // };
      // dds.build(levels, img.width, img.height, Niflib::PX_FMT_DXT1, mask, pixelBytes);
      
      // Decompress the internal DDS texture.
      size_t const rowStride = img.component * img.width;
      rawPixels.resize( rowStride * img.height );
      decompressDXT(img.width, img.height, pixelBytes.data(), (uint32_t *)rawPixels.data());

      // if (Niflib::PX_FMT_DXT1 == fmt) {
      //   s3tc::DecompressDXT1(img.width, img.height, pixelBytes.data(), (uint32_t *)rawPixels.data());
      // } else if (Niflib::PX_FMT_DXT5 == fmt) {
      //   s3tc::DecompressDXT5(img.width, img.height, pixelBytes.data(), (uint32_t *)rawPixels.data());
      // }

      // Save image as PNG.
      if constexpr (kDebugOutputPNG) {
        // (external)

        std::string const uri{img.name + ".png"};
        stbi_write_png(uri.c_str(), img.width, img.height, img.component, rawPixels.data(), rowStride);
      } else {
        // (internal)

        auto pngToMemory{[](void *context, void *data, int size) -> void {
          auto img = reinterpret_cast<tinygltf::Image*>(context);
          auto pngBytes = reinterpret_cast<uint8_t const*>(data);  
          img->image.clear();
          img->image.insert(img->image.end(), pngBytes, pngBytes + size);
        }};
        int const writeSuccess{stbi_write_png_to_func(
          pngToMemory, &img, img.width, img.height, img.component, rawPixels.data(), rowStride
        )};

        // [ TODO ] Fill internal image buffer.
        if (writeSuccess) {
          // auto& bufferView = BufferViews[current_buffer_view++];
          // img.mimeType = "image/png";
          // img.bufferView = current_buffer_view;
          // bufferView.buffer = (int)BufferId::IMAGE;
          // bufferView.byteOffset = imageBufferOffset;
          // bufferView.byteLength = img.image.size();
          // imageBufferOffset += bufferView.byteLength;
        }
      }
    }

    // Set the texture to the textureMap.
    tex.sampler = 0; //
    tex.source = *texIndex;

    // [ TODO ]
    // Handle texture transforms.

    return true;
  }};

  // [lambda] Wrapper for processInternalTexture using Niflib::TexDesc.
  // When the texture is available the texIndex is set and the function return true.
  auto processTexDesc{[processInternalTexture](Niflib::TexDesc const& texDesc, size_t* texIndex) -> bool {
    auto srcTex = texDesc.source;
    if (nullptr == srcTex) {
      return false;
    }
    if (srcTex->IsTextureExternal()) {
      DOJIMA_QUIET_LOG( "[ Warning ] External texture loading is not supported." );
      return false;
    }
    // [ TODO ]
    // Retrieve sampler from texdesc flag and samplerMap.
    return processInternalTexture(srcTex->GetPixelData(), srcTex->GetTextureFileName(), texIndex);
  }};

  // -- Meshes

  // Associate a mesh name to its internal id.
  std::unordered_map<std::string, size_t> mapMeshNameToId;

  auto const& meshIndices = getTypeListIndices( kNiMeshKey );
  for (size_t mesh_id = 0; mesh_id < meshIndices.size(); ++mesh_id) {
    auto const nifMesh = Niflib::StaticCast<Niflib::NiMesh>(nifList[meshIndices[mesh_id]]);
    auto const primType = GetPrimitiveType(nifMesh->GetPrimitiveType());
    auto &mesh = Meshes.at(mesh_id);
    
    // Name.
    mesh.name = nifMesh->GetName();
    mapMeshNameToId[mesh.name] = mesh_id;

    // ------------------------
    // Material.
    // 
    //  WotS NiMesh have the following properties :
    //    * NiMaterialProperty (with non PBR values),
    //    * NiAlphaProperty
    //    * NiSpecularProperty
    //    * NiVertexColorProperty
    //    * NiTexturingProperty, with a varying number of NiSourceTexture, each with a NiPixelData :
    //          + a Base texture (FMT_DXT3), seems useless.
    //          + a Detail Texture (FMT_DXT1), a RGB(A) mask.
    //          + a Normal Texture (FMT_DXT1).
    //          + a Gloss Texture (FMT_DXT1).
    //
    //  ( After each root's "part" node, a NiPixelData is defined containing the 
    //   diffuse texture (DXT1) for this part's mesh )
    // 
    size_t const meshMaterialIndex = current_material++; //
    auto &material = Materials[meshMaterialIndex];
    // material.name = mesh.name + "::material";
    
    // Prefer using the specular or SpecularGlossiness extensions, see :
    //   https://kcoley.github.io/glTF/extensions/2.0/Khronos/KHR_materials_pbrSpecularGlossiness/
    // if (auto prop = nifMesh->GetPropertyByType(Niflib::NiVertexColorProperty::TYPE); prop) {}
    // if (auto prop = nifMesh->GetPropertyByType(Niflib::NiSpecularProperty::TYPE); prop) {}

    // Basic / non-PBR parameters. 
    if (auto prop = nifMesh->GetPropertyByType(Niflib::NiMaterialProperty::TYPE); prop) {
      auto matProp = Niflib::StaticCast<Niflib::NiMaterialProperty>(prop);

      Niflib::Color3 const& emissive{ matProp->GetEmissiveColor() };
      material.emissiveFactor = { emissive.r, emissive.g, emissive.b };

      // Technically not for metal roughness PBR but well.
      Niflib::Color3 const& diffuse{ matProp->GetDiffuseColor() };
      material.pbrMetallicRoughness.baseColorFactor = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    }

    // Alpha Testing & Blending.
    if (auto prop = nifMesh->GetPropertyByType(Niflib::NiAlphaProperty::TYPE); prop) {
      auto alphaProp = Niflib::StaticCast<Niflib::NiAlphaProperty>(prop);

      auto const alphaTestFunc = alphaProp->GetTestFunc();
      switch (alphaTestFunc) {
        case Niflib::NiAlphaProperty::TF_LESS:
          material.alphaMode = "BLEND";
        break;

        case Niflib::NiAlphaProperty::TF_GREATER:
          // (weirdly, some body parts have this flag set for no clear reasons,
          // should we discard them ?)
        case Niflib::NiAlphaProperty::TF_GEQUAL:
          material.alphaMode = "MASK";
          material.alphaCutoff = alphaProp->GetTestThreshold() / 255.0;
        break;

        case Niflib::NiAlphaProperty::TF_ALWAYS:
        default:
          material.alphaMode = "OPAQUE";
        break;
      }
    }

    // Check for double sided geometry.
    if (auto prop = nifMesh->GetPropertyByType(Niflib::NiStencilProperty::TYPE); prop) {
      auto stencilProp = Niflib::StaticCast<Niflib::NiStencilProperty>(prop);
      material.doubleSided = (Niflib::DRAW_BOTH == stencilProp->GetFaceDrawMode());
    }

    // Image / Texture.    
    if (auto prop = nifMesh->GetPropertyByType(Niflib::NiTexturingProperty::TYPE); prop) {
      auto texProp = Niflib::StaticCast<Niflib::NiTexturingProperty>(prop);
      size_t texIndex;

      // Normal Texture.
      if (processTexDesc(texProp->GetNormalTexture(), &texIndex)) {
        // material.normalTexture.index = texIndex;
      }
      
      // "Specular" Texture.
      // processTexDesc(texProp->GetGlossTexture(), &texIndex);

      // Some RGBA mask.
      // processTexDesc(texProp->GetDetailTexture(), &texIndex);
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

          if (0 == numHalfPrecisionBuffers) {
            VertexBuffer.data.insert(VertexBuffer.data.end(), data.cbegin(), data.cend());
          } else if (numHalfPrecisionBuffers == md.numComponents) {
            // We handle half precision buffer conversion only when all the attributes
            // in the buffer are half-precision.

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
            DOJIMA_QUIET_LOG( "[Warning] Sparsed half precision buffers are not supported ." );
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

            // Set acc default params depending on format.
            auto &acc = Accessors[current_accessor++];
            size_t const byteSize = SetAccessorFormat(format, &acc);
            
            // Set the primitive attribute accessor.
            SetAttribute(sem, accessorIndex, &prim, &acc);

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

  // --------------

  // -- Node hierarchy.

  // Determine the total number of hierarchical nodes.
  size_t totalNodeCount = 0;
  for (auto const& n : nifList) {
    auto ptr = Niflib::DynamicCast<Niflib::NiAVObject>(n);
    totalNodeCount += (ptr != nullptr) ? 1 : 0;
  }

  std::vector<tinygltf::Node> Nodes;
  Nodes.reserve(totalNodeCount);

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

      if (auto nifMesh = Niflib::DynamicCast<Niflib::NiMesh>(nifAVO); nifMesh) {
        node.mesh = mapMeshNameToId[node.name];
      }

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


  // (on NIF's "Characters Pack")
  //
  //  The first node is a "NiIntegerExtraData" and contains the number of
  //   "NiStringExtraData" on the upper root.
  //
  // Then for each NiStringExtraData, if the following object has a consecutive index
  // it contains the path for that part's data and is alternatively a NiNode or a NiPixelData.
  //
  // The PixelData representing the DDS DXT1/5 diffuse texture for the mesh.
  //

  // Detects if the NIF file is a pack.
  uint32_t numSubParts = 0u;
  if (auto firstNode = Niflib::DynamicCast<Niflib::NiIntegerExtraData>(nifList[0]); firstNode) {
    numSubParts = firstNode->GetData() / 2u;
  }

  if (bool const bNIFPack = (numSubParts > 0u); bNIFPack) {
    // -- NIF Pack.

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
          nodeParts.push_back( {i, ++i} );
          continue;
        }
        if (auto nifPixelData = Niflib::DynamicCast<Niflib::NiPixelData>(n1); nifPixelData) {
          pixelParts.push_back( {i, ++i} );
          continue;
        }
      }
    }

    // Append the node part to the nodes hierarchy.
    for (size_t i = 0; i < nodeParts.size(); ++i) {
      // Geometry.
      auto const nodeId = nodeParts[i].data_id;
      auto upperNode = Niflib::StaticCast<Niflib::NiNode>(nifList[nodeId]);
      fillNodes(nullptr, upperNode);

      // Diffuse texture.
      auto const pixelPart = pixelParts[i];
      auto texPathNode = Niflib::StaticCast<Niflib::NiStringExtraData>(nifList[pixelPart.path_id]);
      auto texPixelNode = Niflib::StaticCast<Niflib::NiPixelData>(nifList[pixelPart.data_id]);
      if (size_t texIndex; processInternalTexture(texPixelNode, texPathNode->GetData(), &texIndex)) {
        // [ TODO ]
        // Associate each meshes material diffuse texture with the one defined by
        // their following upper texPixelNode (check with ID). 

        // auto &pbr = material.pbrMetallicRoughness;
        // pbr.baseColorTextureindex = -1;  // required.
        // pbr.baseColorTexture.texCoord; // (texcoord index)
      }
    }
  } else if (auto firstNode = Niflib::DynamicCast<Niflib::NiNode>(nifList[0]); firstNode) {
    // -- NIF Part.
    fillNodes(nullptr, firstNode);
  } else if (auto firstNode = Niflib::DynamicCast<Niflib::NiPixelData>(nifList[0]); firstNode) {
    // -- NIF PixelData.
    // WotS4 PixelData-only NIF files are encoded as BIG_ENDIAN and supposedly from a
    // version unsupported by Niflib so we can't read them as is. 
    // Consequently we never reach this branch as Niflib::ReadHeader will crash before.
  }

  // --------------

  // std::cerr << "Nodes usage : " << Nodes.size() << " / " << Nodes.capacity() 
  //           << std::endl;

  // Remove excess size (happens when meshes do not have joints attributes).
  Accessors.resize(current_accessor);

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

    if constexpr(false) {
      data.images = std::move( Images );
      data.textures = std::move( Textures );
      data.samplers = std::move( Samplers );
    }

    //data.skins;
    //data.animations;
    //data.cameras;
    //data.lights;

    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    data.scenes.push_back(scene);
  }

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
      &data, gltfFilename, kGLTFEmbedImages, kGLTFEmbedBuffers, kGLTFPrettyPrint, kGLTFWriteBinary
    )) 
  {
    DOJIMA_LOG( "[Error] glTF file writing failed." );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
