/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//---THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT---//

//To change this file, alter the niftools/docsys/gen_niflib.py Python script.

#include "nif/gen/ByteColor3.h"
using namespace Niflib;

//Constructor
ByteColor3::ByteColor3() : r((ubyte_t)0), g((ubyte_t)0), b((ubyte_t)0) {};

//Copy Constructor
ByteColor3::ByteColor3( const ByteColor3 & src ) {
	*this = src;
};

//Copy Operator
ByteColor3 & ByteColor3::operator=( const ByteColor3 & src ) {
	this->r = src.r;
	this->g = src.g;
	this->b = src.b;
	return *this;
};

//Destructor
ByteColor3::~ByteColor3() {};

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
