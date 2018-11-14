#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "j2Entity.h"

class j2Enemy : public j2Entity 
{
public:


	//Start
	bool Start();
	//Update
	bool Update(float dt);
	//CleanUp
	bool CleanUp();




private:



};




#endif