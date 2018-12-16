#include "j1App.h"
#include "j2MainMenu.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j2EntityManager.h"
#include "ElementGUI.h"
#include "j1Input.h"
#include "PugiXml/src/pugixml.hpp"


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

	App->scene->Disable();
	App->entities->Disable();

	MainMenuTex = App->tex->Load(texturePath.GetString());

	//Play the menu song
	p2SString menu_song("%s%s", App->audio->music_folder.GetString(), App->audio->songs_list.end->data->GetString());
	App->audio->PlayMusic(menu_song.GetString(), 0.5f);

	App->render->camera.x = 0;
	App->render->camera.y = 0;
	App->gui->Hide("Settings_Window");
	App->gui->Hide("Credits_Window");
	App->gui->Hide("InGameUI");
	App->gui->Hide("InGame_Settings_Window");

	App->gui->Display("Main_Menu");

	/*App->gui->CreateMainMenuScreen();*/


	//If the Continue button exists and is disabled and there is a save, enable
	pugi::xml_document save_file;
	pugi::xml_parse_result res;

	res = save_file.load_file("save_game.xml");
	
	ElementGUI* continue_ptr = nullptr;
	for (p2List_item<ElementGUI*>* item = App->gui->ElementList.start; item != nullptr; item = item->next)
	{
		if (item->data->action == ElementAction::CONTINUE)
		{
			continue_ptr = item->data;
			break;
		}
	}
	
	if (res != NULL && continue_ptr !=nullptr)
	{
		continue_ptr->interactable = true;
	}

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

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		App->gui->debug = !App->gui->debug;
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
		MainMenuTex = nullptr;
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

		case	ElementAction::SETTINGS_BACK:
			if (element->was_clicked && element->clicked == false)
			{
				App->gui->Hide("Settings_Window");
			}
			break;

		case	ElementAction::CREDITS:
			if (element->was_clicked && element->clicked == false)
			{
				App->gui->Display("Credits_Window");
			}
			break;

		case	ElementAction::CREDITS_BACK:
			if (element->was_clicked && element->clicked == false)
			{
				App->gui->Hide("Credits_Window");
			}
			break;

		case	ElementAction::WEB:
			if (element->was_clicked && element->clicked == false)
			{
				ShellExecuteA(NULL, "open", "https://adrianfr99.github.io/Raider-of-the-Lost-World/", NULL, NULL, SW_SHOWNORMAL);
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

