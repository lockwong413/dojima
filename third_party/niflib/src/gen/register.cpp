/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//---THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT---//

//To change this file, alter the niftools/docsys/gen_niflib.py Python script.

#include "nif/ObjectRegistry.h"
#include "nif/obj/NiObject.h"
#include "nif/obj/Ni3dsAlphaAnimator.h"
#include "nif/obj/Ni3dsAnimationNode.h"
#include "nif/obj/Ni3dsColorAnimator.h"
#include "nif/obj/Ni3dsMorphShape.h"
#include "nif/obj/Ni3dsParticleSystem.h"
#include "nif/obj/Ni3dsPathController.h"
#include "nif/obj/NiParticleModifier.h"
#include "nif/obj/NiPSysCollider.h"
#include "nif/obj/bhkRefObject.h"
#include "nif/obj/bhkSerializable.h"
#include "nif/obj/bhkWorldObject.h"
#include "nif/obj/bhkPhantom.h"
#include "nif/obj/bhkShapePhantom.h"
#include "nif/obj/bhkSimpleShapePhantom.h"
#include "nif/obj/bhkEntity.h"
#include "nif/obj/bhkRigidBody.h"
#include "nif/obj/bhkRigidBodyT.h"
#include "nif/obj/bhkConstraint.h"
#include "nif/obj/bhkLimitedHingeConstraint.h"
#include "nif/obj/bhkMalleableConstraint.h"
#include "nif/obj/bhkStiffSpringConstraint.h"
#include "nif/obj/bhkRagdollConstraint.h"
#include "nif/obj/bhkPrismaticConstraint.h"
#include "nif/obj/bhkHingeConstraint.h"
#include "nif/obj/bhkBallAndSocketConstraint.h"
#include "nif/obj/bhkBallSocketConstraintChain.h"
#include "nif/obj/bhkShape.h"
#include "nif/obj/bhkTransformShape.h"
#include "nif/obj/bhkSphereRepShape.h"
#include "nif/obj/bhkConvexShape.h"
#include "nif/obj/bhkSphereShape.h"
#include "nif/obj/bhkCapsuleShape.h"
#include "nif/obj/bhkBoxShape.h"
#include "nif/obj/bhkConvexVerticesShape.h"
#include "nif/obj/bhkConvexTransformShape.h"
#include "nif/obj/bhkMultiSphereShape.h"
#include "nif/obj/bhkBvTreeShape.h"
#include "nif/obj/bhkMoppBvTreeShape.h"
#include "nif/obj/bhkShapeCollection.h"
#include "nif/obj/bhkListShape.h"
#include "nif/obj/bhkMeshShape.h"
#include "nif/obj/bhkPackedNiTriStripsShape.h"
#include "nif/obj/bhkNiTriStripsShape.h"
#include "nif/obj/NiExtraData.h"
#include "nif/obj/NiInterpolator.h"
#include "nif/obj/NiKeyBasedInterpolator.h"
#include "nif/obj/NiFloatInterpolator.h"
#include "nif/obj/NiTransformInterpolator.h"
#include "nif/obj/NiPoint3Interpolator.h"
#include "nif/obj/NiPathInterpolator.h"
#include "nif/obj/NiBoolInterpolator.h"
#include "nif/obj/NiBoolTimelineInterpolator.h"
#include "nif/obj/NiBlendInterpolator.h"
#include "nif/obj/NiBSplineInterpolator.h"
#include "nif/obj/NiObjectNET.h"
#include "nif/obj/NiCollisionObject.h"
#include "nif/obj/NiCollisionData.h"
#include "nif/obj/bhkNiCollisionObject.h"
#include "nif/obj/bhkCollisionObject.h"
#include "nif/obj/bhkBlendCollisionObject.h"
#include "nif/obj/bhkPCollisionObject.h"
#include "nif/obj/bhkSPCollisionObject.h"
#include "nif/obj/NiAVObject.h"
#include "nif/obj/NiDynamicEffect.h"
#include "nif/obj/NiLight.h"
#include "nif/obj/NiProperty.h"
#include "nif/obj/NiTransparentProperty.h"
#include "nif/obj/NiPSysModifier.h"
#include "nif/obj/NiPSysEmitter.h"
#include "nif/obj/NiPSysVolumeEmitter.h"
#include "nif/obj/NiTimeController.h"
#include "nif/obj/NiInterpController.h"
#include "nif/obj/NiMultiTargetTransformController.h"
#include "nif/obj/NiGeomMorpherController.h"
#include "nif/obj/NiMorphController.h"
#include "nif/obj/NiMorpherController.h"
#include "nif/obj/NiSingleInterpController.h"
#include "nif/obj/NiKeyframeController.h"
#include "nif/obj/NiTransformController.h"
#include "nif/obj/NiPSysModifierCtlr.h"
#include "nif/obj/NiPSysEmitterCtlr.h"
#include "nif/obj/NiPSysModifierBoolCtlr.h"
#include "nif/obj/NiPSysModifierActiveCtlr.h"
#include "nif/obj/NiPSysModifierFloatCtlr.h"
#include "nif/obj/NiPSysEmitterDeclinationCtlr.h"
#include "nif/obj/NiPSysEmitterDeclinationVarCtlr.h"
#include "nif/obj/NiPSysEmitterInitialRadiusCtlr.h"
#include "nif/obj/NiPSysEmitterLifeSpanCtlr.h"
#include "nif/obj/NiPSysEmitterSpeedCtlr.h"
#include "nif/obj/NiPSysGravityStrengthCtlr.h"
#include "nif/obj/NiFloatInterpController.h"
#include "nif/obj/NiFlipController.h"
#include "nif/obj/NiAlphaController.h"
#include "nif/obj/NiTextureTransformController.h"
#include "nif/obj/NiLightDimmerController.h"
#include "nif/obj/NiBoolInterpController.h"
#include "nif/obj/NiVisController.h"
#include "nif/obj/NiPoint3InterpController.h"
#include "nif/obj/NiMaterialColorController.h"
#include "nif/obj/NiLightColorController.h"
#include "nif/obj/NiExtraDataController.h"
#include "nif/obj/NiFloatExtraDataController.h"
#include "nif/obj/NiBoneLODController.h"
#include "nif/obj/NiBSBoneLODController.h"
#include "nif/obj/NiGeometry.h"
#include "nif/obj/NiTriBasedGeom.h"
#include "nif/obj/NiGeometryData.h"
#include "nif/obj/AbstractAdditionalGeometryData.h"
#include "nif/obj/NiTriBasedGeomData.h"
#include "nif/obj/bhkBlendController.h"
#include "nif/obj/BSBound.h"
#include "nif/obj/BSFurnitureMarker.h"
#include "nif/obj/BSParentVelocityModifier.h"
#include "nif/obj/BSPSysArrayEmitter.h"
#include "nif/obj/BSWindModifier.h"
#include "nif/obj/hkPackedNiTriStripsData.h"
#include "nif/obj/NiAlphaProperty.h"
#include "nif/obj/NiAmbientLight.h"
#include "nif/obj/NiParticlesData.h"
#include "nif/obj/NiRotatingParticlesData.h"
#include "nif/obj/NiAutoNormalParticlesData.h"
#include "nif/obj/NiPSysData.h"
#include "nif/obj/NiMeshPSysData.h"
#include "nif/obj/NiBinaryExtraData.h"
#include "nif/obj/NiBinaryVoxelExtraData.h"
#include "nif/obj/NiBinaryVoxelData.h"
#include "nif/obj/NiBlendBoolInterpolator.h"
#include "nif/obj/NiBlendFloatInterpolator.h"
#include "nif/obj/NiBlendPoint3Interpolator.h"
#include "nif/obj/NiBlendTransformInterpolator.h"
#include "nif/obj/NiBoolData.h"
#include "nif/obj/NiBooleanExtraData.h"
#include "nif/obj/NiBSplineBasisData.h"
#include "nif/obj/NiBSplineFloatInterpolator.h"
#include "nif/obj/NiBSplineCompFloatInterpolator.h"
#include "nif/obj/NiBSplinePoint3Interpolator.h"
#include "nif/obj/NiBSplineCompPoint3Interpolator.h"
#include "nif/obj/NiBSplineTransformInterpolator.h"
#include "nif/obj/NiBSplineCompTransformInterpolator.h"
#include "nif/obj/BSRotAccumTransfInterpolator.h"
#include "nif/obj/NiBSplineData.h"
#include "nif/obj/NiCamera.h"
#include "nif/obj/NiColorData.h"
#include "nif/obj/NiColorExtraData.h"
#include "nif/obj/NiControllerManager.h"
#include "nif/obj/NiSequence.h"
#include "nif/obj/NiControllerSequence.h"
#include "nif/obj/NiAVObjectPalette.h"
#include "nif/obj/NiDefaultAVObjectPalette.h"
#include "nif/obj/NiDirectionalLight.h"
#include "nif/obj/NiDitherProperty.h"
#include "nif/obj/NiRollController.h"
#include "nif/obj/NiFloatData.h"
#include "nif/obj/NiFloatExtraData.h"
#include "nif/obj/NiFloatsExtraData.h"
#include "nif/obj/NiFogProperty.h"
#include "nif/obj/NiGravity.h"
#include "nif/obj/NiIntegerExtraData.h"
#include "nif/obj/BSXFlags.h"
#include "nif/obj/NiIntegersExtraData.h"
#include "nif/obj/BSKeyframeController.h"
#include "nif/obj/NiKeyframeData.h"
#include "nif/obj/NiLookAtController.h"
#include "nif/obj/NiLookAtInterpolator.h"
#include "nif/obj/NiMaterialProperty.h"
#include "nif/obj/NiMorphData.h"
#include "nif/obj/NiNode.h"
#include "nif/obj/NiBone.h"
#include "nif/obj/AvoidNode.h"
#include "nif/obj/FxWidget.h"
#include "nif/obj/FxButton.h"
#include "nif/obj/FxRadioButton.h"
#include "nif/obj/NiBillboardNode.h"
#include "nif/obj/NiBSAnimationNode.h"
#include "nif/obj/NiBSParticleNode.h"
#include "nif/obj/NiSwitchNode.h"
#include "nif/obj/NiLODNode.h"
#include "nif/obj/NiPalette.h"
#include "nif/obj/NiParticleBomb.h"
#include "nif/obj/NiParticleColorModifier.h"
#include "nif/obj/NiParticleGrowFade.h"
#include "nif/obj/NiParticleMeshModifier.h"
#include "nif/obj/NiParticleRotation.h"
#include "nif/obj/NiParticles.h"
#include "nif/obj/NiAutoNormalParticles.h"
#include "nif/obj/NiParticleMeshes.h"
#include "nif/obj/NiParticleMeshesData.h"
#include "nif/obj/NiParticleSystem.h"
#include "nif/obj/NiMeshParticleSystem.h"
#include "nif/obj/NiParticleSystemController.h"
#include "nif/obj/NiBSPArrayController.h"
#include "nif/obj/NiPathController.h"
#include "nif/obj/ATextureRenderData.h"
#include "nif/obj/NiPersistentSrcTextureRendererData.h"
#include "nif/obj/NiPixelData.h"
#include "nif/obj/NiPlanarCollider.h"
#include "nif/obj/NiPointLight.h"
#include "nif/obj/NiPosData.h"
#include "nif/obj/NiPSysAgeDeathModifier.h"
#include "nif/obj/NiPSysBombModifier.h"
#include "nif/obj/NiPSysBoundUpdateModifier.h"
#include "nif/obj/NiPSysBoxEmitter.h"
#include "nif/obj/NiPSysColliderManager.h"
#include "nif/obj/NiPSysColorModifier.h"
#include "nif/obj/NiPSysCylinderEmitter.h"
#include "nif/obj/NiPSysDragModifier.h"
#include "nif/obj/NiPSysEmitterCtlrData.h"
#include "nif/obj/NiPSysGravityModifier.h"
#include "nif/obj/NiPSysGrowFadeModifier.h"
#include "nif/obj/NiPSysMeshEmitter.h"
#include "nif/obj/NiPSysMeshUpdateModifier.h"
#include "nif/obj/BSPSysInheritVelocityModifier.h"
#include "nif/obj/BSPSysHavokUpdateModifier.h"
#include "nif/obj/BSPSysRecycleBoundModifier.h"
#include "nif/obj/BSPSysSubTexModifier.h"
#include "nif/obj/NiPSysPlanarCollider.h"
#include "nif/obj/NiPSysSphericalCollider.h"
#include "nif/obj/NiPSysPositionModifier.h"
#include "nif/obj/NiPSysResetOnLoopCtlr.h"
#include "nif/obj/NiPSysRotationModifier.h"
#include "nif/obj/NiPSysSpawnModifier.h"
#include "nif/obj/NiPSysSphereEmitter.h"
#include "nif/obj/NiPSysUpdateCtlr.h"
#include "nif/obj/NiPSysFieldModifier.h"
#include "nif/obj/NiPSysVortexFieldModifier.h"
#include "nif/obj/NiPSysGravityFieldModifier.h"
#include "nif/obj/NiPSysDragFieldModifier.h"
#include "nif/obj/NiPSysTurbulenceFieldModifier.h"
#include "nif/obj/BSPSysLODModifier.h"
#include "nif/obj/BSPSysScaleModifier.h"
#include "nif/obj/NiPSysFieldMagnitudeCtlr.h"
#include "nif/obj/NiPSysFieldAttenuationCtlr.h"
#include "nif/obj/NiPSysFieldMaxDistanceCtlr.h"
#include "nif/obj/NiPSysAirFieldAirFrictionCtlr.h"
#include "nif/obj/NiPSysAirFieldInheritVelocityCtlr.h"
#include "nif/obj/NiPSysAirFieldSpreadCtlr.h"
#include "nif/obj/NiPSysInitialRotSpeedCtlr.h"
#include "nif/obj/NiPSysInitialRotSpeedVarCtlr.h"
#include "nif/obj/NiPSysInitialRotAngleCtlr.h"
#include "nif/obj/NiPSysInitialRotAngleVarCtlr.h"
#include "nif/obj/NiPSysEmitterPlanarAngleCtlr.h"
#include "nif/obj/NiPSysEmitterPlanarAngleVarCtlr.h"
#include "nif/obj/NiPSysAirFieldModifier.h"
#include "nif/obj/NiPSysTrailEmitter.h"
#include "nif/obj/NiLightIntensityController.h"
#include "nif/obj/NiPSysRadialFieldModifier.h"
#include "nif/obj/NiLODData.h"
#include "nif/obj/NiRangeLODData.h"
#include "nif/obj/NiScreenLODData.h"
#include "nif/obj/NiRotatingParticles.h"
#include "nif/obj/NiSequenceStreamHelper.h"
#include "nif/obj/NiShadeProperty.h"
#include "nif/obj/NiSkinData.h"
#include "nif/obj/NiSkinInstance.h"
#include "nif/obj/NiTriShapeSkinController.h"
#include "nif/obj/NiClodSkinInstance.h"
#include "nif/obj/NiSkinPartition.h"
#include "nif/obj/NiTexture.h"
#include "nif/obj/NiSourceTexture.h"
#include "nif/obj/NiSpecularProperty.h"
#include "nif/obj/NiSphericalCollider.h"
#include "nif/obj/NiSpotLight.h"
#include "nif/obj/NiStencilProperty.h"
#include "nif/obj/NiStringExtraData.h"
#include "nif/obj/NiStringPalette.h"
#include "nif/obj/NiStringsExtraData.h"
#include "nif/obj/NiTextKeyExtraData.h"
#include "nif/obj/NiTextureEffect.h"
#include "nif/obj/NiTextureModeProperty.h"
#include "nif/obj/NiImage.h"
#include "nif/obj/NiTextureProperty.h"
#include "nif/obj/NiMultiTextureProperty.h"
#include "nif/obj/NiTexturingProperty.h"
#include "nif/obj/NiTransformData.h"
#include "nif/obj/NiTriShape.h"
#include "nif/obj/NiTriShapeData.h"
#include "nif/obj/NiTriStrips.h"
#include "nif/obj/NiTriStripsData.h"
#include "nif/obj/NiEnvMappedTriShape.h"
#include "nif/obj/NiEnvMappedTriShapeData.h"
#include "nif/obj/NiBezierTriangle4.h"
#include "nif/obj/NiBezierMesh.h"
#include "nif/obj/NiClod.h"
#include "nif/obj/NiClodData.h"
#include "nif/obj/NiUVController.h"
#include "nif/obj/NiUVData.h"
#include "nif/obj/NiVectorExtraData.h"
#include "nif/obj/NiVertexColorProperty.h"
#include "nif/obj/NiVertWeightsExtraData.h"
#include "nif/obj/NiVisData.h"
#include "nif/obj/NiWireframeProperty.h"
#include "nif/obj/NiZBufferProperty.h"
#include "nif/obj/RootCollisionNode.h"
#include "nif/obj/NiRawImageData.h"
#include "nif/obj/NiSortAdjustNode.h"
#include "nif/obj/NiSourceCubeMap.h"
#include "nif/obj/NiPhysXProp.h"
#include "nif/obj/NiPhysXPropDesc.h"
#include "nif/obj/NiPhysXActorDesc.h"
#include "nif/obj/NiPhysXBodyDesc.h"
#include "nif/obj/NiPhysXD6JointDesc.h"
#include "nif/obj/NiPhysXShapeDesc.h"
#include "nif/obj/NiPhysXMeshDesc.h"
#include "nif/obj/NiPhysXMaterialDesc.h"
#include "nif/obj/NiPhysXKinematicSrc.h"
#include "nif/obj/NiPhysXTransformDest.h"
#include "nif/obj/NiArkAnimationExtraData.h"
#include "nif/obj/NiArkImporterExtraData.h"
#include "nif/obj/NiArkTextureExtraData.h"
#include "nif/obj/NiArkViewportInfoExtraData.h"
#include "nif/obj/NiArkShaderExtraData.h"
#include "nif/obj/NiLines.h"
#include "nif/obj/NiLinesData.h"
#include "nif/obj/NiScreenElementsData.h"
#include "nif/obj/NiScreenElements.h"
#include "nif/obj/NiRoomGroup.h"
#include "nif/obj/NiRoom.h"
#include "nif/obj/NiPortal.h"
#include "nif/obj/BSFadeNode.h"
#include "nif/obj/BSShaderProperty.h"
#include "nif/obj/BSShaderLightingProperty.h"
#include "nif/obj/BSShaderNoLightingProperty.h"
#include "nif/obj/BSShaderPPLightingProperty.h"
#include "nif/obj/BSEffectShaderPropertyFloatController.h"
#include "nif/obj/BSEffectShaderPropertyColorController.h"
#include "nif/obj/BSLightingShaderPropertyFloatController.h"
#include "nif/obj/BSLightingShaderPropertyColorController.h"
#include "nif/obj/BSNiAlphaPropertyTestRefController.h"
#include "nif/obj/BSProceduralLightningController.h"
#include "nif/obj/BSShaderTextureSet.h"
#include "nif/obj/WaterShaderProperty.h"
#include "nif/obj/SkyShaderProperty.h"
#include "nif/obj/TileShaderProperty.h"
#include "nif/obj/DistantLODShaderProperty.h"
#include "nif/obj/BSDistantTreeShaderProperty.h"
#include "nif/obj/TallGrassShaderProperty.h"
#include "nif/obj/VolumetricFogShaderProperty.h"
#include "nif/obj/HairShaderProperty.h"
#include "nif/obj/Lighting30ShaderProperty.h"
#include "nif/obj/BSLightingShaderProperty.h"
#include "nif/obj/BSEffectShaderProperty.h"
#include "nif/obj/BSWaterShaderProperty.h"
#include "nif/obj/BSSkyShaderProperty.h"
#include "nif/obj/BSDismemberSkinInstance.h"
#include "nif/obj/BSDecalPlacementVectorExtraData.h"
#include "nif/obj/BSPSysSimpleColorModifier.h"
#include "nif/obj/BSValueNode.h"
#include "nif/obj/BSStripParticleSystem.h"
#include "nif/obj/BSStripPSysData.h"
#include "nif/obj/BSPSysStripUpdateModifier.h"
#include "nif/obj/BSMaterialEmittanceMultController.h"
#include "nif/obj/BSMasterParticleSystem.h"
#include "nif/obj/BSPSysMultiTargetEmitterCtlr.h"
#include "nif/obj/BSRefractionStrengthController.h"
#include "nif/obj/BSOrderedNode.h"
#include "nif/obj/BSBlastNode.h"
#include "nif/obj/BSDamageStage.h"
#include "nif/obj/BSRefractionFirePeriodController.h"
#include "nif/obj/bhkConvexListShape.h"
#include "nif/obj/BSTreadTransfInterpolator.h"
#include "nif/obj/BSAnimNotes.h"
#include "nif/obj/bhkLiquidAction.h"
#include "nif/obj/BSMultiBoundNode.h"
#include "nif/obj/BSMultiBound.h"
#include "nif/obj/BSMultiBoundData.h"
#include "nif/obj/BSMultiBoundOBB.h"
#include "nif/obj/BSMultiBoundSphere.h"
#include "nif/obj/BSSegmentedTriShape.h"
#include "nif/obj/BSMultiBoundAABB.h"
#include "nif/obj/NiAdditionalGeometryData.h"
#include "nif/obj/BSPackedAdditionalGeometryData.h"
#include "nif/obj/BSWArray.h"
#include "nif/obj/bhkAabbPhantom.h"
#include "nif/obj/BSFrustumFOVController.h"
#include "nif/obj/BSDebrisNode.h"
#include "nif/obj/bhkBreakableConstraint.h"
#include "nif/obj/bhkOrientHingedBodyAction.h"
#include "nif/obj/NiDataStream.h"
#include "nif/obj/NiRenderObject.h"
#include "nif/obj/NiMeshModifier.h"
#include "nif/obj/NiMesh.h"
#include "nif/obj/NiMorphWeightsController.h"
#include "nif/obj/NiMorphMeshModifier.h"
#include "nif/obj/NiSkinningMeshModifier.h"
#include "nif/obj/NiInstancingMeshModifier.h"
#include "nif/obj/NiSkinningLODController.h"
#include "nif/obj/NiPSParticleSystem.h"
#include "nif/obj/NiPSMeshParticleSystem.h"
#include "nif/obj/NiPSEmitParticlesCtlr.h"
#include "nif/obj/NiPSForceActiveCtlr.h"
#include "nif/obj/NiPSSimulator.h"
#include "nif/obj/NiPSSimulatorStep.h"
#include "nif/obj/NiPSSimulatorGeneralStep.h"
#include "nif/obj/NiPSSimulatorForcesStep.h"
#include "nif/obj/NiPSSimulatorCollidersStep.h"
#include "nif/obj/NiPSSimulatorMeshAlignStep.h"
#include "nif/obj/NiPSSimulatorFinalStep.h"
#include "nif/obj/NiPSFacingQuadGenerator.h"
#include "nif/obj/NiShadowGenerator.h"
#include "nif/obj/NiPSBoundUpdater.h"
#include "nif/obj/NiPSDragForce.h"
#include "nif/obj/NiPSGravityForce.h"
#include "nif/obj/NiPSBoxEmitter.h"
#include "nif/obj/NiPSMeshEmitter.h"
#include "nif/obj/NiPSGravityStrengthCtlr.h"
#include "nif/obj/NiPSPlanarCollider.h"
#include "nif/obj/NiPSEmitterSpeedCtlr.h"
#include "nif/obj/NiPSEmitterRadiusCtlr.h"
#include "nif/obj/NiPSResetOnLoopCtlr.h"
#include "nif/obj/NiPSSphereEmitter.h"
#include "nif/obj/NiPSCylinderEmitter.h"
#include "nif/obj/NiPSEmitterDeclinationCtlr.h"
#include "nif/obj/NiPSEmitterDeclinationVarCtlr.h"
#include "nif/obj/NiPSEmitterPlanarAngleCtlr.h"
#include "nif/obj/NiPSEmitterPlanarAngleVarCtlr.h"
#include "nif/obj/NiPSEmitterRotAngleCtlr.h"
#include "nif/obj/NiPSEmitterRotAngleVarCtlr.h"
#include "nif/obj/NiPSEmitterRotSpeedCtlr.h"
#include "nif/obj/NiPSEmitterRotSpeedVarCtlr.h"
#include "nif/obj/NiPSEmitterLifeSpanCtlr.h"
#include "nif/obj/NiPSBombForce.h"
#include "nif/obj/NiPSSphericalCollider.h"
#include "nif/obj/NiPSSpawner.h"
#include "nif/obj/NiSequenceData.h"
#include "nif/obj/NiTransformEvaluator.h"
#include "nif/obj/NiBSplineCompTransformEvaluator.h"
#include "nif/obj/NiMeshHWInstance.h"
#include "nif/obj/NiFurSpringController.h"
#include "nif/obj/CStreamableAssetData.h"
#include "nif/obj/bhkCompressedMeshShape.h"
#include "nif/obj/bhkCompressedMeshShapeData.h"
#include "nif/obj/BSInvMarker.h"
#include "nif/obj/BSBoneLODExtraData.h"
#include "nif/obj/BSBehaviorGraphExtraData.h"
#include "nif/obj/BSLagBoneController.h"
#include "nif/obj/BSLODTriShape.h"
#include "nif/obj/BSFurnitureMarkerNode.h"
#include "nif/obj/BSLeafAnimNode.h"
#include "nif/obj/BSTreeNode.h"

namespace Niflib {
	void RegisterObjects() {

		ObjectRegistry::RegisterObject( "NiObject", NiObject::Create );
		ObjectRegistry::RegisterObject( "Ni3dsAlphaAnimator", Ni3dsAlphaAnimator::Create );
		ObjectRegistry::RegisterObject( "Ni3dsAnimationNode", Ni3dsAnimationNode::Create );
		ObjectRegistry::RegisterObject( "Ni3dsColorAnimator", Ni3dsColorAnimator::Create );
		ObjectRegistry::RegisterObject( "Ni3dsMorphShape", Ni3dsMorphShape::Create );
		ObjectRegistry::RegisterObject( "Ni3dsParticleSystem", Ni3dsParticleSystem::Create );
		ObjectRegistry::RegisterObject( "Ni3dsPathController", Ni3dsPathController::Create );
		ObjectRegistry::RegisterObject( "NiParticleModifier", NiParticleModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysCollider", NiPSysCollider::Create );
		ObjectRegistry::RegisterObject( "bhkRefObject", bhkRefObject::Create );
		ObjectRegistry::RegisterObject( "bhkSerializable", bhkSerializable::Create );
		ObjectRegistry::RegisterObject( "bhkWorldObject", bhkWorldObject::Create );
		ObjectRegistry::RegisterObject( "bhkPhantom", bhkPhantom::Create );
		ObjectRegistry::RegisterObject( "bhkShapePhantom", bhkShapePhantom::Create );
		ObjectRegistry::RegisterObject( "bhkSimpleShapePhantom", bhkSimpleShapePhantom::Create );
		ObjectRegistry::RegisterObject( "bhkEntity", bhkEntity::Create );
		ObjectRegistry::RegisterObject( "bhkRigidBody", bhkRigidBody::Create );
		ObjectRegistry::RegisterObject( "bhkRigidBodyT", bhkRigidBodyT::Create );
		ObjectRegistry::RegisterObject( "bhkConstraint", bhkConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkLimitedHingeConstraint", bhkLimitedHingeConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkMalleableConstraint", bhkMalleableConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkStiffSpringConstraint", bhkStiffSpringConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkRagdollConstraint", bhkRagdollConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkPrismaticConstraint", bhkPrismaticConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkHingeConstraint", bhkHingeConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkBallAndSocketConstraint", bhkBallAndSocketConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkBallSocketConstraintChain", bhkBallSocketConstraintChain::Create );
		ObjectRegistry::RegisterObject( "bhkShape", bhkShape::Create );
		ObjectRegistry::RegisterObject( "bhkTransformShape", bhkTransformShape::Create );
		ObjectRegistry::RegisterObject( "bhkSphereRepShape", bhkSphereRepShape::Create );
		ObjectRegistry::RegisterObject( "bhkConvexShape", bhkConvexShape::Create );
		ObjectRegistry::RegisterObject( "bhkSphereShape", bhkSphereShape::Create );
		ObjectRegistry::RegisterObject( "bhkCapsuleShape", bhkCapsuleShape::Create );
		ObjectRegistry::RegisterObject( "bhkBoxShape", bhkBoxShape::Create );
		ObjectRegistry::RegisterObject( "bhkConvexVerticesShape", bhkConvexVerticesShape::Create );
		ObjectRegistry::RegisterObject( "bhkConvexTransformShape", bhkConvexTransformShape::Create );
		ObjectRegistry::RegisterObject( "bhkMultiSphereShape", bhkMultiSphereShape::Create );
		ObjectRegistry::RegisterObject( "bhkBvTreeShape", bhkBvTreeShape::Create );
		ObjectRegistry::RegisterObject( "bhkMoppBvTreeShape", bhkMoppBvTreeShape::Create );
		ObjectRegistry::RegisterObject( "bhkShapeCollection", bhkShapeCollection::Create );
		ObjectRegistry::RegisterObject( "bhkListShape", bhkListShape::Create );
		ObjectRegistry::RegisterObject( "bhkMeshShape", bhkMeshShape::Create );
		ObjectRegistry::RegisterObject( "bhkPackedNiTriStripsShape", bhkPackedNiTriStripsShape::Create );
		ObjectRegistry::RegisterObject( "bhkNiTriStripsShape", bhkNiTriStripsShape::Create );
		ObjectRegistry::RegisterObject( "NiExtraData", NiExtraData::Create );
		ObjectRegistry::RegisterObject( "NiInterpolator", NiInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiKeyBasedInterpolator", NiKeyBasedInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiFloatInterpolator", NiFloatInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiTransformInterpolator", NiTransformInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiPoint3Interpolator", NiPoint3Interpolator::Create );
		ObjectRegistry::RegisterObject( "NiPathInterpolator", NiPathInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBoolInterpolator", NiBoolInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBoolTimelineInterpolator", NiBoolTimelineInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBlendInterpolator", NiBlendInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineInterpolator", NiBSplineInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiObjectNET", NiObjectNET::Create );
		ObjectRegistry::RegisterObject( "NiCollisionObject", NiCollisionObject::Create );
		ObjectRegistry::RegisterObject( "NiCollisionData", NiCollisionData::Create );
		ObjectRegistry::RegisterObject( "bhkNiCollisionObject", bhkNiCollisionObject::Create );
		ObjectRegistry::RegisterObject( "bhkCollisionObject", bhkCollisionObject::Create );
		ObjectRegistry::RegisterObject( "bhkBlendCollisionObject", bhkBlendCollisionObject::Create );
		ObjectRegistry::RegisterObject( "bhkPCollisionObject", bhkPCollisionObject::Create );
		ObjectRegistry::RegisterObject( "bhkSPCollisionObject", bhkSPCollisionObject::Create );
		ObjectRegistry::RegisterObject( "NiAVObject", NiAVObject::Create );
		ObjectRegistry::RegisterObject( "NiDynamicEffect", NiDynamicEffect::Create );
		ObjectRegistry::RegisterObject( "NiLight", NiLight::Create );
		ObjectRegistry::RegisterObject( "NiProperty", NiProperty::Create );
		ObjectRegistry::RegisterObject( "NiTransparentProperty", NiTransparentProperty::Create );
		ObjectRegistry::RegisterObject( "NiPSysModifier", NiPSysModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitter", NiPSysEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSysVolumeEmitter", NiPSysVolumeEmitter::Create );
		ObjectRegistry::RegisterObject( "NiTimeController", NiTimeController::Create );
		ObjectRegistry::RegisterObject( "NiInterpController", NiInterpController::Create );
		ObjectRegistry::RegisterObject( "NiMultiTargetTransformController", NiMultiTargetTransformController::Create );
		ObjectRegistry::RegisterObject( "NiGeomMorpherController", NiGeomMorpherController::Create );
		ObjectRegistry::RegisterObject( "NiMorphController", NiMorphController::Create );
		ObjectRegistry::RegisterObject( "NiMorpherController", NiMorpherController::Create );
		ObjectRegistry::RegisterObject( "NiSingleInterpController", NiSingleInterpController::Create );
		ObjectRegistry::RegisterObject( "NiKeyframeController", NiKeyframeController::Create );
		ObjectRegistry::RegisterObject( "NiTransformController", NiTransformController::Create );
		ObjectRegistry::RegisterObject( "NiPSysModifierCtlr", NiPSysModifierCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterCtlr", NiPSysEmitterCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysModifierBoolCtlr", NiPSysModifierBoolCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysModifierActiveCtlr", NiPSysModifierActiveCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysModifierFloatCtlr", NiPSysModifierFloatCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterDeclinationCtlr", NiPSysEmitterDeclinationCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterDeclinationVarCtlr", NiPSysEmitterDeclinationVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterInitialRadiusCtlr", NiPSysEmitterInitialRadiusCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterLifeSpanCtlr", NiPSysEmitterLifeSpanCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterSpeedCtlr", NiPSysEmitterSpeedCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysGravityStrengthCtlr", NiPSysGravityStrengthCtlr::Create );
		ObjectRegistry::RegisterObject( "NiFloatInterpController", NiFloatInterpController::Create );
		ObjectRegistry::RegisterObject( "NiFlipController", NiFlipController::Create );
		ObjectRegistry::RegisterObject( "NiAlphaController", NiAlphaController::Create );
		ObjectRegistry::RegisterObject( "NiTextureTransformController", NiTextureTransformController::Create );
		ObjectRegistry::RegisterObject( "NiLightDimmerController", NiLightDimmerController::Create );
		ObjectRegistry::RegisterObject( "NiBoolInterpController", NiBoolInterpController::Create );
		ObjectRegistry::RegisterObject( "NiVisController", NiVisController::Create );
		ObjectRegistry::RegisterObject( "NiPoint3InterpController", NiPoint3InterpController::Create );
		ObjectRegistry::RegisterObject( "NiMaterialColorController", NiMaterialColorController::Create );
		ObjectRegistry::RegisterObject( "NiLightColorController", NiLightColorController::Create );
		ObjectRegistry::RegisterObject( "NiExtraDataController", NiExtraDataController::Create );
		ObjectRegistry::RegisterObject( "NiFloatExtraDataController", NiFloatExtraDataController::Create );
		ObjectRegistry::RegisterObject( "NiBoneLODController", NiBoneLODController::Create );
		ObjectRegistry::RegisterObject( "NiBSBoneLODController", NiBSBoneLODController::Create );
		ObjectRegistry::RegisterObject( "NiGeometry", NiGeometry::Create );
		ObjectRegistry::RegisterObject( "NiTriBasedGeom", NiTriBasedGeom::Create );
		ObjectRegistry::RegisterObject( "NiGeometryData", NiGeometryData::Create );
		ObjectRegistry::RegisterObject( "AbstractAdditionalGeometryData", AbstractAdditionalGeometryData::Create );
		ObjectRegistry::RegisterObject( "NiTriBasedGeomData", NiTriBasedGeomData::Create );
		ObjectRegistry::RegisterObject( "bhkBlendController", bhkBlendController::Create );
		ObjectRegistry::RegisterObject( "BSBound", BSBound::Create );
		ObjectRegistry::RegisterObject( "BSFurnitureMarker", BSFurnitureMarker::Create );
		ObjectRegistry::RegisterObject( "BSParentVelocityModifier", BSParentVelocityModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysArrayEmitter", BSPSysArrayEmitter::Create );
		ObjectRegistry::RegisterObject( "BSWindModifier", BSWindModifier::Create );
		ObjectRegistry::RegisterObject( "hkPackedNiTriStripsData", hkPackedNiTriStripsData::Create );
		ObjectRegistry::RegisterObject( "NiAlphaProperty", NiAlphaProperty::Create );
		ObjectRegistry::RegisterObject( "NiAmbientLight", NiAmbientLight::Create );
		ObjectRegistry::RegisterObject( "NiParticlesData", NiParticlesData::Create );
		ObjectRegistry::RegisterObject( "NiRotatingParticlesData", NiRotatingParticlesData::Create );
		ObjectRegistry::RegisterObject( "NiAutoNormalParticlesData", NiAutoNormalParticlesData::Create );
		ObjectRegistry::RegisterObject( "NiPSysData", NiPSysData::Create );
		ObjectRegistry::RegisterObject( "NiMeshPSysData", NiMeshPSysData::Create );
		ObjectRegistry::RegisterObject( "NiBinaryExtraData", NiBinaryExtraData::Create );
		ObjectRegistry::RegisterObject( "NiBinaryVoxelExtraData", NiBinaryVoxelExtraData::Create );
		ObjectRegistry::RegisterObject( "NiBinaryVoxelData", NiBinaryVoxelData::Create );
		ObjectRegistry::RegisterObject( "NiBlendBoolInterpolator", NiBlendBoolInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBlendFloatInterpolator", NiBlendFloatInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBlendPoint3Interpolator", NiBlendPoint3Interpolator::Create );
		ObjectRegistry::RegisterObject( "NiBlendTransformInterpolator", NiBlendTransformInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBoolData", NiBoolData::Create );
		ObjectRegistry::RegisterObject( "NiBooleanExtraData", NiBooleanExtraData::Create );
		ObjectRegistry::RegisterObject( "NiBSplineBasisData", NiBSplineBasisData::Create );
		ObjectRegistry::RegisterObject( "NiBSplineFloatInterpolator", NiBSplineFloatInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineCompFloatInterpolator", NiBSplineCompFloatInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplinePoint3Interpolator", NiBSplinePoint3Interpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineCompPoint3Interpolator", NiBSplineCompPoint3Interpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineTransformInterpolator", NiBSplineTransformInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineCompTransformInterpolator", NiBSplineCompTransformInterpolator::Create );
		ObjectRegistry::RegisterObject( "BSRotAccumTransfInterpolator", BSRotAccumTransfInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineData", NiBSplineData::Create );
		ObjectRegistry::RegisterObject( "NiCamera", NiCamera::Create );
		ObjectRegistry::RegisterObject( "NiColorData", NiColorData::Create );
		ObjectRegistry::RegisterObject( "NiColorExtraData", NiColorExtraData::Create );
		ObjectRegistry::RegisterObject( "NiControllerManager", NiControllerManager::Create );
		ObjectRegistry::RegisterObject( "NiSequence", NiSequence::Create );
		ObjectRegistry::RegisterObject( "NiControllerSequence", NiControllerSequence::Create );
		ObjectRegistry::RegisterObject( "NiAVObjectPalette", NiAVObjectPalette::Create );
		ObjectRegistry::RegisterObject( "NiDefaultAVObjectPalette", NiDefaultAVObjectPalette::Create );
		ObjectRegistry::RegisterObject( "NiDirectionalLight", NiDirectionalLight::Create );
		ObjectRegistry::RegisterObject( "NiDitherProperty", NiDitherProperty::Create );
		ObjectRegistry::RegisterObject( "NiRollController", NiRollController::Create );
		ObjectRegistry::RegisterObject( "NiFloatData", NiFloatData::Create );
		ObjectRegistry::RegisterObject( "NiFloatExtraData", NiFloatExtraData::Create );
		ObjectRegistry::RegisterObject( "NiFloatsExtraData", NiFloatsExtraData::Create );
		ObjectRegistry::RegisterObject( "NiFogProperty", NiFogProperty::Create );
		ObjectRegistry::RegisterObject( "NiGravity", NiGravity::Create );
		ObjectRegistry::RegisterObject( "NiIntegerExtraData", NiIntegerExtraData::Create );
		ObjectRegistry::RegisterObject( "BSXFlags", BSXFlags::Create );
		ObjectRegistry::RegisterObject( "NiIntegersExtraData", NiIntegersExtraData::Create );
		ObjectRegistry::RegisterObject( "BSKeyframeController", BSKeyframeController::Create );
		ObjectRegistry::RegisterObject( "NiKeyframeData", NiKeyframeData::Create );
		ObjectRegistry::RegisterObject( "NiLookAtController", NiLookAtController::Create );
		ObjectRegistry::RegisterObject( "NiLookAtInterpolator", NiLookAtInterpolator::Create );
		ObjectRegistry::RegisterObject( "NiMaterialProperty", NiMaterialProperty::Create );
		ObjectRegistry::RegisterObject( "NiMorphData", NiMorphData::Create );
		ObjectRegistry::RegisterObject( "NiNode", NiNode::Create );
		ObjectRegistry::RegisterObject( "NiBone", NiBone::Create );
		ObjectRegistry::RegisterObject( "AvoidNode", AvoidNode::Create );
		ObjectRegistry::RegisterObject( "FxWidget", FxWidget::Create );
		ObjectRegistry::RegisterObject( "FxButton", FxButton::Create );
		ObjectRegistry::RegisterObject( "FxRadioButton", FxRadioButton::Create );
		ObjectRegistry::RegisterObject( "NiBillboardNode", NiBillboardNode::Create );
		ObjectRegistry::RegisterObject( "NiBSAnimationNode", NiBSAnimationNode::Create );
		ObjectRegistry::RegisterObject( "NiBSParticleNode", NiBSParticleNode::Create );
		ObjectRegistry::RegisterObject( "NiSwitchNode", NiSwitchNode::Create );
		ObjectRegistry::RegisterObject( "NiLODNode", NiLODNode::Create );
		ObjectRegistry::RegisterObject( "NiPalette", NiPalette::Create );
		ObjectRegistry::RegisterObject( "NiParticleBomb", NiParticleBomb::Create );
		ObjectRegistry::RegisterObject( "NiParticleColorModifier", NiParticleColorModifier::Create );
		ObjectRegistry::RegisterObject( "NiParticleGrowFade", NiParticleGrowFade::Create );
		ObjectRegistry::RegisterObject( "NiParticleMeshModifier", NiParticleMeshModifier::Create );
		ObjectRegistry::RegisterObject( "NiParticleRotation", NiParticleRotation::Create );
		ObjectRegistry::RegisterObject( "NiParticles", NiParticles::Create );
		ObjectRegistry::RegisterObject( "NiAutoNormalParticles", NiAutoNormalParticles::Create );
		ObjectRegistry::RegisterObject( "NiParticleMeshes", NiParticleMeshes::Create );
		ObjectRegistry::RegisterObject( "NiParticleMeshesData", NiParticleMeshesData::Create );
		ObjectRegistry::RegisterObject( "NiParticleSystem", NiParticleSystem::Create );
		ObjectRegistry::RegisterObject( "NiMeshParticleSystem", NiMeshParticleSystem::Create );
		ObjectRegistry::RegisterObject( "NiParticleSystemController", NiParticleSystemController::Create );
		ObjectRegistry::RegisterObject( "NiBSPArrayController", NiBSPArrayController::Create );
		ObjectRegistry::RegisterObject( "NiPathController", NiPathController::Create );
		ObjectRegistry::RegisterObject( "ATextureRenderData", ATextureRenderData::Create );
		ObjectRegistry::RegisterObject( "NiPersistentSrcTextureRendererData", NiPersistentSrcTextureRendererData::Create );
		ObjectRegistry::RegisterObject( "NiPixelData", NiPixelData::Create );
		ObjectRegistry::RegisterObject( "NiPlanarCollider", NiPlanarCollider::Create );
		ObjectRegistry::RegisterObject( "NiPointLight", NiPointLight::Create );
		ObjectRegistry::RegisterObject( "NiPosData", NiPosData::Create );
		ObjectRegistry::RegisterObject( "NiPSysAgeDeathModifier", NiPSysAgeDeathModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysBombModifier", NiPSysBombModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysBoundUpdateModifier", NiPSysBoundUpdateModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysBoxEmitter", NiPSysBoxEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSysColliderManager", NiPSysColliderManager::Create );
		ObjectRegistry::RegisterObject( "NiPSysColorModifier", NiPSysColorModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysCylinderEmitter", NiPSysCylinderEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSysDragModifier", NiPSysDragModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterCtlrData", NiPSysEmitterCtlrData::Create );
		ObjectRegistry::RegisterObject( "NiPSysGravityModifier", NiPSysGravityModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysGrowFadeModifier", NiPSysGrowFadeModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysMeshEmitter", NiPSysMeshEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSysMeshUpdateModifier", NiPSysMeshUpdateModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysInheritVelocityModifier", BSPSysInheritVelocityModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysHavokUpdateModifier", BSPSysHavokUpdateModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysRecycleBoundModifier", BSPSysRecycleBoundModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysSubTexModifier", BSPSysSubTexModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysPlanarCollider", NiPSysPlanarCollider::Create );
		ObjectRegistry::RegisterObject( "NiPSysSphericalCollider", NiPSysSphericalCollider::Create );
		ObjectRegistry::RegisterObject( "NiPSysPositionModifier", NiPSysPositionModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysResetOnLoopCtlr", NiPSysResetOnLoopCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysRotationModifier", NiPSysRotationModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysSpawnModifier", NiPSysSpawnModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysSphereEmitter", NiPSysSphereEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSysUpdateCtlr", NiPSysUpdateCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysFieldModifier", NiPSysFieldModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysVortexFieldModifier", NiPSysVortexFieldModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysGravityFieldModifier", NiPSysGravityFieldModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysDragFieldModifier", NiPSysDragFieldModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysTurbulenceFieldModifier", NiPSysTurbulenceFieldModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysLODModifier", BSPSysLODModifier::Create );
		ObjectRegistry::RegisterObject( "BSPSysScaleModifier", BSPSysScaleModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysFieldMagnitudeCtlr", NiPSysFieldMagnitudeCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysFieldAttenuationCtlr", NiPSysFieldAttenuationCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysFieldMaxDistanceCtlr", NiPSysFieldMaxDistanceCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysAirFieldAirFrictionCtlr", NiPSysAirFieldAirFrictionCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysAirFieldInheritVelocityCtlr", NiPSysAirFieldInheritVelocityCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysAirFieldSpreadCtlr", NiPSysAirFieldSpreadCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysInitialRotSpeedCtlr", NiPSysInitialRotSpeedCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysInitialRotSpeedVarCtlr", NiPSysInitialRotSpeedVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysInitialRotAngleCtlr", NiPSysInitialRotAngleCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysInitialRotAngleVarCtlr", NiPSysInitialRotAngleVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterPlanarAngleCtlr", NiPSysEmitterPlanarAngleCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysEmitterPlanarAngleVarCtlr", NiPSysEmitterPlanarAngleVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSysAirFieldModifier", NiPSysAirFieldModifier::Create );
		ObjectRegistry::RegisterObject( "NiPSysTrailEmitter", NiPSysTrailEmitter::Create );
		ObjectRegistry::RegisterObject( "NiLightIntensityController", NiLightIntensityController::Create );
		ObjectRegistry::RegisterObject( "NiPSysRadialFieldModifier", NiPSysRadialFieldModifier::Create );
		ObjectRegistry::RegisterObject( "NiLODData", NiLODData::Create );
		ObjectRegistry::RegisterObject( "NiRangeLODData", NiRangeLODData::Create );
		ObjectRegistry::RegisterObject( "NiScreenLODData", NiScreenLODData::Create );
		ObjectRegistry::RegisterObject( "NiRotatingParticles", NiRotatingParticles::Create );
		ObjectRegistry::RegisterObject( "NiSequenceStreamHelper", NiSequenceStreamHelper::Create );
		ObjectRegistry::RegisterObject( "NiShadeProperty", NiShadeProperty::Create );
		ObjectRegistry::RegisterObject( "NiSkinData", NiSkinData::Create );
		ObjectRegistry::RegisterObject( "NiSkinInstance", NiSkinInstance::Create );
		ObjectRegistry::RegisterObject( "NiTriShapeSkinController", NiTriShapeSkinController::Create );
		ObjectRegistry::RegisterObject( "NiClodSkinInstance", NiClodSkinInstance::Create );
		ObjectRegistry::RegisterObject( "NiSkinPartition", NiSkinPartition::Create );
		ObjectRegistry::RegisterObject( "NiTexture", NiTexture::Create );
		ObjectRegistry::RegisterObject( "NiSourceTexture", NiSourceTexture::Create );
		ObjectRegistry::RegisterObject( "NiSpecularProperty", NiSpecularProperty::Create );
		ObjectRegistry::RegisterObject( "NiSphericalCollider", NiSphericalCollider::Create );
		ObjectRegistry::RegisterObject( "NiSpotLight", NiSpotLight::Create );
		ObjectRegistry::RegisterObject( "NiStencilProperty", NiStencilProperty::Create );
		ObjectRegistry::RegisterObject( "NiStringExtraData", NiStringExtraData::Create );
		ObjectRegistry::RegisterObject( "NiStringPalette", NiStringPalette::Create );
		ObjectRegistry::RegisterObject( "NiStringsExtraData", NiStringsExtraData::Create );
		ObjectRegistry::RegisterObject( "NiTextKeyExtraData", NiTextKeyExtraData::Create );
		ObjectRegistry::RegisterObject( "NiTextureEffect", NiTextureEffect::Create );
		ObjectRegistry::RegisterObject( "NiTextureModeProperty", NiTextureModeProperty::Create );
		ObjectRegistry::RegisterObject( "NiImage", NiImage::Create );
		ObjectRegistry::RegisterObject( "NiTextureProperty", NiTextureProperty::Create );
		ObjectRegistry::RegisterObject( "NiMultiTextureProperty", NiMultiTextureProperty::Create );
		ObjectRegistry::RegisterObject( "NiTexturingProperty", NiTexturingProperty::Create );
		ObjectRegistry::RegisterObject( "NiTransformData", NiTransformData::Create );
		ObjectRegistry::RegisterObject( "NiTriShape", NiTriShape::Create );
		ObjectRegistry::RegisterObject( "NiTriShapeData", NiTriShapeData::Create );
		ObjectRegistry::RegisterObject( "NiTriStrips", NiTriStrips::Create );
		ObjectRegistry::RegisterObject( "NiTriStripsData", NiTriStripsData::Create );
		ObjectRegistry::RegisterObject( "NiEnvMappedTriShape", NiEnvMappedTriShape::Create );
		ObjectRegistry::RegisterObject( "NiEnvMappedTriShapeData", NiEnvMappedTriShapeData::Create );
		ObjectRegistry::RegisterObject( "NiBezierTriangle4", NiBezierTriangle4::Create );
		ObjectRegistry::RegisterObject( "NiBezierMesh", NiBezierMesh::Create );
		ObjectRegistry::RegisterObject( "NiClod", NiClod::Create );
		ObjectRegistry::RegisterObject( "NiClodData", NiClodData::Create );
		ObjectRegistry::RegisterObject( "NiUVController", NiUVController::Create );
		ObjectRegistry::RegisterObject( "NiUVData", NiUVData::Create );
		ObjectRegistry::RegisterObject( "NiVectorExtraData", NiVectorExtraData::Create );
		ObjectRegistry::RegisterObject( "NiVertexColorProperty", NiVertexColorProperty::Create );
		ObjectRegistry::RegisterObject( "NiVertWeightsExtraData", NiVertWeightsExtraData::Create );
		ObjectRegistry::RegisterObject( "NiVisData", NiVisData::Create );
		ObjectRegistry::RegisterObject( "NiWireframeProperty", NiWireframeProperty::Create );
		ObjectRegistry::RegisterObject( "NiZBufferProperty", NiZBufferProperty::Create );
		ObjectRegistry::RegisterObject( "RootCollisionNode", RootCollisionNode::Create );
		ObjectRegistry::RegisterObject( "NiRawImageData", NiRawImageData::Create );
		ObjectRegistry::RegisterObject( "NiSortAdjustNode", NiSortAdjustNode::Create );
		ObjectRegistry::RegisterObject( "NiSourceCubeMap", NiSourceCubeMap::Create );
		ObjectRegistry::RegisterObject( "NiPhysXProp", NiPhysXProp::Create );
		ObjectRegistry::RegisterObject( "NiPhysXPropDesc", NiPhysXPropDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXActorDesc", NiPhysXActorDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXBodyDesc", NiPhysXBodyDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXD6JointDesc", NiPhysXD6JointDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXShapeDesc", NiPhysXShapeDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXMeshDesc", NiPhysXMeshDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXMaterialDesc", NiPhysXMaterialDesc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXKinematicSrc", NiPhysXKinematicSrc::Create );
		ObjectRegistry::RegisterObject( "NiPhysXTransformDest", NiPhysXTransformDest::Create );
		ObjectRegistry::RegisterObject( "NiArkAnimationExtraData", NiArkAnimationExtraData::Create );
		ObjectRegistry::RegisterObject( "NiArkImporterExtraData", NiArkImporterExtraData::Create );
		ObjectRegistry::RegisterObject( "NiArkTextureExtraData", NiArkTextureExtraData::Create );
		ObjectRegistry::RegisterObject( "NiArkViewportInfoExtraData", NiArkViewportInfoExtraData::Create );
		ObjectRegistry::RegisterObject( "NiArkShaderExtraData", NiArkShaderExtraData::Create );
		ObjectRegistry::RegisterObject( "NiLines", NiLines::Create );
		ObjectRegistry::RegisterObject( "NiLinesData", NiLinesData::Create );
		ObjectRegistry::RegisterObject( "NiScreenElementsData", NiScreenElementsData::Create );
		ObjectRegistry::RegisterObject( "NiScreenElements", NiScreenElements::Create );
		ObjectRegistry::RegisterObject( "NiRoomGroup", NiRoomGroup::Create );
		ObjectRegistry::RegisterObject( "NiRoom", NiRoom::Create );
		ObjectRegistry::RegisterObject( "NiPortal", NiPortal::Create );
		ObjectRegistry::RegisterObject( "BSFadeNode", BSFadeNode::Create );
		ObjectRegistry::RegisterObject( "BSShaderProperty", BSShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSShaderLightingProperty", BSShaderLightingProperty::Create );
		ObjectRegistry::RegisterObject( "BSShaderNoLightingProperty", BSShaderNoLightingProperty::Create );
		ObjectRegistry::RegisterObject( "BSShaderPPLightingProperty", BSShaderPPLightingProperty::Create );
		ObjectRegistry::RegisterObject( "BSEffectShaderPropertyFloatController", BSEffectShaderPropertyFloatController::Create );
		ObjectRegistry::RegisterObject( "BSEffectShaderPropertyColorController", BSEffectShaderPropertyColorController::Create );
		ObjectRegistry::RegisterObject( "BSLightingShaderPropertyFloatController", BSLightingShaderPropertyFloatController::Create );
		ObjectRegistry::RegisterObject( "BSLightingShaderPropertyColorController", BSLightingShaderPropertyColorController::Create );
		ObjectRegistry::RegisterObject( "BSNiAlphaPropertyTestRefController", BSNiAlphaPropertyTestRefController::Create );
		ObjectRegistry::RegisterObject( "BSProceduralLightningController", BSProceduralLightningController::Create );
		ObjectRegistry::RegisterObject( "BSShaderTextureSet", BSShaderTextureSet::Create );
		ObjectRegistry::RegisterObject( "WaterShaderProperty", WaterShaderProperty::Create );
		ObjectRegistry::RegisterObject( "SkyShaderProperty", SkyShaderProperty::Create );
		ObjectRegistry::RegisterObject( "TileShaderProperty", TileShaderProperty::Create );
		ObjectRegistry::RegisterObject( "DistantLODShaderProperty", DistantLODShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSDistantTreeShaderProperty", BSDistantTreeShaderProperty::Create );
		ObjectRegistry::RegisterObject( "TallGrassShaderProperty", TallGrassShaderProperty::Create );
		ObjectRegistry::RegisterObject( "VolumetricFogShaderProperty", VolumetricFogShaderProperty::Create );
		ObjectRegistry::RegisterObject( "HairShaderProperty", HairShaderProperty::Create );
		ObjectRegistry::RegisterObject( "Lighting30ShaderProperty", Lighting30ShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSLightingShaderProperty", BSLightingShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSEffectShaderProperty", BSEffectShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSWaterShaderProperty", BSWaterShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSSkyShaderProperty", BSSkyShaderProperty::Create );
		ObjectRegistry::RegisterObject( "BSDismemberSkinInstance", BSDismemberSkinInstance::Create );
		ObjectRegistry::RegisterObject( "BSDecalPlacementVectorExtraData", BSDecalPlacementVectorExtraData::Create );
		ObjectRegistry::RegisterObject( "BSPSysSimpleColorModifier", BSPSysSimpleColorModifier::Create );
		ObjectRegistry::RegisterObject( "BSValueNode", BSValueNode::Create );
		ObjectRegistry::RegisterObject( "BSStripParticleSystem", BSStripParticleSystem::Create );
		ObjectRegistry::RegisterObject( "BSStripPSysData", BSStripPSysData::Create );
		ObjectRegistry::RegisterObject( "BSPSysStripUpdateModifier", BSPSysStripUpdateModifier::Create );
		ObjectRegistry::RegisterObject( "BSMaterialEmittanceMultController", BSMaterialEmittanceMultController::Create );
		ObjectRegistry::RegisterObject( "BSMasterParticleSystem", BSMasterParticleSystem::Create );
		ObjectRegistry::RegisterObject( "BSPSysMultiTargetEmitterCtlr", BSPSysMultiTargetEmitterCtlr::Create );
		ObjectRegistry::RegisterObject( "BSRefractionStrengthController", BSRefractionStrengthController::Create );
		ObjectRegistry::RegisterObject( "BSOrderedNode", BSOrderedNode::Create );
		ObjectRegistry::RegisterObject( "BSBlastNode", BSBlastNode::Create );
		ObjectRegistry::RegisterObject( "BSDamageStage", BSDamageStage::Create );
		ObjectRegistry::RegisterObject( "BSRefractionFirePeriodController", BSRefractionFirePeriodController::Create );
		ObjectRegistry::RegisterObject( "bhkConvexListShape", bhkConvexListShape::Create );
		ObjectRegistry::RegisterObject( "BSTreadTransfInterpolator", BSTreadTransfInterpolator::Create );
		ObjectRegistry::RegisterObject( "BSAnimNotes", BSAnimNotes::Create );
		ObjectRegistry::RegisterObject( "bhkLiquidAction", bhkLiquidAction::Create );
		ObjectRegistry::RegisterObject( "BSMultiBoundNode", BSMultiBoundNode::Create );
		ObjectRegistry::RegisterObject( "BSMultiBound", BSMultiBound::Create );
		ObjectRegistry::RegisterObject( "BSMultiBoundData", BSMultiBoundData::Create );
		ObjectRegistry::RegisterObject( "BSMultiBoundOBB", BSMultiBoundOBB::Create );
		ObjectRegistry::RegisterObject( "BSMultiBoundSphere", BSMultiBoundSphere::Create );
		ObjectRegistry::RegisterObject( "BSSegmentedTriShape", BSSegmentedTriShape::Create );
		ObjectRegistry::RegisterObject( "BSMultiBoundAABB", BSMultiBoundAABB::Create );
		ObjectRegistry::RegisterObject( "NiAdditionalGeometryData", NiAdditionalGeometryData::Create );
		ObjectRegistry::RegisterObject( "BSPackedAdditionalGeometryData", BSPackedAdditionalGeometryData::Create );
		ObjectRegistry::RegisterObject( "BSWArray", BSWArray::Create );
		ObjectRegistry::RegisterObject( "bhkAabbPhantom", bhkAabbPhantom::Create );
		ObjectRegistry::RegisterObject( "BSFrustumFOVController", BSFrustumFOVController::Create );
		ObjectRegistry::RegisterObject( "BSDebrisNode", BSDebrisNode::Create );
		ObjectRegistry::RegisterObject( "bhkBreakableConstraint", bhkBreakableConstraint::Create );
		ObjectRegistry::RegisterObject( "bhkOrientHingedBodyAction", bhkOrientHingedBodyAction::Create );
		ObjectRegistry::RegisterObject( "NiDataStream", NiDataStream::Create );
		ObjectRegistry::RegisterObject( "NiRenderObject", NiRenderObject::Create );
		ObjectRegistry::RegisterObject( "NiMeshModifier", NiMeshModifier::Create );
		ObjectRegistry::RegisterObject( "NiMesh", NiMesh::Create );
		ObjectRegistry::RegisterObject( "NiMorphWeightsController", NiMorphWeightsController::Create );
		ObjectRegistry::RegisterObject( "NiMorphMeshModifier", NiMorphMeshModifier::Create );
		ObjectRegistry::RegisterObject( "NiSkinningMeshModifier", NiSkinningMeshModifier::Create );
		ObjectRegistry::RegisterObject( "NiInstancingMeshModifier", NiInstancingMeshModifier::Create );
		ObjectRegistry::RegisterObject( "NiSkinningLODController", NiSkinningLODController::Create );
		ObjectRegistry::RegisterObject( "NiPSParticleSystem", NiPSParticleSystem::Create );
		ObjectRegistry::RegisterObject( "NiPSMeshParticleSystem", NiPSMeshParticleSystem::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitParticlesCtlr", NiPSEmitParticlesCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSForceActiveCtlr", NiPSForceActiveCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulator", NiPSSimulator::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulatorStep", NiPSSimulatorStep::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulatorGeneralStep", NiPSSimulatorGeneralStep::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulatorForcesStep", NiPSSimulatorForcesStep::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulatorCollidersStep", NiPSSimulatorCollidersStep::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulatorMeshAlignStep", NiPSSimulatorMeshAlignStep::Create );
		ObjectRegistry::RegisterObject( "NiPSSimulatorFinalStep", NiPSSimulatorFinalStep::Create );
		ObjectRegistry::RegisterObject( "NiPSFacingQuadGenerator", NiPSFacingQuadGenerator::Create );
		ObjectRegistry::RegisterObject( "NiShadowGenerator", NiShadowGenerator::Create );
		ObjectRegistry::RegisterObject( "NiPSBoundUpdater", NiPSBoundUpdater::Create );
		ObjectRegistry::RegisterObject( "NiPSDragForce", NiPSDragForce::Create );
		ObjectRegistry::RegisterObject( "NiPSGravityForce", NiPSGravityForce::Create );
		ObjectRegistry::RegisterObject( "NiPSBoxEmitter", NiPSBoxEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSMeshEmitter", NiPSMeshEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSGravityStrengthCtlr", NiPSGravityStrengthCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSPlanarCollider", NiPSPlanarCollider::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterSpeedCtlr", NiPSEmitterSpeedCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterRadiusCtlr", NiPSEmitterRadiusCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSResetOnLoopCtlr", NiPSResetOnLoopCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSSphereEmitter", NiPSSphereEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSCylinderEmitter", NiPSCylinderEmitter::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterDeclinationCtlr", NiPSEmitterDeclinationCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterDeclinationVarCtlr", NiPSEmitterDeclinationVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterPlanarAngleCtlr", NiPSEmitterPlanarAngleCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterPlanarAngleVarCtlr", NiPSEmitterPlanarAngleVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterRotAngleCtlr", NiPSEmitterRotAngleCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterRotAngleVarCtlr", NiPSEmitterRotAngleVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterRotSpeedCtlr", NiPSEmitterRotSpeedCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterRotSpeedVarCtlr", NiPSEmitterRotSpeedVarCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSEmitterLifeSpanCtlr", NiPSEmitterLifeSpanCtlr::Create );
		ObjectRegistry::RegisterObject( "NiPSBombForce", NiPSBombForce::Create );
		ObjectRegistry::RegisterObject( "NiPSSphericalCollider", NiPSSphericalCollider::Create );
		ObjectRegistry::RegisterObject( "NiPSSpawner", NiPSSpawner::Create );
		ObjectRegistry::RegisterObject( "NiSequenceData", NiSequenceData::Create );
		ObjectRegistry::RegisterObject( "NiTransformEvaluator", NiTransformEvaluator::Create );
		ObjectRegistry::RegisterObject( "NiBSplineCompTransformEvaluator", NiBSplineCompTransformEvaluator::Create );
		ObjectRegistry::RegisterObject( "NiMeshHWInstance", NiMeshHWInstance::Create );
		ObjectRegistry::RegisterObject( "NiFurSpringController", NiFurSpringController::Create );
		ObjectRegistry::RegisterObject( "CStreamableAssetData", CStreamableAssetData::Create );
		ObjectRegistry::RegisterObject( "bhkCompressedMeshShape", bhkCompressedMeshShape::Create );
		ObjectRegistry::RegisterObject( "bhkCompressedMeshShapeData", bhkCompressedMeshShapeData::Create );
		ObjectRegistry::RegisterObject( "BSInvMarker", BSInvMarker::Create );
		ObjectRegistry::RegisterObject( "BSBoneLODExtraData", BSBoneLODExtraData::Create );
		ObjectRegistry::RegisterObject( "BSBehaviorGraphExtraData", BSBehaviorGraphExtraData::Create );
		ObjectRegistry::RegisterObject( "BSLagBoneController", BSLagBoneController::Create );
		ObjectRegistry::RegisterObject( "BSLODTriShape", BSLODTriShape::Create );
		ObjectRegistry::RegisterObject( "BSFurnitureMarkerNode", BSFurnitureMarkerNode::Create );
		ObjectRegistry::RegisterObject( "BSLeafAnimNode", BSLeafAnimNode::Create );
		ObjectRegistry::RegisterObject( "BSTreeNode", BSTreeNode::Create );

	}
}
