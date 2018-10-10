#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
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

void j1Map::Draw()
{
	if(map_loaded == false)
		return;
	

	for (int x = 0; x < data.imagelayers.count();++x) {
		
		App->render->Blit(data.imagelayers[x]->texture,data.imagelayers[x]->OffsetX, data.imagelayers[x]->OffsetY, &data.imagelayers[x]->GetImageLayerRect());

	}


//DRAW FUNCTION FOR LAYERS AND TILESTS



	for (int x = 0; x < data.tilesets.count(); x++)
	{
		for (uint l = 0; l < data.layers.count(); l++)
		{

			MapLayer*layer = data.layers.At(l)->data;

			if (layer->properties.GetProperty("Draw", 1) == 0)
				continue;

			for (uint row = 0; row < data.height; row++)
			{
				for (uint column = 0; column < data.width; column++)
				{

					iPoint pos = MapToWorld(column, row);

					uint gid= (data.layers[l]->data[data.layers[l]->Get(column, row)]);

					//Read out the flags
					bool flipped_horizontally = (gid & FLIPPED_HORIZONTALLY_FLAG);
					bool flipped_vertically = (gid & FLIPPED_VERTICALLY_FLAG);
					bool flipped_diagonally = (gid & FLIPPED_DIAGONALLY_FLAG);

					// Clear the flags
						(gid) &= ~(FLIPPED_HORIZONTALLY_FLAG |
						FLIPPED_VERTICALLY_FLAG |
						FLIPPED_DIAGONALLY_FLAG);
					
						//// Resolve the tile
						//for (int i = data.tilesets.count() - 1; i >= 0; --i) {
						//	TileSet*tileset = data.tilesets[i];

						//	if (tileset->firstgid<=gid) {
						//		tiles[y][x] = tileset->tileAt(gid - tileset->firstgid);
						//		break;
						//	}
						//}

					App->render->Blit(data.tilesets[x]->texture,    //texture 
						pos.x,                     //position.x of tile
						pos.y,                         //position.y of tile
						&data.tilesets[x]->GetTileRect(gid)); //rectangle
				}
			}
		}
	}

}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);
	
	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}

	else if (data.type == MapTypes::MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width / 2);
		ret.y = (x + y) * (data.tile_height / 2);
	}

	return ret;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);
	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}

	else if (data.type == MapTypes::MAPTYPE_ISOMETRIC)
	{

		y -= 16;
		ret.x = ((x / (data.tile_width / 2) + (y / (data.tile_height / 2))) / 2);
		ret.y = ((y / (data.tile_height / 2) - (x / (data.tile_width / 2))) / 2);

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
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	//remove all ImageLayers

	p2List_item<ImageLayer*>* item3;
	item3 = data.imagelayers.start;

	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.imagelayers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
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
		ret = LoadMap();
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

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
	}

	//Load ImageLayer Info-------------------------------------------
	pugi::xml_node Image_Layer;
	for (Image_Layer = map_file.child("map").child("imagelayer"); Image_Layer && ret; Image_Layer = Image_Layer.next_sibling("imagelayer"))
	{

		ImageLayer*ImagLay = new ImageLayer();

		ret = LoadImageLayer(Image_Layer,ImagLay);

		if (ret == true)
			data.imagelayers.add(ImagLay);

	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

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

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		p2List_item<ImageLayer*>*Item_ImageLayer = data.imagelayers.start;
		while (Item_ImageLayer != NULL) {

			ImageLayer*I = Item_ImageLayer->data;
			LOG("ImageLayer-----");
			LOG("name:%s",I->name.GetString());
			LOG("image width: %d image height: %d",I->Width,I->Height);
			Item_ImageLayer = Item_ImageLayer->next;

		}

	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
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
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

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
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
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

	if (node.attribute("offsetx").as_int() != NULL)
		Image->OffsetX = node.attribute("offsetx").as_int();


	if (node.attribute("offsety").as_int() != NULL)
	Image->OffsetY = node.attribute("offsety").as_int();
	
	



	

	return ret;
}


TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* itemP = data.tilesets.start;
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
int Properties::GetProperty(const char* value, int def_value) const
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