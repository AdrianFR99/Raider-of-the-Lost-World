#ifndef __J2STATIC_ENTITIES_H__
#define __J2STATIC_ENTITIES_H__

#include "j2Entity.h"
#include "SDL/include/SDL.h"


class j2StaticEntity:public j2Entity
{
public:

	j2StaticEntity() : j2Entity(type) {};

	virtual ~j2StaticEntity();


public:
	Collider* EntityCollider;
	SDL_Rect  ColliderRect;
	collisionControl Offsets;

};



#endif