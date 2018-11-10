#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------


struct ObjectGroup {

	p2SString nameGroup;

	struct Object
	{
		p2SString name;
		float x, y, width, height;
		
	};


	~ObjectGroup()
	{
		p2List_item<Object*>* itemP;
		itemP = Objectlist.start;

		while (itemP != NULL)
		{
			RELEASE(itemP->data);
			itemP = itemP->next;
		}

		Objectlist.clear();
	}


	

	p2List<Object*>	Objectlist;

};




struct Properties
{
	struct Property
	{
		p2SString name;
		float value;
	};


	~Properties()
	{
		p2List_item<Property*>* itemP;
		itemP = Propertieslist.start;

		while (itemP != NULL)
		{
			RELEASE(itemP->data);
			itemP = itemP->next;
		}

		Propertieslist.clear();
	}
	
	float GetProperty(const char* value, float def_value) const;

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	Propertieslist;
};

struct ImageLayer {

	SDL_Rect GetImageLayerRect() const;

	p2SString name;
	int Width, Height;
	float OffsetX, OffsetY = 0.0f;
	
	SDL_Texture*texture;

	float SpeedBack;

	Properties PropImg;

};

struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	uint*		data;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// TODO 6 (old): Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(x+(y*width))];
	}


	Properties	properties;

};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<ImageLayer*> imagelayers;
	p2List<ObjectGroup*>ObjectGamesGroup;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw(MapData &DataAux);

	// Called before quitting
	bool CleanUp(MapData &DataAux);

	// Load new map
	bool Load(const char* path, MapData &DataAux);

	bool CreateColliders(MapData &DataAux);

	float SetPlayerToInitial(MapData&DataAux);

	float SetLimitPoint(MapData&DataAux);



	TileSet* GetTilesetFromTileId(int id,MapData& DataAux) const;
	// Coordinate translation methods
	iPoint MapToWorld(int x, int y, MapData &DataAux) const;
	iPoint WorldToMap(int x, int y, MapData &DataAux) const;

	//For Pathfinding
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer, MapData& DataAux) const;
	

private:

	bool LoadMap(MapData &DataAux);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadImageLayer(pugi::xml_node& node, ImageLayer* Image);
	bool LoadProperties(pugi::xml_node& node, Properties& list);
	bool LoadGameObjects(pugi::xml_node& node, ObjectGroup*ObjGroup);
	
public:

	MapData data;
	MapData data2;
	p2List<MapData> MapDataList;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__