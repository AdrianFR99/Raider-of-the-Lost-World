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
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j2EntityManager.h"
#include "Brofiler/Brofiler.h"



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


		pathfinding_image_path = config.child("pathfinding_image").attribute("path").as_string();
		pathfinding_debug_tex = nullptr;

		folder = config.child_value("folder");

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

	//Load Navigation for the 1st Map
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data,App->map->data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	//Selecting which Colliders create 

	if (CurrentMap2 == false)
		App->map->CreateColliders(App->map->data);

	else
		App->map->CreateColliders(App->map->data2);

	//Play the first song
	p2SString lvl_song("%s%s", App->audio->music_folder.GetString(), App->audio->songs_list.start->data->GetString());
	App->audio->PlayMusic(lvl_song.GetString(), 2.0f);

	//Load pathfining debug image 
	p2SString pathfinding_tex_path("%s%s",folder.GetString(), pathfinding_image_path.GetString());
	pathfinding_debug_tex = App->tex->Load(pathfinding_tex_path.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene_PreUpdate", Profiler::Color::DarkGray);
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y, App->map->data);
	p = App->map->WorldToMap(p.x, p.y,App->map->data);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	//CHANGE/FIX D�dac
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		p = App->render->ScreenToWorld(x, y, App->map->data);
		//App->audio->PlayFx(App->audio->bat_sound, 0, 5);
		App->audio->PlayEnvironmentalFx(App->audio->bat_sound,5,p, App->entities->entities.start->data->position);
		/*App->audio->PlayFx(App->audio->bat_sound, 0, 5);
		App->audio->PlayEnvironmentalFx(App->audio->bat_sound, 5, p, App->player->player.playerPos);*/
		//App->audio->PlayFx(App->audio->bat_sound,-1,5);
		//App->audio->PlayEnvironmentalFx(5, p, App->player->player.playerPos);
		//App->audio->ApplyDistanceAttenuation(dis);
	}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Gray);
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 2 * App->win->GetScale();

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 2 * App->win->GetScale();*/

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

		if (App->entities->entities.start->data->position.x >= App->map->SetLimitPoint(App->map->data)) {
			switchTheMaps();
		}

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

			if (App->entities->entities.start->data->position.x >= App->map->SetLimitPoint(App->map->data2)) {
				switchTheMaps();
			}

	}


	
	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y, App->map->data);
	p = App->map->WorldToMap(p.x, p.y, App->map->data);
	p = App->map->MapToWorld(p.x, p.y, App->map->data);

	App->render->Blit(pathfinding_debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y, App->map->data);
		App->render->Blit(pathfinding_debug_tex, pos.x, pos.y);
	}
	
	return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene_PostUpdate", Profiler::Color::DimGray);
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
		App->entities->player->NullifyPlayerColliders(App->entities->player->player);
		App->map->CreateColliders(App->map->data2);
		App->render->camera.x = App->map->SetPlayerToInitial(App->map->data2);
		CurrentMap2 = true;
		p2SString lvl_song("%s%s", App->audio->music_folder.GetString(), App->audio->songs_list.start->next->data->GetString());
		App->audio->PlayMusic(lvl_song.GetString(), 2.0f);
		
		//Load Navigation for the 2nd Map
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data, App->map->data2))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	
	else {

		App->collision->CleanUp();
		App->entities->player->NullifyPlayerColliders(App->entities->player->player);
		App->map->CreateColliders(App->map->data);
		App->render->camera.x = App->map->SetPlayerToInitial(App->map->data);
		CurrentMap2 = false;
		p2SString lvl_song("%s%s", App->audio->music_folder.GetString(), App->audio->songs_list.start->data->GetString());
		App->audio->PlayMusic(lvl_song.GetString(), 2.0f);

		//Load Navigation for the 1st Map
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data, App->map->data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
}


