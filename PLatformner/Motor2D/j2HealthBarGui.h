#ifndef __J2HEALTHBARGUI__H_
#define __J2HEALTHBARGUI__H_


#include "ElementGUI.h"
class j2HealthBarGui :public ElementGUI
{

public:
	
	j2HealthBarGui(const char* name, ElementType type, ElementAction action, iPoint position,bool isStatic, SDL_Texture* tex, bool draggable, bool interactuable, bool invisible);
	~j2HealthBarGui();



	bool Awake();
	//Start
	bool Start();
	//PreUpdate		
	bool PreUpdate();
	//Update		
	bool Update();
	//PostUpdate	
	bool PostUpdate();
	
	

public:


	void DisplayHealthBar();

public:

	SDL_Rect LifeTo3;
	SDL_Rect LifeTo2;
	SDL_Rect LifeTo1;
	SDL_Rect Null;
	
	SDL_Rect ShieldTo2;
	SDL_Rect ShieldTo1;
	
	SDL_Rect rect2;

	int life;
	int shield;
};

#endif