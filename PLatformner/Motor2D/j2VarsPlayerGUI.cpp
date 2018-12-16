#include "j2VarsPlayerGUI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j2EntityManager.h"
#include "j2Player.h"
#include "j1Fonts.h"
#include "SDL_TTF\include\SDL_ttf.h"


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
	RectText = {0,0,35,35};
	RectScore = { -2,0,200,40 };

	return true;
}
//PreUpdate		
bool j2VarsPlayerGUI::PreUpdate() {



	return true;
}
//Update		
bool j2VarsPlayerGUI::Update() {

	if (App->entities->active == true) {
	
		sprintf_s(Coins, 4, "%d", App->entities->player->Coins);
		CoinText = Coins;
		CointextTexture = App->font->Print(CoinText, ColorText, Textfont);


		sprintf_s(Score, 20, "Score: %d", App->entities->player->Score);
		ScoreText = Score;
		ScoretextTexture = App->font->Print(ScoreText, ColorText, Textfont);
	
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

	return true;
}



void j2VarsPlayerGUI::voidDisplayCoins() {




	App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &rect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	App->render->Blit(CointextTexture, GlobalPosition.x + 50, GlobalPosition.y,&RectText, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic,true);
	App->render->Blit(ScoretextTexture, GlobalPosition.x + 600, GlobalPosition.y, &RectScore, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic,true);


}