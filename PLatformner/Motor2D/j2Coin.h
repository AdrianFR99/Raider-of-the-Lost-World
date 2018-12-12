#ifndef __J2COIN_H__
#define __J2COIN_H__

#include "j2StaticEntity.h"
#include "j2Animation.h"
#include "PugiXml/src/pugixml.hpp"

class j2Coin :public j2StaticEntity
{
public:
	j2Coin();
	~j2Coin();
	
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
	
public:
	
	Animation CoinAnim;
	Animation*CurrentAnimation;
private:
	pugi::xml_node AnimPushBack;
	pugi::xml_document configAnim;


};

#endif