#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j2Player.h"
#include "j2Collision.h"
#include "j1Scene.h"



j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	
		pugi::xml_node Scenes;
	
		for (Scenes = config.child("Map"); Scenes != NULL; Scenes = Scenes.next_sibling("Map")) {

		p2SString*Stringmap = new p2SString();

		Stringmap->create(Scenes.attribute("file").as_string());

		loadedMap.add(Stringmap);
		
		}





		return ret;
}

// Load Game State
bool j1Scene::Load(pugi::xml_node& data)
{
	if (data.child("switchScene").attribute("value").as_bool() != CurrentMap2)
	{
		switchTheMaps();
	}
	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node switchScene = data.append_child("switchScene");

	switchScene.append_attribute("value") = CurrentMap2;



	return true;
}

// Called before the first frame
bool j1Scene::Start()
{

	LOG("Loading Maps");

	for (int i = 0; i < loadedMap.count(); i++) {
		if (i == 1) {
			App->map->Load(loadedMap[i]->GetString(), App->map->data2);
			continue;
		}

		App->map->Load(loadedMap[i]->GetString(), App->map->data);

	}

	//Selecting which Colliders create 

	if (CurrentMap2 == false)
		App->map->CreateColliders(App->map->data);

	else
		App->map->CreateColliders(App->map->data2);


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		switchTheMaps();

	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

		if (CurrentMap2 == false)
			App->render->camera.x = App->map->SetPlayerToInitial(App->map->data);

		else
			App->render->camera.x = App->map->SetPlayerToInitial(App->map->data2);
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN){
		
		if (CurrentMap2 == false)
			App->render->camera.x =App->map->SetPlayerToInitial(App->map->data);

		else
			switchTheMaps();

}





	if (CurrentMap2 == false) {	  	      //Draw Map 1
		App->map->Draw(App->map->data);

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data);
		/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d Camera.x =%d Camera.y =%d",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(),
			map_coordinates.x, map_coordinates.y,
			App->render->camera.x, App->render->camera.y);

		App->win->SetTitle(title.GetString());*/

		if (App->player->player.playerPos.x >= App->map->SetLimitPoint(App->map->data)) {
			switchTheMaps();
		}


		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->player->player.colliding.wallFront == false)
		{

			//moving the offsetx of parallax map1
			App->map->data.imagelayers.At(1)->data->SpeedBack -= App->map->data.imagelayers.At(1)->data->PropImg.GetProperty("speed", 0.5); 
			App->map->data.imagelayers.At(0)->data->SpeedBack -= App->map->data.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.25);
		}


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->player->player.colliding.wallFront == false)
		{

			//moving the offsetx of parallax map1
			App->map->data.imagelayers.At(1)->data->SpeedBack += App->map->data.imagelayers.At(1)->data->PropImg.GetProperty("speed",0.5);
			App->map->data.imagelayers.At(0)->data->SpeedBack += App->map->data.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.25);
		}

		/*if (App->player->player.dead) {

			App->map->data.imagelayers.At(0)->data->SpeedBack = 0.0;
			App->map->data.imagelayers.At(3)->data->SpeedBack = 0.0;
			App->map->data.imagelayers.At(2)->data->SpeedBack = 0.0;

		}*/

	}


	else {
		App->map->Draw(App->map->data2);   //Draw Map 2

		int x, y;
		App->input->GetMousePosition(x, y);
		/*iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data2);*/
		/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d Camera.x =%d Camera.y =%d",
			App->map->data2.width, App->map->data2.height,
			App->map->data2.tile_width, App->map->data2.tile_height,
			App->map->data2.tilesets.count(),
			map_coordinates.x, map_coordinates.y,
			App->render->camera.x, App->render->camera.y);*/

			/*App->win->SetTitle(title.GetString());*/

			if (App->player->player.playerPos.x >= App->map->SetLimitPoint(App->map->data2)) {
				switchTheMaps();
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->player->player.colliding.wallFront == false)
			{

				//moving the offsetx of parallax map2
				App->map->data2.imagelayers.At(0)->data->SpeedBack -=App->map->data2.imagelayers.At(0)->data->PropImg.GetProperty("speed",0.0);
				App->map->data2.imagelayers.At(3)->data->SpeedBack -= App->map->data2.imagelayers.At(3)->data->PropImg.GetProperty("speed", 0.2);
				App->map->data2.imagelayers.At(2)->data->SpeedBack -= App->map->data2.imagelayers.At(2)->data->PropImg.GetProperty("speed", 0.15);

			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->player->player.colliding.wallBack == false)
			{

				//moving the offsetx of parallax map2
				App->map->data2.imagelayers.At(0)->data->SpeedBack += App->map->data2.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.0);
				App->map->data2.imagelayers.At(3)->data->SpeedBack += App->map->data2.imagelayers.At(3)->data->PropImg.GetProperty("speed", 0.2);
				App->map->data2.imagelayers.At(2)->data->SpeedBack += App->map->data2.imagelayers.At(2)->data->PropImg.GetProperty("speed", 0.15);

			}

	}

	/*if (App->player->player.dead ) {

		App->map->data2.imagelayers.At(0)->data->SpeedBack = 0.0;
		App->map->data2.imagelayers.At(3)->data->SpeedBack = 0.0;
		App->map->data2.imagelayers.At(2)->data->SpeedBack = 0.0;

	}*/

	

	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->map->CleanUp(App->map->data2);
		ret = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::switchTheMaps()
{
	
	if (CurrentMap2 == false) {
		App->collision->CleanUp();
		App->map->CreateColliders(App->map->data2);
		App->map->SetPlayerToInitial(App->map->data2); 
		CurrentMap2 = true;
	}

	else {

		App->collision->CleanUp();
		App->map->CreateColliders(App->map->data);
		App->map->SetPlayerToInitial(App->map->data);
		CurrentMap2 = false;

	}
}


