/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//-----------------------------------NOTICE----------------------------------//
// Some of this file is automatically filled in by a Python script.  Only    //
// add custom code in the designated areas or it will be overwritten during  //
// the next update.                                                          //
//-----------------------------------NOTICE----------------------------------//

#ifndef _NIPSBOXEMITTER_H_
#define _NIPSBOXEMITTER_H_

//--BEGIN FILE HEAD CUSTOM CODE--//

//--END CUSTOM CODE--//

#include "nif/obj/NiObject.h"
namespace Niflib {

class NiPSBoxEmitter;
typedef Ref<NiPSBoxEmitter> NiPSBoxEmitterRef;

/*!  */
class NiPSBoxEmitter : public NiObject {
public:
	/*! Constructor */
	NIFLIB_API NiPSBoxEmitter();

	/*! Destructor */
	NIFLIB_API virtual ~NiPSBoxEmitter();

	/*!
	 * A constant value which uniquly identifies objects of this type.
	 */
	NIFLIB_API static const Type TYPE;

	/*!
	 * A factory function used during file reading to create an instance of this type of object.
	 * \return A pointer to a newly allocated instance of this type of object.
	 */
	NIFLIB_API static NiObject * Create();

	/*!
	 * Summarizes the information contained in this object in English.
	 * \param[in] verbose Determines whether or not detailed information about large areas of data will be printed out.
	 * \return A string containing a summary of the information within the object in English.  This is the function that Niflyze calls to generate its analysis, so the output is the same.
	 */
	NIFLIB_API virtual string asString( bool verbose = false ) const;

	/*!
	 * Used to determine the type of a particular instance of this object.
	 * \return The type constant for the actual type of the object.
	 */
	NIFLIB_API virtual const Type & GetType() const;

	//--BEGIN MISC CUSTOM CODE--//

	//--END CUSTOM CODE--//
protected:
	/*! Unknown. */
	IndexString name;
	/*! Unknown. */
	float unknown1;
	/*! Unknown. */
	float unknown2;
	/*! Unknown. */
	ubyte_t unknown3;
	/*! Unknown. */
	ubyte_t unknown4;
	/*! Unknown. */
	ubyte_t unknown5;
	/*! Unknown. */
	ubyte_t unknown6;
	/*! Unknown. */
	float unknown7;
	/*! Unknown. */
	ubyte_t unknown8;
	/*! Unknown. */
	ubyte_t unknown9;
	/*! Unknown. */
	ubyte_t unknown10;
	/*! Unknown. */
	ubyte_t unknown11;
	/*! Unknown. */
	float unknown12;
	/*! Unknown. */
	int unknown13;
	/*! Unknown. */
	float unknown14;
	/*! Unknown. */
	float unknown15;
	/*! Unknown. */
	float unknown16;
	/*! Unknown. */
	float unknown17;
	/*! Unknown. */
	float unknown18;
	/*! Unknown. */
	float unknown19;
	/*! Unknown. */
	float unknown20;
	/*! Unknown. */
	float unknown21;
	/*! Unknown. */
	float unknown22;
	/*! Unknown. */
	ubyte_t unknown23;
	/*! Unknown. */
	ubyte_t unknown24;
	/*! Unknown. */
	ubyte_t unknown25;
	/*! Unknown. */
	ubyte_t unknown26;
	/*! Unknown. */
	ubyte_t unknown27;
	/*! Unknown. */
	ubyte_t unknown28;
	/*! Unknown. */
	ubyte_t unknown29;
	/*! Unknown. */
	ubyte_t unknown30;
	/*! Unknown. */
	ubyte_t unknown31;
	/*! Unknown. */
	ubyte_t unknown32;
	/*! Unknown. */
	ubyte_t unknown33;
	/*! Unknown. */
	ubyte_t unknown34;
	/*! Unknown. */
	ubyte_t unknown35;
	/*! Unknown. */
	ubyte_t unknown36;
	/*! Unknown. */
	ubyte_t unknown37;
	/*! Unknown. */
	ubyte_t unknown38;
	/*! Unknown. */
	ubyte_t unknown39;
	/*! Unknown. */
	ubyte_t unknown40;
	/*! Unknown. */
	ubyte_t unknown41;
	/*! Unknown. */
	ubyte_t unknown42;
	/*! Unknown. */
	ubyte_t unknown43;
	/*! Unknown. */
	ubyte_t unknown44;
	/*! Unknown. */
	ubyte_t unknown45;
	/*! Unknown. */
	ubyte_t unknown46;
	/*! Unknown. */
	ubyte_t unknown47;
	/*! Unknown. */
	ubyte_t unknown48;
public:
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual void Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info );
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual void Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, list<NiObject *> & missing_link_stack, const NifInfo & info ) const;
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual void FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, list<NiObjectRef> & missing_link_stack, const NifInfo & info );
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual list<NiObjectRef> GetRefs() const;
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual list<NiObject *> GetPtrs() const;
};

//--BEGIN FILE FOOT CUSTOM CODE--//

//--END CUSTOM CODE--//

} //End Niflib namespace
#endif
