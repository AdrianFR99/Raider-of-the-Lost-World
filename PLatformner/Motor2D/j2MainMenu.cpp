#include "j1App.h"
#include "j2MainMenu.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Render.h"


j2MainMenu::j2MainMenu()
{
	name.create("menu");


	
}


j2MainMenu::~j2MainMenu()
{


	


}


bool j2MainMenu::Awake(pugi::xml_node& config) {

	
	folder.create(config.child("folder").child_value());
	
	p2SString strAux = config.child("MainMenuTex").attribute("path").as_string();
	p2SString strAux2("%s%s", folder.GetString(), strAux.GetString());
	
	MainMenuRect.x = config.child("MainMenuRect").attribute("x").as_int();
	MainMenuRect.y= config.child("MainMenuRect").attribute("y").as_int();
	MainMenuRect.w = config.child("MainMenuRect").attribute("w").as_int();
	MainMenuRect.h = config.child("MainMenuRect").attribute("h").as_int();


	texturePath = strAux2;

	return true;
}

// Called before the first frame
bool j2MainMenu::Start() {

	
	MainMenuTex = App->tex->Load(texturePath.GetString());


	return true;

}

// Called each loop iteration
bool j2MainMenu::PreUpdate() {





	return true;
}
bool j2MainMenu::Update(float dt) {




	return true;

}
bool j2MainMenu::PostUpdate() {


	App->render->Blit(MainMenuTex,0,0,&MainMenuRect);

	return true;

}

// Called before quitting
bool j2MainMenu::CleanUp() {



	App->tex->UnLoad(MainMenuTex);

	return true;

}

