#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "j2Entity.h"
#include "PugiXml/src/pugixml.hpp"
class j2Enemy : public j2Entity 
{
public:

	
	//Start
	bool Start();

	//PreUpdate		Called each loop iteration
	bool PreUpdate();
	//Update		Called each loop iteration
	bool Update(float dt);
	//PostUpdate	Called each loop iteration
	bool PostUpdate();

	//CleanUp
	bool CleanUp();

	//Save & Load Functions
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);


private:


};




#endif