#include "j1App.h"
#include "j2MainMenu.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j2EntityManager.h"
#include "ElementGUI.h"


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

	//Play the menu song
	p2SString menu_song("%s%s", App->audio->music_folder.GetString(), App->audio->songs_list.end->data->GetString());
	App->audio->PlayMusic(menu_song.GetString(), 0.5f);

	App->gui->CreateMainMenuScreen();

	return true;

}

// Called each loop iteration
bool j2MainMenu::PreUpdate() {





	return true;
}
bool j2MainMenu::Update(float dt) {

	bool ret = true;

	if (exit_game == true)
	{
		ret = false;
	}



	return ret;

}
bool j2MainMenu::PostUpdate() {


	App->render->Blit(MainMenuTex,0,0,&MainMenuRect);

	return true;

}

// Called before quitting
bool j2MainMenu::CleanUp() {


	if (MainMenuTex !=nullptr)
	{
		App->tex->UnLoad(MainMenuTex);
	}

	return true;

}


void j2MainMenu::callbackUiElement(ElementGUI *element)
{
	if (element->type == ElementType::BUTTON)
	{
		switch (element->action)
		{
		case	ElementAction::PLAY:
			if (element->was_clicked && element->clicked == false)
			{
				App->fade->FadeToBlack(this, App->scene, 3.0f);
			}
			break;

		case	ElementAction::CONTINUE:
			if (element->was_clicked && element->clicked == false)
			{
				App->fade->FadeToBlack(this, App->scene, 3.0f, true);
			}
			break;

		case	ElementAction::SETTINGS:
			if (element->was_clicked && element->clicked == false)
			{
				App->gui->Display("Settings_Window");
			}
			break;

		case	ElementAction::CREDITS:
			if (element->was_clicked && element->clicked == false)
			{
			
			}
			break;

		case	ElementAction::EXIT:
			if (element->was_clicked && element->clicked == false)
			{
				exit_game = true;
			}
			break;
		}
	}
}

