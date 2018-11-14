#ifndef __J2ENTITYMANAGER_H__
#define __J2ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2List.h"

class j2Entity;

enum class ENTITY_TYPE
{
	ENEMY,
	PLAYER,
	UNKNOWN
};

class j2EntityManager : public j1Module
{
public:

	//Start
	bool Start();
	//PreUpdate
	bool PreUpdate();
	//Update
	bool Update(float dt);
	//PostUpdate
	bool PostUpdate();


	bool CleanUp();



	j2Entity* CreateEntity(ENTITY_TYPE type);




private:
	j2Entity* dummy_enemy;
	bool do_logic;
	float accumulated_time;

	p2List<j2Entity*> entities;
};





#endif

