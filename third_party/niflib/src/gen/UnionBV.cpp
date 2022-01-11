/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//---THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT---//

//To change this file, alter the niftools/docsys/gen_niflib.py Python script.

#include "nif/gen/UnionBV.h"
#include "nif/gen/BoundingVolume.h"
#include "nif/gen/SphereBV.h"
#include "nif/gen/BoxBV.h"
#include "nif/gen/CapsuleBV.h"
#include "nif/gen/HalfSpaceBV.h"
using namespace Niflib;

//Constructor
UnionBV::UnionBV() : numBv((unsigned int)0) {};

//Copy Constructor
UnionBV::UnionBV( const UnionBV & src ) {
	*this = src;
};

//Copy Operator
UnionBV & UnionBV::operator=( const UnionBV & src ) {
	this->numBv = src.numBv;
	this->boundingVolumes = src.boundingVolumes;
	return *this;
};

//Destructor
UnionBV::~UnionBV() {};

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
