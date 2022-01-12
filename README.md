[![unlicense](https://img.shields.io/badge/Unlicense-%23373737)](https://unlicense.org/)
![language: c++17](https://img.shields.io/badge/c++-17-blue.svg)

# ⚒ Dojima

**Dojima** is a command line tool to convert data files from the game « _Way of the Samurai 4_ » from the NetImmerse / Gamebryo `NIF` format to `glTF 2.0`.

#### Features

- [x] mesh hierarchy (Characters part & pack)
- [ ] float16 buffers (Items, Maps & Weapons)
- [ ] materials
<!-- - [ ] bones -->
- [ ] skeletal animation
- [ ] facial animation
- [ ] extra properties

### Quick start

```bash
# Clone the project.
git clone https://github.com/tcoppex/dojima
cd dojima

# Create a build directory.
mkdir BUILD && cd BUILD

# Compile using Ninja as generator (optional).
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Run the cli converter.
#../bin/nif2gltf /path/to/wots4/Common/Character/Model/Pack/Dog01.nif
```

### Acknowledgment

Dojima is shipped with modified versions of its third parties in accordance to their
respective license. They are :

* [niflib](https://github.com/niftools/niflib) : library for nif IO. 
* [tinygltf](https://github.com/syoyo/tinygltf) : library for glTF 2.0 IO.
* [stb_image](https://github.com/nothings/stb) : image loading/decoding from file/memory.
* [json.hpp](https://github.com/nlohmann/json) : JSON for Modern C++.

`ACQUIRE`'s _Way of the Samurai 4_ can be acquired from steam [here](https://store.steampowered.com/app/312780/Way_of_the_Samurai_4/).

### Unlicense

_**Dojima**_ is released under *The Unlicense*.

<!-- 
## Notes

Using `niflib` as nif reader is probably overkill but might be changed later on.

## (Work in Progress)

Nif data block used by WotS4 (not completed) :
      + NiNode, 
      + NiSwitchNode,
      
      + NiIntegerExtraData, 
      + NiStringExtraData, 
      + NiFloatExtraData, 
      + NiColorExtraData, 
      
      + NiMaterialProperty, 
      + NiTexturingProperty, 
      + NiVertexColorProperty, 
      + NiShadeProperty, 
      + NiSpecularProperty, 
      + NiAlphaProperty, 
      + NiZBufferProperty, 
      
      + NiMesh, 
      
      + NiTransformController, 
      + NiSkinningLODController, *
      + NiSkinningMeshModifier,
      
      + NiSourceTexture, 
      + NiPixelData,

      + NiDataStream018, 
      + NiDataStream118, 
      + NiDataStream33, 
      
      + NiSequenceData, 
      + NiTransformEvaluator, 
      + NiTransformData, 
      + NiBSplineCompTransformEvaluator, 
      + NiBSplineData, 
      + NiBSplineBasisData, 
      + NiTextKeyExtraData, 


### Notes on WotS4 data layout

In Resources/Character, `Part` defined single nif mesh object whereas `Pack` defined
the whole model (minus animation data).

WOTS4 has 3 kind of datastream :
    * INDEX
    * ATTRIBUTES (Texcoord, Position, Normal, Binormal, Tangent, BlendIndice, BlendWeight)
    * BONE_PALETTE

#### Models Packs.

first block is a IntegerExtraData (supposedly an id).

Then on the root, each parts are described by 4 root block :
  
  1- a `StringExtraData` to the mesh part.

  2- An upper `Node` for internal structure.
   * Internally they have the following rendering properties:
    - StringExtraData (creation info)
    - ZBufferProperty
    - ShadeProperty (smooth or not)
    - VertexColorProperty
    - MaterialProperty
   
   * Then a node with internally two elements :
    - first contain the rig hierarchy (supposedly, to detail)
      + First node is a transformController (to move)
      + Second is the root node of the rig.
    - second is a SwitchNode for LOD (higher level lower quality).
  
  3- a `StringExtraData` to the part's texture name / path.

  4- a `PixelData` describing (containing ?) the part's texture data.

 -->