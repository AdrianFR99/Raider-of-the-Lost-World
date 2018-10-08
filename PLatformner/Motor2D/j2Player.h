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

	//DEBUG FUNCTIONS THAT SHOULD BE IN SOME COLLISIONS CPP
	bool j2Player::CheckCollision(const SDL_Rect& r) const;
	bool j2Player::PreCheckCollision(const SDL_Rect& r) const;
	bool j2Player::CheckVerticalCollision(const SDL_Rect& r) const;

public: //Variables

	SDL_Rect playerRect;
	iPoint playerPos;
	int x_speed;
	int y_speed;
	int d_to_ground;
	bool landed;


	//Debug purpose RECTS
	SDL_Rect lateralTest;
	SDL_Rect verticalTest;
};



#endif

