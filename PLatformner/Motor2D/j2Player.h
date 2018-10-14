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
		
		//Collision Adjusters
		int x_CollisionAdjuster;
		int y_CollisionController;
	};

	struct animations
	{
		
		Animation* currentAnimation;
		Animation idle;
		Animation run;
		Animation jump;
		Animation jumpDouble;
		Animation die;
		Animation slide;
		Animation GodMode;
	};
	
	SDL_Rect playerRect;
	iPoint playerPos;
	//Player Speeds
	int gravity_speed;
	int y_max_speed;
	int x_speed, y_speed;
	int actual_x_speed, actual_y_speed;
	int stopped_speed;
	
	int d_to_ground;
	//DoubleJump
	int doubleJump_delay;
	int doubleJump_counter;
	bool doubleJump;
	//Landed statuses
	bool landed;
	bool nextFrameLanded;
	//Death vars
	bool dead;
	int deadDelay;
	int deadCounter;
	int maximumDeadY_map1;
	int maximumDeadY_map2;
	//God Mode
	bool godMode;

	//Collision Adjusters
	int x_CollisionAdjuster;
	int y_CollisionController;

	//Animations bools
	bool idle_Bool_Left=false;
	bool idle_Bool_Right = false;
	bool run_Bool_Left = false;
	bool run_Bool_Right = false;
	bool jump_Bool = false;
	bool die_Bool = false;
	bool GodMode_Left = false;

	Collider* playerHitbox;
	Collider* playerGodModeHitbox;
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

	

	//DEBUG FUNCTIONS THAT SHOULD BE IN SOME COLLISIONS CPP
	/*bool j2Player::CheckCollision(const SDL_Rect& r) const;
	bool j2Player::PreCheckCollision(const SDL_Rect& r) const;
	bool j2Player::CheckVerticalCollision(const SDL_Rect& r) const;*/

public: //Variables

	Player player;

	Player player_Init;
	
	SDL_Texture* playTex = nullptr;

	pugi::xml_node AnimPushBack;
	pugi::xml_document configAnim;

	SDL_Rect AnimationRect;

private:
	p2SString	folder;
};



#endif

