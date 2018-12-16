#ifndef __J2COINS_PLAYER_GUI_H__
#define __J2COINS_PLAYER_GUI_H__


#include "ElementGUI.h"


struct _TTF_Font;
struct SDL_Texture;

class j2CoinsPlayerGUI :public ElementGUI
{




public:
	
	j2CoinsPlayerGUI(const char* name, ElementType type, ElementAction action, iPoint position, bool isStatic, SDL_Texture* tex,bool draggable, bool interactuable, bool invisible);
	~j2CoinsPlayerGUI();


	bool Awake();
	//Start
	bool Start();
	//PreUpdate		
	bool PreUpdate();
	//Update		
	bool Update();
	//PostUpdate	
	bool PostUpdate();

	bool CleanUp();

public:

	void voidDisplayCoins();

public:

	char size[20];
	const char*Text;

	SDL_Texture*textTexture;

	SDL_Rect RectText;
	
	SDL_Color ColorText;
	_TTF_Font*Textfont;


};

#endif