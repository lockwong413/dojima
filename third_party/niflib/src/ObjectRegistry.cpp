/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

#include "nif/ObjectRegistry.h"
#include "nif/obj/NiDataStream.h"

#include <string>

namespace {

std::vector<std::string> strsplit(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
	tokens.push_back(token);
	}
	return tokens;
}

}

namespace Niflib {

map<string, obj_factory_func> ObjectRegistry::object_map;

void ObjectRegistry::RegisterObject( const string & type_name, obj_factory_func create_func ) {
	object_map[type_name] = create_func;
}

NiObject * ObjectRegistry::_CreateObject( const string & type_name ) {
	auto it = object_map.find(type_name);

	if ( it != object_map.end() ) {
		//Requested type has been registered
		return it->second();
	} else {
		//An unknown type has been encountered
		return NULL; //Return null
	}
}

NiObject * ObjectRegistry::CreateObject( const string & type_name ) {
	// [hacky]
	// NiDataStream may have RTTI arguments to be retrieved.
	if (type_name.find("NiDataStream\x01") == 0) {
		auto tokens = strsplit(type_name, '\x01');
		if (tokens.size() != 3) {
			return nullptr;
		}
		int metadata_usage = std::stoi(tokens[1]);
		int metadata_access = std::stoi(tokens[2]);

		// std::cerr << tokens[0] << " " 
		// 				  << tokens[1] << " (" << metadata_usage << ") " 
		// 				  << tokens[2] << " (" << metadata_access << ") " 
		// 				  << std::endl;
		
		NiDataStream *obj = (NiDataStream*)_CreateObject(tokens[0]);
		
		obj->setUsage(DataStreamUsage(metadata_usage));
		obj->setAccess(DataStreamAccess(metadata_access));

		return obj;
	}

	return _CreateObject(type_name);
}

} //End namespace Niflib
