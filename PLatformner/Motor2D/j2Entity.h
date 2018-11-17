#ifndef __J2ENTITIES_H__
#define __J2ENTITIES_H__

#include "p2SString.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL_image/include/SDL_image.h"
#include "PugiXml/src/pugixml.hpp"

class j2EntityManager;
class j2DynamicEntity;
struct Collider;

enum class ENTITY_TYPE
{
	ENEMY,
	PLAYER,
	UNKNOWN
};

class j2Entity
{
public:

	//Constructor
	j2Entity(ENTITY_TYPE type) : name("Unnamed"),manager(NULL),type(type)  {}
	//Destructor
	virtual ~j2Entity() {}

	virtual bool Awake(pugi::xml_node& config) { return true; }
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
	SDL_Rect EntityRect;
	p2SString name;
	bool active;

	SDL_Texture*EntityText=nullptr;
	p2List<Collider*> colliders;

	ENTITY_TYPE type;

	
	
private:
	j2EntityManager* manager;
};

#endif
