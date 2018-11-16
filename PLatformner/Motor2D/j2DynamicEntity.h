#ifndef __J2DYNAMIC_ENTITIES_H__
#define __J2DYNAMIC_ENTITIES_H__

#include "j2Entity.h"
#include "SDL/include/SDL.h"

struct Collider;


class j2DynamicEntity:public j2Entity
{
public:
	j2DynamicEntity() : j2Entity() {}

	virtual ~j2DynamicEntity() {}

	//Start
	virtual	bool Start()override { return true; }
	//PreUpdate		Called each loop iteration
	virtual bool PreUpdate()override { return true; }
	//Update		Called each loop iteration
	virtual bool Update(float dt, bool do_logic)override { return true; }
	//PostUpdate	Called each loop iteration
	virtual bool PostUpdate()override { return true; }
	//CleanUp
	virtual bool CleanUp()override { return true; }


	//Load
	virtual bool Load(pugi::xml_node&)override { return true; }
	//Save
	virtual bool Save(pugi::xml_node&) const override { return true; }
	//OnCollision Callback
	virtual void OnCollision(Collider* c1, Collider* c2)override {}

public:
//vars
	struct collisionControl
	{

		bool wallFront;
		bool wallBack;
		bool wallTop;
		bool wallDown;

		//Collision Adjusters
		int x_CollisionAdjuster;
		int y_CollisionController;

		iPoint colliderOffset;
		iPoint colliderOffsetGodMode;

		int colliderOffsetGroundBasic;
		int colliderOffsetGroundSlash;
		int collisionOffsetY;

	};

//playerCollisionsRects
	//Rects
	SDL_Rect EntityRect;
	//fpoints
	fPoint Speed;
	fPoint Maxspeed;

	//bools
	bool landed;
	bool dead;
	
	

	bool ToMoveRight = false;
	bool ToMoveLeft = false;
	bool ToMoveUp = false;
	bool ToMoveDown = false;

	bool lookingRight;
	
	//Blit ints
	int PivotAdjustment;


public:
	//funcitons
	
	virtual void CheckEntityMovement(){}
	virtual void SwithcingStates(float dt){}
	virtual void EntityFX(){}
	virtual void EntityMovement(float dt){}
	virtual void EntityAttacks(float dt){}

	virtual void SetColliderRespectPivot(bool lookingTo, Collider*col, iPoint CharacterPos, int Displacementx, int Displacementy){}


};





#endif