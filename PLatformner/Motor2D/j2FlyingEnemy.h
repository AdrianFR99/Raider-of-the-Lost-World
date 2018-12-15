#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "j2DynamicEntity.h"
#include "j2Animation.h"
#include "p2DynArray.h"

#include "PugiXml/src/pugixml.hpp"

enum class FLYING_ENEMY_STATE
{
	PATROLLING,
	CHASING_PLAYER
};

class j2FlyingEnemy : public j2DynamicEntity
{
public:

	//Constructor
	j2FlyingEnemy();
	//Destructor
	~j2FlyingEnemy();
	
	//Start
	bool Start();

	//PreUpdate		Called each loop iteration
	bool PreUpdate();
	//Update		Called each loop iteration
	bool Update(float dt,bool do_logic);
	//PostUpdate	Called each loop iteration
	bool PostUpdate();

	//CleanUp
	bool CleanUp();

	//Save & Load Functions
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);


	void EntityMovement(float dt);

	void SwithcingStates(float dt)override;
	void EntityFX();
	void PatrollingFX();

	void CheckRelativePosition();

public:
	Animation* currentAnimation;
	Animation idle;
	Animation deadAnim;

	SDL_Rect AnimationRect;
	iPoint enemyPathfindingPosition;
	iPoint pathFindingDistance;
	iPoint tileDistanceBetweenEntities;

	

	Collider* enemy_collider;

	//Vars to load values from xml
	p2SString texturePath;
	int tileDistance;
	int maxtileDistance;
	int maxSoundDistance;

	float speed_x;
	float speed_y;

	int playerPathPositionAdjuster_x;
	int playerPathPositionAdjuster_y;

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
	
	
	FLYING_ENEMY_STATE CurrentState;
};




#endif