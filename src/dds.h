#ifndef DDS_H_
#define DDS_H_

#include <cstdint>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------

namespace {

// Make a Little-Endian word out of a 4 bytes string.
constexpr uint32_t MakeFourCC(const char w[4]) {
  return (w[3] << 24) | (w[2] << 16) | (w[1] << 8) | (w[0] << 0);
}

}

// ----------------------------------------------------------------------------

//
// Helps construct a DDS file (DXT 1/3/5) from an already compressed pixel buffer.
//
struct DDS {
  enum SurfaceDescBits {
    DDSD_CAPS                  = 0x00000001,
    DDSD_HEIGHT                = 0x00000002,
    DDSD_WIDTH                 = 0x00000004,
    DDSD_PITCH                 = 0x00000008,
    DDSD_PIXELFORMAT           = 0x00001000,
    DDSD_MIPMAPCOUNT           = 0x00020000,
    DDSD_LINEARSIZE            = 0x00080000,
    DDSD_DEPTH                 = 0x00800000,
    
    DDSD_TEXTURE               = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT,
  };

  enum PixelFormatBits {
    DDPF_ALPHAPIXELS           = 0x00000001,
    DDPF_ALPHA                 = 0x00000002,
    DDPF_FOURCC                = 0x00000004,
    DDPF_PALETTEINDEXED8       = 0x00000020,
    DDPF_RGB                   = 0x00000040,
    DDPF_LUMINANCE             = 0x00020000,

    DDPF_RGBA                  = DDPF_RGB | DDPF_ALPHAPIXELS,
  };

  enum SurfaceBits {
    DDSCAPS_COMPLEX            = 0x00000008,
    DDSCAPS_TEXTURE            = 0x00001000,
    DDSCAPS_MIPMAP             = 0x00400000,
  };

  enum CubemapBits {
    DDSCAPS2_CUBEMAP           = 0x00000200,
    DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400,
    DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800,
    DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000,
    DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000,
    DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000,
    DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000,
    DDSCAPS2_VOLUME            = 0x00200000,
  };

#pragma pack(push,1)

  struct PixelFormat {
    uint32_t size;
    uint32_t flags;
    uint32_t fourCC;
    uint32_t RGBBitCount;
    uint32_t RBitMask;
    uint32_t GBitMask;
    uint32_t BBitMask;
    uint32_t ABitMask;
  };

  struct Header {
    uint32_t size;
    uint32_t flags;
    uint32_t height;
    uint32_t width;
    uint32_t pitchOrLinearSize;
    uint32_t depth;
    uint32_t mipMapCount;
    uint32_t reserved[11];
    PixelFormat pixelFormat;
    uint32_t surfaceFlags;
    uint32_t cubemapFlags;
    uint32_t reserved2[3];
  };

#pragma pack(pop)

  static constexpr uint32_t kMagicNumber{ MakeFourCC("DDS ") };
  static_assert( DDS::kMagicNumber == 0x20534444 );
  
  static constexpr size_t kPixelFormatSize{ sizeof(PixelFormat) };
  static constexpr PixelFormat kDXT1{ kPixelFormatSize, DDPF_FOURCC, MakeFourCC("DXT1"), 0, 0, 0, 0, 0 };
  static constexpr PixelFormat kDXT3{ kPixelFormatSize, DDPF_FOURCC, MakeFourCC("DXT3"), 0, 0, 0, 0, 0 };
  static constexpr PixelFormat kDXT5{ kPixelFormatSize, DDPF_FOURCC, MakeFourCC("DXT5"), 0, 0, 0, 0, 0 };

  using ByteBuffer = std::vector<uint8_t>;
  ByteBuffer bytes;

  void pushBytes(uint8_t const* data, size_t size) {
    bytes.insert(bytes.end(), data, data + size);
  }

  bool build(uint32_t lvl, uint32_t w, uint32_t h, uint32_t nifFormat, uint32_t const mask[4], ByteBuffer const& pixelBytes) {
    Header header{};
    header.size = sizeof( header );
    header.flags = DDSD_TEXTURE | DDSD_LINEARSIZE | DDSD_MIPMAPCOUNT;
    header.height = h;
    header.width = w;
    header.pitchOrLinearSize = w * h;
    header.mipMapCount = lvl;
    header.surfaceFlags = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
    
    switch (nifFormat) {
      case 4: // PX_FMT_DXT1
        header.pixelFormat = kDXT1; 
        header.pitchOrLinearSize /= 2;
      break;
      
      case 5: // PX_FMT_DXT3
        header.pixelFormat = kDXT3;
      break;

      case 6: // PX_FMT_DXT5
        header.pixelFormat = kDXT5;
      break;

      default:
        std::cerr << "Not DDS format " << nifFormat << " is not supported." << std::endl;
      return false;
    }

    // TODO ?
    header.pixelFormat.RBitMask = mask[0];
    header.pixelFormat.GBitMask = mask[1];
    header.pixelFormat.BBitMask = mask[2];
    header.pixelFormat.ABitMask = mask[3];

    size_t const bytesize{ sizeof(kMagicNumber) + sizeof(header) + pixelBytes.size() };
    bytes.reserve(bytesize);
    bytes.clear();

    pushBytes((uint8_t *const)&kMagicNumber, sizeof(kMagicNumber));
    pushBytes((uint8_t *const)&header, sizeof(header));
    pushBytes((uint8_t *const)pixelBytes.data(), pixelBytes.size());

    return true;
  }

  void save(std::string_view filename) {
    if (FILE *fd = fopen(filename.data(), "wb"); fd) {
      fwrite( bytes.data(), 1, bytes.size(), fd);
    }
  }
};

// ----------------------------------------------------------------------------

#endif // DDS_H_
