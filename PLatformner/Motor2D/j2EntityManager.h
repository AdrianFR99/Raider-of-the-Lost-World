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

	//Constructor
	j2EntityManager();
	//Destructor
	virtual ~j2EntityManager();
	
	// Awake: Called before render is available
	bool Awake(pugi::xml_node& config);
	
	//Start
	bool Start();
	//PreUpdate
	bool PreUpdate();
	//Update
	bool Update(float dt);
	//PostUpdate
	bool PostUpdate();

	bool CleanUp();


public:
	j2Entity* CreateEntity(ENTITY_TYPE type);

private:

	bool	do_logic;
	int		logic_updates_per_second;
	float	update_ms_cycle;
	float   accumulated_time;

	p2List<j2Entity*> entities;
};





#endif

