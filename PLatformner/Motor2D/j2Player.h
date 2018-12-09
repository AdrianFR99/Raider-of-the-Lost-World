#ifndef __j2Player_H__
#define __j2Player_H__


#include "j2DynamicEntity.h"
#include "j2Collision.h"
#include "j2Animation.h"
#include "p2Point.h"
#include "j1Timer.h"

struct Collider;

enum class Player_State {

	IDLE,
	RUNNING,
	AIR,
	CROUCHING,
	ATTACK,
	
};

struct FXPlayer {
	//Sound FX
	uint jumpSound;
	uint doublejumpSound;
	uint runningSound;
	uint dieSound;
	uint SlashSwordSound;
	uint StrongSlashSound;

	//Sound FX Paths
	p2SString jumpSoundPath;
	p2SString runningSoundPath;
	p2SString doublejumpSoundPath;
	p2SString dieSoundPath;
	p2SString SlashSoundPath;
	p2SString StrongSlashPath;
};

struct Player
{
	//playerCollisionsRects
	//SDL_Rect playerRect;//DynamiucEntityREct
	SDL_Rect playerRectCrouched;
	SDL_Rect playerRectJump;
	SDL_Rect playerRectDJump;
	SDL_Rect PlayerRectRunning;
	SDL_Rect PlayerRectAttackCharged;
	SDL_Rect PlayerRectAttackAir;

  //PlayerAttacksCollisionsREcts
	SDL_Rect ChargedAttackCollider;
	SDL_Rect AirAttackCollider;
	SDL_Rect BasicAttackCollider;

	//fPoint playerPos;
	//iPoint playerPos;//entityPos
	SDL_Rect fakeCollisionRect;

	
	bool doubleJump;
	//bool landed; //dynamicEntity
	bool nextFrameLanded;
	//bool dead; //dynamicEntity

	bool DeleteColliderChargeA;
	bool DeleteColliderAirA;
	bool DeleteColliderBasicA;


	int deadDelay;
	int deadCounter;
	int maximumDeadY_map1;
	int maximumDeadY_map2;
	

	//Collision Adjusters
	int x_CollisionAdjuster;
	int y_CollisionController;


	Collider* playerHitbox; 
	Collider* PlayerAttackCollider;
	Collider* fakeHitbox;
	Collider* playerGodModeHitbox;
	
};

class j2Player : public j2DynamicEntity
{
public:

	//Constructor
	j2Player();

	//Virtual Destructor
	virtual ~j2Player();

	// Called before render is available
	bool Awake(pugi::xml_node& config) override;

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt,bool do_logic)override;
	
	// Called each loop iteration
	bool PostUpdate()override;

	// Called before quitting
	bool CleanUp() override;

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	void OnCollision(Collider*c1, Collider*c2) override;
	

	//Main functions
	void PlayerMovementInputs();
	//void CheckPlayerMovement();//DynamicEntity
	//void CheckEntityMovement()override;

	void SwithcingStates(float dt) override;//DynamicEntity

		void IdleStateTo(float dt);
		void CrouchingStateTo(float dt);
		void RunningStateTo(float dt);
		void AirStateTo(float dt);
		void AttackStateTo(float dt);


	void EntityFX() override;//DynamicEntity
		void IdleFX();
		void CrouchingFX();
		void RunningFX();
		void AirFX();
		void AttackFX();

	void EntityMovement(float dt) override;//DynamicEntity
	void EntityAttacks(float dt)override;//DynamicEntity



	

	void NullifyPlayerColliders(Player &p);

	void ColliderShapeStates();
		void IdleColliderShape();
		void CrouchColliderShape();
		void JumpColliderShape();
		void RunnigColliderShape();
		void ChargedAttackColliderShape();

		void CollidersAttacks();
		void CheckCollidersAttacks();

	bool CreatePlayerColliders(Player &p);


	void PlayerDebugF();


public: //Variables

	Player player;

	//As a security Aux player
	FXPlayer player_fx;
	

	pugi::xml_node AnimPushBack;
	pugi::xml_document configAnim;

	SDL_Rect AnimationRect;

	//new structure for player

	fPoint Impulse;

	float JumpForce;
	float Currentacceleration;
	float ChargedDesaceleration;
	float gravity;
	
	//Vars for Blitig
	int PivotAdjustment;

	bool FirstJump = false;
	//FX Sound Booleans
	bool PlayFXJump=false;
	bool playeFXDoublejump = false;
	bool PlayFXDie = false;
	bool PlayFxSwordSwing = false;
	bool playFxSwordStrongSwing = false;

	//Attack Booleans
	bool ChargedAttackB = false;
	bool BasicAttackB = false;
	//bool BasicAttackB2 = false;
	bool AirAttackB = false;
	bool arealAttackUsed = false;
	//bool combo = false;
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
	Animation BasicAttack2;
	Animation AirAttack;


	Player_State CurrentState;


	j1Timer Guard;
	j1Timer DeathTime;
	
	collisionControl colliding;

private:
	p2SString	folder;
};




#endif

