#ifndef __J2ENTITYMANAGER_H__
#define __J2ENTITYMANAGER_H__

#include "j1Module.h"

class j2Entity;

class j2EntityManager : public j1Module
{
public:

	//Start
	bool Start();
	//PreUpdate
	bool PreUpdate();
	//Update
	bool Update();
	//PostUpdate
	bool PostUpdate();


	bool CleanUp();






private:
	j2Entity* dummy_enemy;
};





#endif

