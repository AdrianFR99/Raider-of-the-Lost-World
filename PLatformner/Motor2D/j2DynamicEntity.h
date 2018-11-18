#ifndef __J2DYNAMIC_ENTITIES_H__
#define __J2DYNAMIC_ENTITIES_H__

#include "j2Entity.h"
#include "SDL/include/SDL.h"

struct Collider;


class j2DynamicEntity : public j2Entity
{
public:
	j2DynamicEntity() : j2Entity(type) {}

	virtual ~j2DynamicEntity(){}



	virtual bool Awake(pugi::xml_node& config) { return true; }
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
	virtual bool Load(pugi::xml_node&) { return true; }
	//Save
	virtual bool Save(pugi::xml_node&) const  { return true; }
	//OnCollision Callback
	virtual void OnCollision(Collider* c1, Collider* c2)override {}

	//void j2DynamicEntity::CheckEntityMovement();

	//void j2DynamicEntity::SetColliderRespectPivot(bool lookingTo, Collider*col, iPoint CharacterPos, int Displacementx, int Displacementy);

public:
//vars

public:

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
	
	//fpoints
	fPoint Speed;
	fPoint Maxspeed;

	float gravity;

	//bools
	bool landed;
	bool dead;
	

	bool ToMoveRight = false;
	bool ToMoveLeft = false;
	bool ToMoveUp = false;
	bool ToMoveDown = false;

	bool MovingRight = false;
	bool MovingLeft = false;
	bool MovingUp = false;
	bool MovingDown = false;

	bool lookingRight;
	
	//Blit ints
	int PivotAdjustment;

	collisionControl boundaries;

public:
	//funcitons
	

	void CheckEntityMovement();
	virtual void SwithcingStates(float dt){} //readablefunction
	virtual void EntityFX(){}
	virtual void EntityMovement(float dt){}
	virtual void EntityAttacks(float dt){}

	void SetColliderRespectPivot(bool lookingTo, Collider*col, iPoint CharacterPos, int Displacementx, int Displacementy);




};





#endif