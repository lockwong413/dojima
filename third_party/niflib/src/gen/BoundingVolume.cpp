/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//---THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT---//

//To change this file, alter the niftools/docsys/gen_niflib.py Python script.

#include "nif/gen/BoundingVolume.h"
#include "nif/gen/SphereBV.h"
#include "nif/gen/BoxBV.h"
#include "nif/gen/CapsuleBV.h"
#include "nif/gen/HalfSpaceBV.h"
using namespace Niflib;

//Constructor
BoundingVolume::BoundingVolume() : collisionType((BoundVolumeType)0) {};

//Copy Constructor
BoundingVolume::BoundingVolume( const BoundingVolume & src ) {
	*this = src;
};

//Copy Operator
BoundingVolume & BoundingVolume::operator=( const BoundingVolume & src ) {
	this->collisionType = src.collisionType;
	this->sphere = src.sphere;
	this->box = src.box;
	this->capsule = src.capsule;
	this->halfspace = src.halfspace;
	return *this;
};

//Destructor
BoundingVolume::~BoundingVolume() {};

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
