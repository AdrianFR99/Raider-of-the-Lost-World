#ifndef __ELEMENTGUI_H__
#define __ELEMENTGUI_H__

#include "j1Gui.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL/include/SDL.h"  


class ElementGUI
{
public:

	ElementGUI(const char* name, ElementType Element, ElementAction action, iPoint position, SDL_Rect rect, bool isStatic, bool interactable, bool draggable, bool invisible, SDL_Texture*tex = nullptr);

	virtual ~ElementGUI();


	virtual bool Awake() { return true; }
	//Start
	virtual bool Start() { return true; }
	//PreUpdate		
	virtual bool PreUpdate() { return true; }
	//Update		
	virtual bool Update() { return true; }
	//PostUpdate	
	virtual bool PostUpdate() { return true; }
	//CleanUp
	bool CleanUp();

	virtual bool InteractionUpdate() { return true; }
	virtual void UpdatePos() {}

	void debugDraw();

public:

	const char* name;
	iPoint position = { 0,0 };
	iPoint GlobalPosition = { 0,0 };
	iPoint MousePos = { 0,0 };
	iPoint LastMousePos = { 0,0 };

	SDL_Rect rect;
	SDL_Rect InterRect;
	SDL_Texture*tex;

	ElementType type;
	ElementAction action;
	//This value will only be used by the Slider
	float slider_value = 1.0f;

	bool isStatic;

	bool hovering;
	bool clicked;

	bool was_hovered;
	bool was_clicked;

	bool draggable;
	bool dragging;

	bool interactable;
	bool invisible;
	bool being_used;

	uint scale;

	ElementGUI*Parent = nullptr;

	p2List<ElementGUI*> children;

}; 

#endif