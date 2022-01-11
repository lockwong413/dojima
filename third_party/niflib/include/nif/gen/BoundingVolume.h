/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//---THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT---//

//To change this file, alter the niftools/docsys/gen_niflib.py Python script.

#ifndef _BOUNDINGVOLUME_H_
#define _BOUNDINGVOLUME_H_

#include "nif/NIF_IO.h"

// Include structures
#include "nif/gen/SphereBV.h"
#include "nif/gen/BoxBV.h"
#include "nif/gen/CapsuleBV.h"
#include "nif/gen/HalfSpaceBV.h"
namespace Niflib {


/*!  */
struct BoundingVolume {
	/*! Default Constructor */
	NIFLIB_API BoundingVolume();
	/*! Default Destructor */
	NIFLIB_API ~BoundingVolume();
	/*! Copy Constructor */
	NIFLIB_API BoundingVolume( const BoundingVolume & src );
	/*! Copy Operator */
	NIFLIB_API BoundingVolume & operator=( const BoundingVolume & src );
	/*! Type of collision data. */
	BoundVolumeType collisionType;
	/*! Sphere */
	SphereBV sphere;
	/*! Box */
	BoxBV box;
	/*! Capsule */
	CapsuleBV capsule;
	/*! Half Space */
	HalfSpaceBV halfspace;
	//--BEGIN MISC CUSTOM CODE--//
	//--END CUSTOM CODE--//
};

}
#endif
