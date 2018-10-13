#ifndef __j2Player_H__
#define __j2Player_H__

#include "j1Module.h"
#include "j2Animation.h"

struct Collider;


struct Player
{
	struct collisionControl 
	{
		bool wallFront;
		bool wallBack;
		bool wallTop;
		bool wallDown;
	};

	struct animations
	{
		SDL_Texture* playTex;
		Animation* currentAnimation;
		Animation idle;
		Animation run;
		Animation jump;
		Animation hurt;
	};
	

	SDL_Rect playerRect;
	iPoint playerPos;
	float gravity_speed;
	int y_max_speed;
	int x_speed, y_speed;
	int actual_x_speed, actual_y_speed;
	int stopped_speed;
	int d_to_ground;
	bool landed;
	bool nextFrameLanded;
	Collider* playerHitbox;
	collisionControl colliding;
	animations animations;
};

class j2Player : public j1Module
{
public:

	//Constructor
	j2Player();

	//Virtual Destructor
	virtual ~j2Player();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	void OnCollision(Collider*, Collider*);
	void OnPreCollision(int d);

	void changedMaps();

	//DEBUG FUNCTIONS THAT SHOULD BE IN SOME COLLISIONS CPP
	/*bool j2Player::CheckCollision(const SDL_Rect& r) const;
	bool j2Player::PreCheckCollision(const SDL_Rect& r) const;
	bool j2Player::CheckVerticalCollision(const SDL_Rect& r) const;*/

public: //Variables

	Player player;
	Player player_Init;
	


	//Debug purpose RECTS
	SDL_Rect lateralTest;
	SDL_Rect verticalTest;
	SDL_Rect lateralTest_2;
	
	Collider* verticalTestHitbox;
	Collider* lateralTestHitbox;

	Collider* lateralTestHitbox_2;
};



#endif

