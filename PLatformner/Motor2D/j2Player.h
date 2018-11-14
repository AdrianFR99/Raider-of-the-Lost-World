#ifndef __j2Player_H__
#define __j2Player_H__

#include "j1Module.h"
#include "j2Animation.h"

struct Collider;

enum class Player_State {

	IDLE,
	RUNNING,
	AIR,
	CROUCHING,
	SLIDING,

};

struct FXPlayer {

	p2SString path;
	uint ChunkSize;

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
  
	//fPoint playerPos;

	iPoint playerPos;
	SDL_Rect fakeCollisionRect;

	
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
	Collider* fakeHitbox;
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

	//Main functions
	void PlayerMovementInputs();
	void CheckPlayerMovement();

	void SwithcingStates();
		void IdleStateTo();
		void CrouchingStateTo();
		void RunningStateTo();
		void AirStateTo();

	void PlayerFX();
		void IdleFX();
		void CrouchingFX();
		void RunningFX();
		void AirFX();

	void PlayerMovement(float dt);

	void PlayerAttack(float dt);


	//Functions apply the Animations and sounds for every state

	void NullifyPlayerColliders(Player &p);
	bool CreatePlayerColliders(Player &p);


	//Chek Conditions to change from the current state to others
	

	void PlayerDebugF();


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
	fPoint Impulse;

	float JumpForce;
	float Currentacceleration;
	float ChargedDesaceleration;
	float gravity;

	//Inputs pressed
	bool ToMoveRight=false;
	bool ToMoveLeft = false;
	bool ToMoveUp = false;
	bool ToMoveDown = false;
	

	//Current movemvent
	bool MovingRight = false;
	bool MovingLeft = false;
	bool MovingUp = false;
	bool MovingDown = false;

	bool lookingRight=false;
	
	//FX Sound Booleans
	bool PlayFXJump=false;
	bool playeFXDoublejump = false;
	bool PlayFXDie = false;


	//Attack Booleans
	bool ChargedAttackB = false;
	bool BasicAttackB = false;
	bool AirAttackB = false;
	bool arealAttackUsed = false;

	Animation* currentAnimation=nullptr;
	Animation idle;
	Animation run;
	Animation jump;
	Animation jumpDouble;
	Animation die;
	Animation slide;
	Animation fall;
	Animation crouch;
	Animation GodMode;
	Animation push;
	Animation ChargedAttack;
	Animation BasicAttack;
	Animation AirAttack;

	//Sound FX
	FXPlayer jumpSound;
	FXPlayer DoublejumpSound;
	FXPlayer runningSound;
	FXPlayer DieSound;

	Player_State CurrentState;
private:
	p2SString	folder;
};




#endif

