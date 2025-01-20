/*
Copyright (c) 2009 Benjamin Dobell, Glass Echidna

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef S3TC_H
#define S3TC_H

#include <cstdint>

namespace s3tc {

/*
  Helper method that packs RGBA channels into a single 4 byte pixel.

  @param r:     red channel.
  @param g:     green channel.
  @param b:     blue channel.
  @param a:     alpha channel.
*/  
uint32_t PackRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/*
  Decompresses one block of a DXT1 texture and stores the resulting pixels at the appropriate offset in 'image'.

  @param x:                     x-coordinate of the first pixel in the block.
  @param y:                     y-coordinate of the first pixel in the block.
  @param width:                 width of the texture being decompressed.
  @param height:                height of the texture being decompressed.
  @param blockStorage:          pointer to the block to decompress.
  @param image:                 pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockDXT1(uint32_t x, uint32_t y, uint32_t width, uint8_t const* blockStorage, uint32_t *image);

/*
  Decompresses all the blocks of a DXT1 compressed texture and stores the resulting pixels in 'image'.

  @param width:                 Texture width.
  @param height:                Texture height.
  @param blockStorage:          pointer to compressed DXT1 blocks.
  @param image:                 pointer to the image where the decompressed pixels will be stored.
*/
void DecompressDXT1(uint32_t width, uint32_t height, uint8_t const* blockStorage, uint32_t *image);

/*
  Decompresses one block of a DXT5 texture and stores the resulting pixels at the appropriate offset in 'image'.

  @param x:                     x-coordinate of the first pixel in the block.
  @param y:                     y-coordinate of the first pixel in the block.
  @param width:                 width of the texture being decompressed.
  @param height:                height of the texture being decompressed.
  @param blockStorage:          pointer to the block to decompress.
  @param image:                 pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockDXT5(uint32_t x, uint32_t y, uint32_t width, uint8_t const* blockStorage, uint32_t *image);

/*
  Decompresses all the blocks of a DXT5 compressed texture and stores the resulting pixels in 'image'.

  @param width:                 Texture width.
  @param height:                Texture height.
  @param blockStorage:          pointer to compressed DXT5 blocks.
  @param image:                 pointer to the image where the decompressed pixels will be stored.
*/
void DecompressDXT5(uint32_t width, uint32_t height, uint8_t const* blockStorage, uint32_t *image);

// ----------------------------------------------------------------------------

#ifdef S3TC_IMPLEMENTATION

uint32_t PackRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return ((r << 0) | (g << 8) | (b << 16) | (a << 24));
}

void DecompressBlockDXT1(uint32_t x, uint32_t y, uint32_t width, uint8_t const* blockStorage, uint32_t *image) {
  uint16_t const color0 = *reinterpret_cast<uint16_t const *>(blockStorage);
  uint16_t const color1 = *reinterpret_cast<uint16_t const *>(blockStorage + 2);

  uint32_t temp;

  // Color0
  temp = (color0 >> 11) * 255 + 16;
  uint8_t const r0 = (uint8_t)((temp/32 + temp)/32);
  
  temp = ((color0 & 0x07E0) >> 5) * 255 + 32;
  uint8_t const g0 = (uint8_t)((temp/64 + temp)/64);
  
  temp = (color0 & 0x001F) * 255 + 16;
  uint8_t const b0 = (uint8_t)((temp/32 + temp)/32);

  // Color1
  temp = (color1 >> 11) * 255 + 16;
  uint8_t const r1 = (uint8_t)((temp/32 + temp)/32);
  
  temp = ((color1 & 0x07E0) >> 5) * 255 + 32;
  uint8_t const g1 = (uint8_t)((temp/64 + temp)/64);
  
  temp = (color1 & 0x001F) * 255 + 16;
  uint8_t const b1 = (uint8_t)((temp/32 + temp)/32);

  uint32_t const code = *reinterpret_cast<uint32_t const*>(blockStorage + 4);

  for (int j = 0; j < 4; ++j)
  {
    for (int i = 0; i < 4; ++i)
    {
      uint8_t const positionCode = (code >> 2 * (4 * j + i)) & 0x03;
      uint32_t finalColor = 0;

      if (color0 > color1)
      {
        switch (positionCode)
        {
          case 0:
           finalColor = PackRGBA(r0, g0, b0, 255);
          break;
          case 1:
            finalColor = PackRGBA(r1, g1, b1, 255);
          break;
          case 2:
            finalColor = PackRGBA((2*r0+r1)/3, (2*g0+g1)/3, (2*b0+b1)/3, 255);
          break;
          case 3:
            finalColor = PackRGBA((r0+2*r1)/3, (g0+2*g1)/3, (b0+2*b1)/3, 255);
          break;
        }
      }
      else
      {
        switch (positionCode)
        {
          case 0:
            finalColor = PackRGBA(r0, g0, b0, 255);
          break;
          case 1:
            finalColor = PackRGBA(r1, g1, b1, 255);
          break;
          case 2:
            finalColor = PackRGBA((r0+r1)/2, (g0+g1)/2, (b0+b1)/2, 255);
          break;
          case 3:
            finalColor = PackRGBA(0, 0, 0, 0);
          break;
        }
      }

      if (x + i < width) {
        image[(y + j)*width + (x + i)] = finalColor;
      }
    }
  }
}

void DecompressDXT1(uint32_t width, uint32_t height, uint8_t const* blockStorage, uint32_t *image) {
  uint32_t const blockWidth = (width < 4) ? width : 4;
  uint32_t const blockHeight = (height < 4) ? height : 4;
  
  uint32_t const blockCountX = (width + blockWidth - 1) / blockWidth;
  uint32_t const blockCountY = (height + blockHeight - 1 ) / blockHeight;
  
  uint32_t constexpr blockSize = 8;  //= 4 * sizeof(uint8_t) + sizeof(uint32_t);

  uint32_t blockOffset = 0;
  for (uint32_t j = 0; j < blockCountY; ++j) {
    for (uint32_t i = 0; i < blockCountX; ++i) {
      DecompressBlockDXT1(i * blockWidth, j * blockHeight, width, blockStorage + blockOffset, image);
      blockOffset += blockSize;
    }
  }
}

void DecompressBlockDXT5(uint32_t x, uint32_t y, uint32_t width, uint8_t const* blockStorage, uint32_t *image) {
  uint8_t const alpha0 = *reinterpret_cast<uint8_t const*>(blockStorage);
  uint8_t const alpha1 = *reinterpret_cast<uint8_t const*>(blockStorage + 1);

  uint8_t const* bits = blockStorage + 2;
  uint32_t const alphaCode1 = bits[2] | (bits[3] << 8) | (bits[4] << 16) | (bits[5] << 24);
  uint16_t const alphaCode2 = bits[0] | (bits[1] << 8);

  uint16_t const color0 = *reinterpret_cast<uint16_t const*>(blockStorage + 8);
  uint16_t const color1 = *reinterpret_cast<uint16_t const*>(blockStorage + 10);   

  uint32_t temp;

  // Color 0
  temp = (color0 >> 11) * 255 + 16;
  uint8_t r0 = (uint8_t)((temp/32 + temp)/32);

  temp = ((color0 & 0x07E0) >> 5) * 255 + 32;
  uint8_t g0 = (uint8_t)((temp/64 + temp)/64);
  
  temp = (color0 & 0x001F) * 255 + 16;
  uint8_t b0 = (uint8_t)((temp/32 + temp)/32);

  // Color 1
  temp = (color1 >> 11) * 255 + 16;
  uint8_t r1 = (uint8_t)((temp/32 + temp)/32);
  
  temp = ((color1 & 0x07E0) >> 5) * 255 + 32;
  uint8_t g1 = (uint8_t)((temp/64 + temp)/64);
  
  temp = (color1 & 0x001F) * 255 + 16;
  uint8_t b1 = (uint8_t)((temp/32 + temp)/32);

  uint32_t code = *reinterpret_cast<const uint32_t *>(blockStorage + 12);

  for (int j = 0; j < 4; ++j) 
  {
    for (int i = 0; i < 4; ++i) 
    {
      int const alphaCodeIndex = 3 * (4 * j + i);
      
      int alphaCode;
      if (alphaCodeIndex <= 12)
      {
        alphaCode = (alphaCode2 >> alphaCodeIndex) & 0x07;
      }
      else if (alphaCodeIndex == 15)
      {
        alphaCode = (alphaCode2 >> 15) | ((alphaCode1 << 1) & 0x06);
      }
      else // alphaCodeIndex >= 18 && alphaCodeIndex <= 45
      {
        alphaCode = (alphaCode1 >> (alphaCodeIndex - 16)) & 0x07;
      }

      uint8_t finalAlpha;
      if (alphaCode == 0)
      {
        finalAlpha = alpha0;
      }
      else if (alphaCode == 1)
      {
        finalAlpha = alpha1;
      }
      else
      {
        if (alpha0 > alpha1)
        {
          finalAlpha = ((8-alphaCode)*alpha0 + (alphaCode-1)*alpha1)/7;
        }
        else
        {
          if (alphaCode == 6) 
          {
            finalAlpha = 0;
          } 
          else if (alphaCode == 7) 
          {
            finalAlpha = 255;
          } 
          else 
          {
            finalAlpha = ((6-alphaCode)*alpha0 + (alphaCode-1)*alpha1)/5;
          }
        }
      }

      uint8_t const colorCode = (code >> 2*(4*j+i)) & 0x03;

      uint32_t finalColor;
      switch (colorCode)
      {
        case 0:
          finalColor = PackRGBA(r0, g0, b0, finalAlpha);
          break;
        case 1:
          finalColor = PackRGBA(r1, g1, b1, finalAlpha);
          break;
        case 2:
          finalColor = PackRGBA((2*r0+r1)/3, (2*g0+g1)/3, (2*b0+b1)/3, finalAlpha);
          break;
        case 3:
          finalColor = PackRGBA((r0+2*r1)/3, (g0+2*g1)/3, (b0+2*b1)/3, finalAlpha);
          break;
      }

      uint32_t const px = x + i;
      uint32_t const py = y + j;
      if (px < width) {
        image[py * width + px] = finalColor;
      }
    }
  }
}

void DecompressDXT5(uint32_t width, uint32_t height, uint8_t const* blockStorage, uint32_t *image)
{
  uint32_t const blockWidth = (width < 4) ? width : 4;
  uint32_t const blockHeight = (height < 4) ? height : 4;

  uint32_t const blockCountX = (width + blockWidth - 1) / blockWidth;
  uint32_t const blockCountY = (height + blockHeight - 1 ) / blockHeight;

  uint32_t constexpr blockSize = 16; 

  uint32_t blockOffset = 0;
  for (uint32_t j = 0; j < blockCountY; ++j) {
    for (uint32_t i = 0; i < blockCountX; ++i) {
      DecompressBlockDXT5(i * blockWidth, j * blockHeight, width, blockStorage + blockOffset, image);
      blockOffset += blockSize;
    }
  }
}

#endif // S3TC_IMPLEMENTATION

} // namespace s3tc

#endif // S3TC_H
