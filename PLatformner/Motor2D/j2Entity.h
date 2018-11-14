#ifndef __J2ENTITIES_H__
#define __J2ENTITIES_H__

#include "p2SString.h"
#include "p2Point.h"


class j2Entity
{
public:

	//Constructor

	//Destructor

	//Start
	virtual bool Start();
	//Update
	virtual bool Update(float dt);
	//CleanUp
	virtual bool CleanUp();






public:
	iPoint position;
	p2SString name;
	
};





#endif
