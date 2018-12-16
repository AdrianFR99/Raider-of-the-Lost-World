#ifndef __j2MAINMENU_H__
#define __j2MAINMENU_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

struct SDL_Rect;
struct SDL_Texture;

class j2MainMenu: public j1Module
{

public:
	j2MainMenu();
	~j2MainMenu();


	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

SDL_Rect MainMenuRect;
SDL_Texture*MainMenuTex=nullptr;

p2SString texturePath;

private:
pugi::xml_node AnimPushBack;
pugi::xml_document configAnim;
p2SString			folder;

};






#endif
