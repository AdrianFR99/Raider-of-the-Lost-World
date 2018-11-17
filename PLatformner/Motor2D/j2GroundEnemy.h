#ifndef __J2GROUNDENEMY_H__
#define __J2GROUNDENEMY_H__

#include "j2DynamicEntity.h"
#include "j2Animation.h"
#include "p2DynArray.h"

#include "PugiXml/src/pugixml.hpp"

enum class GROUND_ENEMY_STATE
{
	PATROLLING,
	IDLE,
	WALKING,
	CHASING_PLAYER,
	ATTACKING,
	HURT,
	DEATH,
};

class j2GroundEnemy : public j2DynamicEntity
{
public:

	//Constructor
	j2GroundEnemy();
	//Destructor
	~j2GroundEnemy();

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

	void EntityMovement(float dt);

	void CheckRelativePosition();

	void EntityFX();
	void PatrollingFX();
	void WalkingFX();
	void AttackingFX();
	void HurtFX();
	void DyingFX();

public:
	Animation* currentAnimation;
	Animation idle;
	Animation walk;
	Animation attack;
	Animation hurt;
	Animation death;

	SDL_Rect AnimationRect;

	iPoint enemyPathfindingPosition;
	iPoint pathFindingDistance;
	iPoint tileDistanceBetweenEntities;

	int tileDistance;

private:
	pugi::xml_node AnimPushBack;
	pugi::xml_document configAnim;

	const p2DynArray<iPoint>* path;

	iPoint playerPathfindingPosition;

	GROUND_ENEMY_STATE CurrentState;

};







#endif 

