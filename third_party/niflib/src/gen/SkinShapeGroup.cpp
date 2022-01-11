/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//---THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT---//

//To change this file, alter the niftools/docsys/gen_niflib.py Python script.

#include "nif/gen/SkinShapeGroup.h"
#include "nif/gen/SkinShape.h"
#include "nif/obj/NiSkinInstance.h"
#include "nif/obj/NiTriBasedGeom.h"
using namespace Niflib;

//Constructor
SkinShapeGroup::SkinShapeGroup() : numLinkPairs((unsigned int)0) {};

//Copy Constructor
SkinShapeGroup::SkinShapeGroup( const SkinShapeGroup & src ) {
	*this = src;
};

//Copy Operator
SkinShapeGroup & SkinShapeGroup::operator=( const SkinShapeGroup & src ) {
	this->numLinkPairs = src.numLinkPairs;
	this->linkPairs = src.linkPairs;
	return *this;
};

//Destructor
SkinShapeGroup::~SkinShapeGroup() {};

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
