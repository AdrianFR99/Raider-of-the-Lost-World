#include "j2VarsPlayerGUI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j2EntityManager.h"
#include "j2Player.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "SDL_TTF\include\SDL_ttf.h"
#include <math.h> 


j2VarsPlayerGUI::j2VarsPlayerGUI(const char* name, ElementType type, ElementAction action, iPoint position, bool isStatic, SDL_Texture* tex,bool draggable, bool interactuable, bool invisible)
:ElementGUI(name,type, action, position, rect, isStatic, interactuable, draggable, invisible, tex)
{
}


j2VarsPlayerGUI::~j2VarsPlayerGUI()
{
}


bool j2VarsPlayerGUI::Awake() {

	position.x /= scale;
	position.y /= scale;

	if (Parent != nullptr) {
		GlobalPosition.x = Parent->GlobalPosition.x + position.x;
		GlobalPosition.y = Parent->GlobalPosition.y + position.y;
	}
	else
	{
		GlobalPosition.x = position.x;
		GlobalPosition.y = position.y;
	}

	ColorText.r = 255;
	ColorText.g = 255;
	ColorText.b = 255;
	ColorText.a = 0;

	Textfont = App->font->sanskrit;

	

	return true;
}
//Start
bool j2VarsPlayerGUI::Start() {

	rect = {1335,244,20,20};
	RectText = {0,0,50,40};

	RectScore = { -2,0,50,40 };
	RectScoreAUX = { -2,0,100,40 };

	RectTimer = {-2,0,100,45};
	RectTimerAUX= { -2,0,100,45 };

	ScoreTextAUX = "Score:";
	TimerTextAUX = "Timer:";

	ScoretextTextureAUX = App->font->Print(ScoreTextAUX, ColorText, Textfont);
	TimertextTextureAUX = App->font->Print(TimerTextAUX, ColorText, Textfont);


	return true;
}
//PreUpdate		
bool j2VarsPlayerGUI::PreUpdate() {



	return true;
}
//Update		
bool j2VarsPlayerGUI::Update() {

	if (App->entities->active == true) {
	

		App->tex->UnLoad(CointextTexture);
		CointextTexture = nullptr;

		sprintf_s(Coins, 4, "%d", App->entities->player->Coins);
		CoinText = Coins;
		CointextTexture = App->font->Print(CoinText, ColorText, Textfont);


		App->tex->UnLoad(ScoretextTexture);
		ScoretextTexture = nullptr;
		
		sprintf_s(Score, 20, "%d", App->entities->player->Score);
		ScoreText = Score;
		ScoretextTexture = App->font->Print(ScoreText, ColorText, Textfont);

		
		
		App->tex->UnLoad(TimertextTexture);
		TimertextTexture = nullptr;
		
		if (App->scene->CurrentMap2 == false) {
		
			sprintf_s(Timer, 20, "%.2f s",((int)(App->scene->Stage1Time * 100 + .5)/100.0));
			TimerText = Timer;
			TimertextTexture = App->font->Print(TimerText, ColorText, Textfont);
		}
		else {
			sprintf_s(Timer, 20, "%.2f s",((int)(App->scene->Stage2Time * 100 + .5)/100.0));
			TimerText = Timer;
			TimertextTexture = App->font->Print(TimerText, ColorText, Textfont);

		}
		


	}





	return true;
}
//PostUpdate	
bool j2VarsPlayerGUI::PostUpdate() {

	voidDisplayCoins();

	return true;
}

bool j2VarsPlayerGUI::CleanUp() {


	if (tex != nullptr)
	App->tex->UnLoad(tex);
	if(CointextTexture != nullptr)
	App->tex->UnLoad(CointextTexture);
	if (ScoretextTexture != nullptr)
	App->tex->UnLoad(ScoretextTexture);
	if (TimertextTexture != nullptr)
		App->tex->UnLoad(TimertextTexture);
	if (ScoretextTexture != nullptr)
		App->tex->UnLoad(ScoretextTextureAUX);
	if (TimertextTextureAUX != nullptr)
		App->tex->UnLoad(TimertextTextureAUX);

	return true;
}



void j2VarsPlayerGUI::voidDisplayCoins() {




	App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &rect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	App->render->Blit(CointextTexture, GlobalPosition.x + 75, GlobalPosition.y,&RectText, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic,true);
	App->render->Blit(ScoretextTextureAUX, GlobalPosition.x + 450, GlobalPosition.y, &RectScoreAUX, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic, true);
	App->render->Blit(ScoretextTexture, GlobalPosition.x + 575, GlobalPosition.y, &RectScore, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic,true);
	App->render->Blit(TimertextTextureAUX, GlobalPosition.x + 725, GlobalPosition.y, &RectTimerAUX, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic, true);
	App->render->Blit(TimertextTexture, GlobalPosition.x + 850, GlobalPosition.y, &RectTimer, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic, true);


}