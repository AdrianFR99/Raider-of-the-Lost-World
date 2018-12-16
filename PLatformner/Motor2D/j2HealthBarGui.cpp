#include "j2HealthBarGui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j2EntityManager.h"
#include "j2Player.h"

j2HealthBarGui::j2HealthBarGui(const char* name, ElementType type, ElementAction action, iPoint position, bool isStatic, SDL_Texture* tex, bool draggable, bool interactuable, bool invisible)
	:ElementGUI(name, type, action, position, rect, isStatic, interactuable, draggable, invisible, tex)
{

	


}

j2HealthBarGui::~j2HealthBarGui()
{
}

bool j2HealthBarGui::Awake() {

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


	return true;
}
bool j2HealthBarGui::Start() {
	
	LifeTo3 = {1263,455,128,6 };
	LifeTo2 = {1263,465,84,6 };
	LifeTo1 = {1263,475,42,6};

	ShieldTo1 = { 1263,501,36,5 };
	ShieldTo2 = { 1263, 491, 73, 5 };

	Null = {0,0,0,0};


	return true;
}
//PreUpdate		
bool j2HealthBarGui::PreUpdate() {

	return true;
}
//Update		
bool j2HealthBarGui::Update() {
	
	life=App->entities->player->HitsToRecive;
	shield= App->entities->player->Shield;

	switch (life)
	{
	case 3:
		
		rect = LifeTo3;

	break;
	
	case 2:

		rect = LifeTo2;

		break;
	
	case 1:
		rect = LifeTo1;

		break;

	case 0:

		rect = Null;
		break;

	}


	switch (shield)
	{
	case 2:

		rect2 = ShieldTo2;

		break;

	case 1:

		rect2 = ShieldTo1;

		break;

	case 0:

		rect2 = Null;
		break;




	}

	return true;
}
//PostUpdate	
bool j2HealthBarGui::PostUpdate() {

	DisplayHealthBar();

	return true;
}

bool j2HealthBarGui::CleanUp() {



	App->tex->UnLoad(tex);
	
	return true;
}


void j2HealthBarGui::DisplayHealthBar() {


	App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &rect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y+25, &rect2, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);


 }
