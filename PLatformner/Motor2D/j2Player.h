#ifndef __j2Player_H__
#define __j2Player_H__

#include "j1Module.h"
#include "j2Animation.h"

struct Collider;

enum class Player_State {

	IDLE,
	RUNNING,
	AIRBORNE,
	CROUCHING,
	SLIDING,


};

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

	
	SDL_Rect playerRect;
	fPoint playerPos;
	
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


	Collider* playerHitbox;
	Collider* playerGodModeHitbox;
	collisionControl colliding;

	
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


	void MovementInputs();
	void ChechMovement();
	void SwithcingStates();
	void PlayerEffects();
	void MovingPlayer();

	void IdleEffects();
	void CrouchingEffects();
	void RunningEffects();
	void AirEffects();

	void IdleMoveCheck();
	void CrouchingMoveCheck();
	void RunningMoveCheck();
	void AirMoveCheck();

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

	//new structure for player
	fPoint Speed;
	fPoint Maxspeed;
	float JumpForce = 5.00;
	float Currentacceleration=0.10;
	float gravity = 0.1;


	bool ToMoveRight=false;
	bool ToMoveLeft = false;
	bool ToMoveUp = false;
	bool ToMoveDown = false;
	bool ToGodMode = false;

	bool MovingRight = false;
	bool MovingLeft = false;
	bool MovingUp = false;
	bool MovingDown = false;

	bool lookingRight=false;

	Animation* currentAnimation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation jumpDouble;
	Animation die;
	Animation slide;
	Animation GodMode;


	Player_State CurrentState;
private:
	p2SString	folder;
};



#endif

