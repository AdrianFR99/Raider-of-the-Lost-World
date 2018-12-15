#ifndef __GUITEXT_H__
#define  __GUITEXT_H__

#include "ElementGUI.h"

struct _TTF_Font;

class j2GUIText :public ElementGUI
{


public:
	j2GUIText(const char* name, ElementType type, ElementAction action, iPoint position, SDL_Rect &rect, bool isStatic, SDL_Texture* tex, const char*Text, bool draggable, bool interactuable, bool invisible);

	~j2GUIText();


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
	void DisplayText();
	void UpdatePos() override;

private:

	const char*Text;
	SDL_Color ColorText;
	_TTF_Font*Textfont;
};

#endif