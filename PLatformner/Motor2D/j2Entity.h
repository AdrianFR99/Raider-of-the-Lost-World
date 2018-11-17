#ifndef __J2ENTITIES_H__
#define __J2ENTITIES_H__

#include "p2SString.h"
#include "p2List.h"
#include "p2Point.h"
#include "PugiXml/src/pugixml.hpp"
#include "SDL_image/include/SDL_image.h"

class j2EntityManager;
struct Collider;

enum class ENTITY_TYPE
{
	FLYING_ENEMY,
	GROUND_ENEMY,
	PLAYER,
	UNKNOWN
};

class j2Entity
{
public:

	//Constructor
	j2Entity(ENTITY_TYPE type) : name("Unnamed"), manager(NULL), type(type) {}
	//Destructor
	virtual ~j2Entity() {}
	//Start
	virtual bool Start() { return true; }
	//PreUpdate		Called each loop iteration
	virtual bool PreUpdate() { return true; }
	//Update		Called each loop iteration
	virtual bool Update(float dt, bool do_logic) { return true; }
	//PostUpdate	Called each loop iteration
	virtual bool PostUpdate() {return true;}
	//CleanUp
	virtual bool CleanUp() { return true; }


	//Load
	virtual bool Load(pugi::xml_node&) { return true; }
	//Save
	virtual bool Save(pugi::xml_node&) const { return true; }

	//OnCollision Callback
	virtual void OnCollision(Collider* c1, Collider* c2) {}

public:
	iPoint position;
	p2SString name;
	bool active;

	SDL_Texture* entityTex = nullptr;
	
	p2List<Collider*> colliders;

	ENTITY_TYPE type;

private:
	j2EntityManager* manager;
};

#endif
