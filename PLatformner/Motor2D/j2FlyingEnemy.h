#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "j2DynamicEntity.h"
#include "j2Animation.h"

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
	bool Save(pugi::xml_node&);

	void EntityFX();
	void PatrollingFX();

public:
	Animation* currentAnimation;
	Animation idle;

	SDL_Rect AnimationRect;

private:
	pugi::xml_node AnimPushBack;
	pugi::xml_document configAnim;

	FLYING_ENEMY_STATE CurrentState;
};




#endif