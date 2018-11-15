#ifndef __j2Player_H__
#define __j2Player_H__

#include "j1Module.h"
#include "j2Animation.h"
#include "p2Point.h"

struct Collider;

enum class Player_State {

	IDLE,
	RUNNING,
	AIR,
	CROUCHING,
	SLIDING,
	ATTACK,

};

struct FXPlayer {
	//Sound FX
	uint jumpSound;
	uint doublejumpSound;
	uint runningSound;
	uint dieSound;

	//Sound FX Paths
	p2SString jumpSoundPath;
	p2SString runningSoundPath;
	p2SString doublejumpSoundPath;
	p2SString dieSoundPath;
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

		iPoint colliderOffset;

		int colliderOffsetGroundBasic;
		int colliderOffsetGroundSlash;
		int collisionOffsetY;


	};

	
	SDL_Rect playerRect;
	SDL_Rect playerRectCrouched;
	SDL_Rect playerRectJump;
	SDL_Rect playerRectDJump;
	SDL_Rect PlayerRectRunning;
	SDL_Rect PlayerRectAttackCharged;
	SDL_Rect PlayerRectAttackAir;
  
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

	void SwithcingStates(float dt);
		void IdleStateTo();
		void CrouchingStateTo();
		void RunningStateTo(float dt);
		void AirStateTo();
		void AttackStateTo();


	void PlayerFX();
		void IdleFX();
		void CrouchingFX();
		void RunningFX();
		void AirFX();
		void AttackFX();

	void PlayerMovement(float dt);




	//Functions apply the Animations and sounds for every state

	void NullifyPlayerColliders(Player &p);
	void ColliderShapeStates();
		void IdleColliderShape();
		void CrouchColliderShape();
		void JumpColliderShape();
		void RunnigColliderShape();
		void ChargedAttackColliderShape();


	bool CreatePlayerColliders(Player &p);
	
	


	//Chek Conditions to change from the current state to others
	

	void PlayerDebugF();


public: //Variables

	Player player;
	Player player_Init;
	FXPlayer player_fx;
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
	
	//Vars for Blitig
	int PivotAdjustment;

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
	
	bool FirstJump = false;
	//FX Sound Booleans
	bool PlayFXJump=false;
	bool playeFXDoublejump = false;
	bool PlayFXDie = false;

	//Attack Booleans
	bool ChargedAttackB = false;
	bool BasicAttackB = false;
	bool AirAttackB = false;
	bool arealAttackUsed = false;

	//God Mode
	bool GodModeB;

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


	Player_State CurrentState;

private:
	p2SString	folder;
};




#endif

