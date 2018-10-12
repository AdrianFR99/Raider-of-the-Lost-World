#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
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
	/*if (loadedMap[SceneCounter]->GetString() != data.child("currentMap").attribute("name").as_string())
	{
		SwitchMap();
	}*/
	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node sceneMap = data.append_child("currentMap");

	sceneMap.append_attribute("name") = loadedMap[SceneCounter]->GetString();

	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	for (int i = 0; i < loadedMap.count();i++) {
		if (i == 1) {
			App->map->Load(loadedMap[i]->GetString(), App->map->data2);
			continue;
		}

		App->map->Load(loadedMap[i]->GetString(), App->map->data);
		
	}
	
	/*App->map->CreateColliders(App->map->data);*/
	
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
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 2 * App->win->GetScale();

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 2 * App->win->GetScale();

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 2 * App->win->GetScale();

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		
		if (switchMap==false)
			switchMap = true;

		else
			switchMap = false;

	}


 
	if (switchMap==false) {
		App->map->Draw(App->map->data);

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data);
		p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d Camera.x =%d Camera.y =%d",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(),
			map_coordinates.x, map_coordinates.y,
			App->render->camera.x, App->render->camera.y);

		App->win->SetTitle(title.GetString());
		
	}

	else {
		App->map->Draw(App->map->data2);

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data2);
		p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d Camera.x =%d Camera.y =%d",
			App->map->data2.width, App->map->data2.height,
			App->map->data2.tile_width, App->map->data2.tile_height,
			App->map->data2.tilesets.count(),
			map_coordinates.x, map_coordinates.y,
			App->render->camera.x, App->render->camera.y);

			App->win->SetTitle(title.GetString());
	}


	//CAREFUL WITH THE MAP DATA
	

	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}




