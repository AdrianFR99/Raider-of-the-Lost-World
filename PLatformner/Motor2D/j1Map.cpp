#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j2Collision.h"
#include "j2Player.h"
#include "Brofiler/Brofiler.h"
#include "j2EntityManager.h"
#include "j2Entity.h"
#include <cmath>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");

}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw(MapData&DataAux)
{
	BROFILER_CATEGORY("Map_Draw", Profiler::Color::Violet);
	if(map_loaded == false)
		return;
	
	ImageLayer*Image;

	for (int x = 0; x < DataAux.imagelayers.count();++x) {
		
		Image = DataAux.imagelayers.At(x)->data;

		/*if (Image->PropImg.GetProperty("Movement", 0) == 1) {*/


			App->render->Blit(DataAux.imagelayers[x]->texture,
				Image->SpeedBack, DataAux.imagelayers[x]->OffsetY,
				&DataAux.imagelayers[x]->GetImageLayerRect());
		
	}


//DRAW FUNCTION FOR LAYERS AND TILESTS



	MapLayer* layer /*this->data.layers.start->data*/;


	for (uint i = 0; i < DataAux.layers.count();i++)
	{
		layer = DataAux.layers.At(i)->data;

		if (layer->properties.GetProperty("Draw",0)==0)
			continue;


		for (int y = 0; y < DataAux.height; ++y)
		{
			for (int x = 0; x < DataAux.width; ++x)
			{
				int tileID = layer->Get(x, y);
				if (tileID > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tileID,DataAux);

					if (tileset != nullptr)
					{
						SDL_Rect rect = tileset->GetTileRect(tileID);
						iPoint position = MapToWorld(x, y,DataAux);
					

					
						if (position.x + tileset->tile_width >= (App->render->camera.x / App->win->GetScale())
							&& position.x < ((App->render->camera.x + App->render->camera.w) / App->win->GetScale()))
						{
							App->render->Blit(tileset->texture, position.x, position.y, &rect);
						}
						
					}
				}
			}
		}
	}

	
	
	
}

iPoint j1Map::MapToWorld(int x, int y,MapData &DataAux) const
{
	iPoint ret(0,0);
	
	if (DataAux.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * DataAux.tile_width;
		ret.y = y * DataAux.tile_height;
	}

	else if (DataAux.type == MapTypes::MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (DataAux.tile_width / 2);
		ret.y = (x + y) * (DataAux.tile_height / 2);
	}

	return ret;
}


iPoint j1Map::WorldToMap(int x, int y,MapData& DataAux) const
{
	iPoint ret(0,0);
	if (DataAux.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / DataAux.tile_width;
		ret.y = y / DataAux.tile_height;
	}

	else if (DataAux.type == MapTypes::MAPTYPE_ISOMETRIC)
	{

		y -= 16;
		ret.x = ((x / (DataAux.tile_width / 2) + (y / (DataAux.tile_height / 2))) / 2);
		ret.y = ((y / (DataAux.tile_height / 2) - (x / (DataAux.tile_width / 2))) / 2);

	}
	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect;
	// TODO 7(old): Create a method that receives a tile id and returns it's Rect
	int relative_id = id - firstgid;
	
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}
SDL_Rect ImageLayer::GetImageLayerRect() const {

	SDL_Rect rec;

	rec.w = Width;
	rec.h = Height;
	rec.x = 0;
	rec.y = 0;

	return rec;
}
// Called before quitting
bool j1Map::CleanUp(MapData& DataAux)
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item =  DataAux.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	DataAux.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = DataAux.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	DataAux.layers.clear();

	//remove all ImageLayers

	p2List_item<ImageLayer*>* item3;
	item3 = DataAux.imagelayers.start;

	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	DataAux.imagelayers.clear();
	//remove all GameObjects

	p2List_item<ObjectGroup*>*Item4; 
	Item4 = DataAux.ObjectGamesGroup.start;
	
	while (Item4 != NULL) {

		RELEASE(Item4->data);
		Item4 = Item4->next;
	}

	DataAux.ObjectGamesGroup.clear();

	

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name,MapData& DataAux)
{
	bool ret = true;

	
	
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap(DataAux);
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		DataAux.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			DataAux.layers.add(lay);
	}

	//Load ImageLayer Info-------------------------------------------
	pugi::xml_node Image_Layer;
	for (Image_Layer = map_file.child("map").child("imagelayer"); Image_Layer && ret; Image_Layer = Image_Layer.next_sibling("imagelayer"))
	{

		ImageLayer*ImagLay = new ImageLayer();

		ret = LoadImageLayer(Image_Layer,ImagLay);

		if (ret == true)
			DataAux.imagelayers.add(ImagLay);

	}
	//Load ObjectGroup Info--------------------------------------------
	pugi::xml_node Object_Game;
	for (Object_Game = map_file.child("map").child("objectgroup"); Object_Game && ret; Object_Game = Object_Game.next_sibling("objectgroup"))
	{
		ObjectGroup*ObjGroup = new ObjectGroup();

		ret = LoadGameObjects(Object_Game, ObjGroup);

		if (ret == true)
			DataAux.ObjectGamesGroup.add(ObjGroup);
	}


//-----------------------------------------------------------------


	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", DataAux.width, DataAux.height);
		LOG("tile_width: %d tile_height: %d", DataAux.tile_width, DataAux.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = DataAux.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		p2List_item<ImageLayer*>*Item_ImageLayer = DataAux.imagelayers.start;
		while (Item_ImageLayer != NULL) {

			ImageLayer*I = Item_ImageLayer->data;
			LOG("ImageLayer-----");
			LOG("name:%s",I->name.GetString());
			LOG("image width: %d image height: %d",I->Width,I->Height);
			Item_ImageLayer = Item_ImageLayer->next;

		}

		p2List_item<ObjectGroup*>*Item_ObjectGroup = DataAux.ObjectGamesGroup.start;
		while (Item_ObjectGroup != NULL) {

			ObjectGroup*O = Item_ObjectGroup->data;
			LOG("ObjectGroup name:%s", O->nameGroup.GetString());
			Item_ObjectGroup = Item_ObjectGroup->next;
	
		}


	}

	map_loaded = ret;
	


	return ret;
}

// Load map general properties
bool j1Map::LoadMap(MapData&DataAux)
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		DataAux.width = map.attribute("width").as_int();
		DataAux.height = map.attribute("height").as_int();
		DataAux.tile_width = map.attribute("tilewidth").as_int();
		DataAux.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		DataAux.background_color.r = 0;
		DataAux.background_color.g = 0;
		DataAux.background_color.b = 0;
		DataAux.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			DataAux.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			DataAux.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			DataAux.type = MAPTYPE_STAGGERED;
		}
		else
		{
			DataAux.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layer_data = node.child("data");

	LoadProperties(node,layer->properties);

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{

				layer->data[i++] = tile.attribute("gid").as_int(0);
			

		}
	}

	return ret;
}


bool j1Map::LoadImageLayer(pugi::xml_node& node, ImageLayer* Image) {

	bool ret = true;

	Image->name = node.attribute("name").as_string();
	Image->Width = node.child("image").attribute("width").as_int();
	Image->Height = node.child("image").attribute("height").as_int();
	Image->texture = App->tex->Load(PATH(folder.GetString(),node.child("image").attribute("source").as_string()));

	LoadProperties(node, Image->PropImg);

	if (node.attribute("offsetx").as_int() != NULL)
		Image->OffsetX = node.attribute("offsetx").as_float();


	if (node.attribute("offsety").as_int() != NULL)
	Image->OffsetY = node.attribute("offsety").as_float();
	
	
	/*if (Image->PropImg.GetProperty("Movement", 0) == 1) {
		if (Image->name == "Front")
			Image->SpeedFront = Image->PropImg.GetProperty("Speed", 0);
		else if(Image->name == "Back")
			Image->SpeedBack = Image->PropImg.GetProperty("Speed", 0);
		
	}*/



	

	return ret;
}


TileSet* j1Map::GetTilesetFromTileId(int id,MapData& DataAux) const
{


	p2List_item<TileSet*>* itemP = DataAux.tilesets.start;
	TileSet* set = NULL;

	while (itemP)
	{
		if (id < itemP->data->firstgid)
		{
			set = itemP->prev->data;
			break;
		}
		set = itemP->data;
		itemP = itemP->next;
	}

	return set;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;
	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* property_aux = new Properties::Property();

			property_aux->name = prop.attribute("name").as_string();
			property_aux->value = prop.attribute("value").as_int();

			properties.Propertieslist.add(property_aux);
		}
	}

	return ret;
}
float Properties::GetProperty(const char* value, float def_value) const
{
	p2List_item<Property*>* itemP = Propertieslist.start;

	while (itemP)
	{
		if (itemP->data->name == value)
			return itemP->data->value;
		itemP = itemP->next;
	}

	return def_value;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = Propertieslist.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}


bool j1Map::LoadGameObjects(pugi::xml_node& node, ObjectGroup*ObjAux) {

	bool ret = true;

	ObjAux->nameGroup = node.attribute("name").as_string();

	for (pugi::xml_node Obj = node.child("object"); Obj && ret; Obj = Obj.next_sibling("object"))
	{
		ObjectGroup::Object*Object = new ObjectGroup::Object();

		Object->id = Obj.attribute("id").as_int();
		Object->name = Obj.attribute("name").as_string();
		Object->x = Obj.attribute("x").as_float();
		Object->y = Obj.attribute("y").as_float();
		Object->width = Obj.attribute("width").as_float();
		Object->height = Obj.attribute("height").as_float();

		ObjAux->Objectlist.add(Object);
	}

	

	return ret;
}



bool j1Map::CreateColliders(MapData&DataAux) {

	

	MapLayer* layer2;


	for (uint i = 0; i < DataAux.layers.count(); i++)
	{
		layer2 = DataAux.layers.At(i)->data;

		if (layer2->properties.GetProperty("Collision", 0) == 0)
			continue;

		for (int y = 0; y < DataAux.height; ++y)
		{
			for (int x = 0; x < DataAux.width; ++x)
			{
				int tileID = layer2->Get(x, y);

				if (tileID > 0)
				{
			
					TileSet* tileset = GetTilesetFromTileId(tileID,DataAux);

					if (tileID >= tileset->firstgid) {

						iPoint position = MapToWorld(x, y,DataAux);
						Collider*Coll = nullptr;

					if(tileID==tileset->firstgid)
							Coll = App->collision->AddCollider({ position.x,position.y,tileset->tile_width,tileset->tile_height }, COLLIDER_TRAP);
							
					else if (tileID == tileset->firstgid+1)
							Coll = App->collision->AddCollider({ position.x,position.y,tileset->tile_width,tileset->tile_height }, COLLIDER_CLIMBWALL);
						

					else if (tileID == tileset->firstgid + 2)
							Coll = App->collision->AddCollider({ position.x,position.y,tileset->tile_width,tileset->tile_height }, COLLIDER_WALL);
						
				
					else if (tileID == tileset->firstgid +3)
							Coll = App->collision->AddCollider({ position.x,position.y,tileset->tile_width,tileset->tile_height }, COLLIDER_WATER);
						
					
					else if (tileID == tileset->firstgid +4)
							Coll = App->collision->AddCollider({ position.x,position.y,tileset->tile_width,tileset->tile_height }, COLLIDER_ICE);
						
					else if (tileID == tileset->firstgid + 5)
							Coll = App->collision->AddCollider({ position.x,position.y,tileset->tile_width,tileset->tile_height }, COLLIDER_PLATFORM);
					

						}
						
					
					
				}
			}

		}
	
	}
	return true;
}

void j1Map::SpawnEnemies(MapData&DataAux) {

	

	for (int i = 0; i < DataAux.ObjectGamesGroup.count(); ++i) {
		
		
		if (DataAux.ObjectGamesGroup.At(i)->data->nameGroup == "EnemySpawns") {


			for (int x = 0; x < DataAux.ObjectGamesGroup.At(i)->data->Objectlist.count();x++) {

				if (DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->name =="UndeadSpawn") {

					j2Entity*Aux;
					Aux = App->entities->CreateEntity(ENTITY_TYPE::GROUND_ENEMY);

					int j = App->entities->entities.find(Aux);

					if (j != -1) {

						
						App->entities->entities.At(j)->data->position.x = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->x;
						App->entities->entities.At(j)->data->position.y = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->y;
						LOG("Entity id %i ,Undead Spawned", DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->id);

						EntitiesPositioInList.add(j);

						continue;

					}
				
				}

				else if (DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->name =="BatSpawn") {


					j2Entity*Aux;
					Aux = App->entities->CreateEntity(ENTITY_TYPE::FLYING_ENEMY);

					int j = App->entities->entities.find(Aux);

					if (j != -1) {

						App->entities->entities.At(j)->data->position.x = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->x;
						App->entities->entities.At(j)->data->position.y = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->y;
						LOG("Entity id %i ,Bat Spawned", DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->id);
						
						EntitiesPositioInList.add(j);

						continue;


					}
					


				}

			}


		}

	}
	App->entities->Start();
}


void j1Map::CleanUpMapEnemies() {


	for (int i = 0; i < EntitiesPositioInList.count();++i) {

		if(App->entities->entities.At(EntitiesPositioInList.At(i)->data)!=nullptr)
		App->entities->entities.At(EntitiesPositioInList.At(i)->data)->data->CleanUp();


}

}

float j1Map::SetPlayerToInitial(MapData&DataAux) {

	for (int i = 0; i < DataAux.ObjectGamesGroup.count(); ++i) {

		if (DataAux.ObjectGamesGroup.At(i)->data->nameGroup == "StartingAndFinishPoint") {

			for (int x = 0; x < DataAux.ObjectGamesGroup.At(i)->data->Objectlist.count(); ++x) {

				if (DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->name == "StartingPoint") {

					App->entities->player->Start();
					App->entities->player->position.x = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->x;
					App->entities->player->position.y = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->y;

					break;
				}
			}
		}
	}
	return (App->entities->player->position.x);
}


float j1Map::SetLimitPoint(MapData&DataAux) {

	float limitPos;
	

	for (int i = 0; i < DataAux.ObjectGamesGroup.count(); ++i) {

		if (DataAux.ObjectGamesGroup.At(i)->data->nameGroup == "StartingAndFinishPoint") {

			for (int x = 0; x < DataAux.ObjectGamesGroup.At(i)->data->Objectlist.count(); ++x) {

				if (DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->name == "FinishPoint") {
					
					limitPos = DataAux.ObjectGamesGroup.At(i)->data->Objectlist.At(x)->data->x;
					 break;
				}
			}
			break;
		}
	}

	
	return limitPos;


}

//Walkability map for Pathfinding
bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer, MapData &DataAux) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = DataAux.layers.start;

	for (item = DataAux.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < DataAux.height; ++y)
		{
			for (int x = 0; x < DataAux.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id,DataAux) : NULL;		//Change/Fix Dídac
				//TileSet* tileset = GetTilesetFromTileId(tile_id, MapDataList.start->data);
				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > -1 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = DataAux.width;
		height = DataAux.height;
		ret = true;

		break;
	}

	return ret;
}





