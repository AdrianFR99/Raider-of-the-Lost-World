#ifndef __BUTTONCLASS_H__
#define __BUTTONCLASS_H__

#include "ElementGUI.h"
#include "p2Point.h"


struct SDL_Rect;

class j2ButtonClass :public ElementGUI
{
public:
	j2ButtonClass(const char* name, ElementType type, ElementAction action, ButtonType buttonType, iPoint position, SDL_Rect &rect1, SDL_Rect &rect2, SDL_Rect &rect3, bool isStatic, SDL_Texture* tex, bool interactable, bool draggable, bool invisible);


	~j2ButtonClass();



	bool Awake();
	//Start
	bool Start();
	//PreUpdate		
	bool PreUpdate();
	//Update		
	bool Update();
	//PostUpdate	
	bool PostUpdate();


	void DisplayButton();
	bool InteractionUpdate() override;
	void UpdatePos() override;


public:

	iPoint MousePos;

	SDL_Rect hoveringRect;
	SDL_Rect clickedRect;

	ButtonType bType;

	bool draggable_x;
	bool draggable_y;

};




#endif 