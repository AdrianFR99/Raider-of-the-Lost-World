#ifndef __J2LIFE_ITEM_H__
#define __J2LIFE_ITEM_H__

#include "j2StaticEntity.h"
#include "j2Animation.h"
#include "PugiXml/src/pugixml.hpp"


class j2LifeItem : public j2StaticEntity
{
public:

	j2LifeItem();
	~j2LifeItem();

	//Start
	bool Start();
	//PreUpdate		Called each loop iteration
	bool PreUpdate();
	//Update		Called each loop iteration
	bool Update(float dt, bool do_logic);
	//PostUpdate	Called each loop iteration
	bool PostUpdate();

	//CleanUp
	bool CleanUp();

	//Save & Load Functions
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);


	void OnCollision(Collider* c1, Collider* c2);
	void CheckPreCollision();

public:
	Collider* LifeItemCollider;
	SDL_Rect  ColliderRect;
	collisionControl Offsets;


};

#endif