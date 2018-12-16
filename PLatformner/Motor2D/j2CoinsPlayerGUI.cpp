#include "j2CoinsPlayerGUI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j2EntityManager.h"
#include "j2Player.h"
#include "j1Fonts.h"
#include "SDL_TTF\include\SDL_ttf.h"


j2CoinsPlayerGUI::j2CoinsPlayerGUI(const char* name, ElementType type, ElementAction action, iPoint position, bool isStatic, SDL_Texture* tex,bool draggable, bool interactuable, bool invisible)
:ElementGUI(name,type, action, position, rect, isStatic, interactuable, draggable, invisible, tex)
{
}


j2CoinsPlayerGUI::~j2CoinsPlayerGUI()
{
}


bool j2CoinsPlayerGUI::Awake() {

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
bool j2CoinsPlayerGUI::Start() {

	rect = {1335,244,20,20};
	RectText = {0,0,20,20};

	return true;
}
//PreUpdate		
bool j2CoinsPlayerGUI::PreUpdate() {



	return true;
}
//Update		
bool j2CoinsPlayerGUI::Update() {

	sprintf_s(size,4, "%d", App->entities->player->Coins);
	Text = size;
	textTexture = App->font->Print(Text, ColorText, Textfont);

	return true;
}
//PostUpdate	
bool j2CoinsPlayerGUI::PostUpdate() {

	voidDisplayCoins();

	return true;
}

bool j2CoinsPlayerGUI::CleanUp() {



	App->tex->UnLoad(tex);
	App->tex->UnLoad(textTexture);


	return true;
}



void j2CoinsPlayerGUI::voidDisplayCoins() {




	App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &rect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	App->render->Blit(textTexture, GlobalPosition.x + 50, GlobalPosition.y,&RectText, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	

}