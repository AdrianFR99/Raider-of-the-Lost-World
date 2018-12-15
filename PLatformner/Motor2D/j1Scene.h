#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"


struct SDL_Texture;


class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();



	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//UI Callback override
	void callbackUiElement(ElementGUI *element);

public:

	bool CurrentMap2=false;  // Use as Indicator of Which maps is the current one false=Map1 and true=Map2
	bool DeleteColliders = false;
	bool SpawnEnemiesMap1 = true;
	bool SpawnEnemiesMap2 = false;

	bool PathfindingDebug = false;

	bool loaded = false;
	//Timers

	float Stage1Time;
	float Stage1LastTime;
	

	float Stage2Time;
	float Stage2LastTime;


	bool StageOneTimerStart;
	bool StageTwoTimerStart;

	j1Timer StageOneTimer;
	j1Timer StageTwoTimer;

private:

	p2List<p2SString*> loadedMap;
	void switchTheMaps();

	p2SString folder;
	p2SString pathfinding_image_path;
	SDL_Texture* pathfinding_debug_tex;
	
};

#endif // __j1SCENE_H__