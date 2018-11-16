#ifndef __J2ENTITIES_H__
#define __J2ENTITIES_H__

#include "p2SString.h"
#include "p2Point.h"
#include "PugiXml/src/pugixml.hpp"

class j2EntityManager;
class j2Entity
{
public:

	//Constructor
	j2Entity() : name("Unnamed"),manager(NULL) {}
	//Destructor
	virtual ~j2Entity() {}
	//Start
	virtual bool Start() { return true; }
	//PreUpdate		Called each loop iteration
	virtual bool PreUpdate() { return true; }
	//Update		Called each loop iteration
	virtual bool Update(float dt) { return true; }
	//PostUpdate	Called each loop iteration
	virtual bool PostUpdate() {return true;}
	//CleanUp
	virtual bool CleanUp() { return true; }


	//Load
	virtual bool Load(pugi::xml_node&) { return true; }
	//Save
	virtual bool Save(pugi::xml_node&) const { return true; }

public:
	iPoint position;
	p2SString name;
	bool active;

private:
	j2EntityManager* manager;
};

#endif
