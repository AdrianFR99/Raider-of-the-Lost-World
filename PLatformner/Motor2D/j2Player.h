#ifndef __j2MAP_H__
#define __j2MAP_H__

#include "j1Module.h"

class j2Player : public j1Module
{
public:

	//Constructor
	j2Player();

	//Virtual Destructor
	virtual ~j2Player();


	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	
	// Called each loop iteration
	bool Update(float dt);
	
	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public: //Variables

	SDL_Rect playerRect;
	iPoint playerPos;
	int max_speed;


	//Debug purpose RECTS
	SDL_Rect lateralTest;
};



#endif

