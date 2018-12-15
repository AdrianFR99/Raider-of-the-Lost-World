#ifndef __J2GROUNDENEMY_H__
#define __J2GROUNDENEMY_H__

#include "j2DynamicEntity.h"
#include "j2Animation.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "PugiXml/src/pugixml.hpp"



enum class GROUND_ENEMY_STATE
{
	PATROLLING,
	AIR,
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
	bool Save(pugi::xml_node&) const;


	void OnCollision(Collider* c1, Collider* c2);
	void CheckPreCollision();

	void EntityMovement(float dt);

	void CheckRelativePosition();
	void SwithcingStates(float dt) override;

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
	Animation hurtAnim;
	Animation death;

	SDL_Rect AnimationRect;
	SDL_Rect ColliderRect;
	


	
	iPoint colliderPosition;

	iPoint enemyPathfindingPosition;
	iPoint pathFindingDistance;
	iPoint tileDistanceBetweenEntities;

	j1Timer HurtTime;

	int tileDistance;

	int life;
	int lifeFixed;
	int KnockBack;

	//Vars to load values from xml
	p2SString texturePath;
	int maxtileDistance;
	int maxSoundDistance;

	float speed_x;
	float speed_y;

	int playerPathPositionAdjuster_x;
	int playerPathPositionAdjuster_y;

	int enemyPathPositionAdjuster_x;
	int enemyPathPositionAdjuster_y;
	int colliderOffset_x;
	int colliderOffset_y;


	int colliderRect_w;
	int colliderRect_h;

	bool hurt=false;


	//sounds
	uint HittedSound;
	p2SString HittedPath;
	

private:
	pugi::xml_node AnimPushBack;
	pugi::xml_document configAnim;

	pugi::xml_node enemyNode;
	pugi::xml_document configEnemy;




	bool valid_path;
	const p2DynArray<iPoint>* path;

	iPoint playerPathfindingPosition;

	GROUND_ENEMY_STATE CurrentState;

};







#endif 

