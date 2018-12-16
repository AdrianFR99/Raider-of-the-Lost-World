#ifndef __J2COINS_PLAYER_GUI_H__
#define __J2COINS_PLAYER_GUI_H__


#include "ElementGUI.h"


struct _TTF_Font;
struct SDL_Texture;

class j2VarsPlayerGUI :public ElementGUI
{




public:
	
	j2VarsPlayerGUI(const char* name, ElementType type, ElementAction action, iPoint position, bool isStatic, SDL_Texture* tex,bool draggable, bool interactuable, bool invisible);
	~j2VarsPlayerGUI();


	bool Awake();
	//Start
	bool Start();
	//PreUpdate		
	bool PreUpdate();
	//Update		
	bool Update();
	//PostUpdate	
	bool PostUpdate();

	bool CleanUp() override;

public:

	void voidDisplayCoins();

public:

	char Coins[3];
	const char*CoinText;
	SDL_Texture*CointextTexture;
	SDL_Rect RectText;

	char Score[20];
	const char*ScoreText;
	const char*ScoreTextAUX;
	SDL_Texture*ScoretextTexture;
	SDL_Texture*ScoretextTextureAUX;
	SDL_Rect RectScore;
	SDL_Rect RectScoreAUX;
	

	char Timer[20];
	const char*TimerText;
	const char*TimerTextAUX;
	SDL_Texture*TimertextTexture;
	SDL_Texture*TimertextTextureAUX;
	SDL_Rect RectTimer;
	SDL_Rect RectTimerAUX;
	
	SDL_Color ColorText;
	_TTF_Font*Textfont;


};

#endif