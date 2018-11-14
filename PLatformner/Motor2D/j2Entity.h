#ifndef __J2ENTITIES_H__
#define __J2ENTITIES_H__

#include "p2SString.h"
#include "p2Point.h"

class j2Entity
{
public:

	//Constructor
	j2Entity() : name("Unnamed") {}
	virtual ~j2Entity() {}

	//Destructor
	
	//Start
	virtual bool Start() { return true; }
	//Update
	virtual bool Update(float dt) { return true; }
	//CleanUp
	virtual bool CleanUp() { return true; }






public:
	iPoint position;
	p2SString name;
	
};





#endif
