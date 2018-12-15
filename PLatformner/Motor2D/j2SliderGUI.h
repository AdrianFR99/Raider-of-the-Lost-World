#ifndef __j2SLIDERGUI_H__
#define __j2SLIDERGUI_H__

#include "ElementGUI.h"
class j2SliderGUI : public ElementGUI
{
public:
	j2SliderGUI(const char* name, ElementType Element, ElementAction action, iPoint position, SDL_Rect rect, SDL_Rect buttonRect, bool isStatic, SDL_Texture*tex, bool draggable, bool interactable, bool invisible);
	~j2SliderGUI();

	//Start
	bool Start();

	//PreUpdate		
	bool PreUpdate();
	//Update		
	bool Update();
	//PostUpdate	
	bool PostUpdate();

	void UpdatePos();

public:
	float slider_value;
	float slider_unitVal;
	uint length;
	bool values_calc = false;
};


#endif


