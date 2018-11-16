#include "j1App.h"
#include "p2Log.h"
#include "j1render.h"
#include "j1window.h"
#include "j1input.h"
#include "j2Collision.h"
#include "j2Animation.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "SDL/include/SDL.h"
#include "j1Map.h"
#include "j2Player.h"
#include "j1audio.h"

#include "Brofiler/Brofiler.h"



//CONSTRUCTOR
j2Player::j2Player()
{
	name.create("player");

	pugi::xml_parse_result result =configAnim.load_file("Animations.xml");


	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("idle");//idle
	idle.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("run");//run
	run.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("jump");//jump
	jump.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("jumpDouble");//DoubleJump
	jumpDouble.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("fall");//fall
	fall.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("crouch");//crouch
	crouch.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("push");//push
	push.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("Chargedattack");//Charged attack
	ChargedAttack.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("Basicattack");//basic attack
	BasicAttack.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("Basicattack2");//basic2 attack
	BasicAttack2.LoadPushBack(AnimPushBack);


	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("Airattack");//AirAttack
	AirAttack.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("slide");//slide
	slide.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("die");//die
	die.LoadPushBack(AnimPushBack);
	
	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("GodMode");//GodMode
	GodMode.LoadPushBack(AnimPushBack);
}

//DESTRUCTOR
j2Player::~j2Player()
{

}

bool j2Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Data");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	if (config != NULL)
	{
		//We load everything player_Init so Player will always have a reference for the initial conditions
		//Player Position
		player_Init.playerPos.x = config.child("playerPos").attribute("x").as_int();
		player_Init.playerPos.y = config.child("playerPos").attribute("y").as_int();

		//Player SDL_Rect
		player_Init.playerRect.w = config.child("playerRect").attribute("width").as_int();
		player_Init.playerRect.h = config.child("playerRect").attribute("height").as_int();
		player_Init.playerRect.x = player_Init.playerPos.x;
		player_Init.playerRect.y = player_Init.playerPos.y;
		
		//PlayerCrouchedRect(collider)
		player_Init.playerRectCrouched.w = config.child("playerCrouchCollider").attribute("width").as_int();
		player_Init.playerRectCrouched.h = config.child("playerCrouchCollider").attribute("height").as_int();
		player_Init.playerRectCrouched.x = player_Init.playerRect.x;
		player_Init.playerRectCrouched.y = player_Init.playerPos.y;
		
		//playerJumpRects(colliders)
		player_Init.playerRectJump.w = config.child("playerJumpCollider").attribute("width").as_int();
		player_Init.playerRectJump.h = config.child("playerJumpCollider").attribute("height").as_int();
		player_Init.playerRectJump.x = player_Init.playerRect.x;
		player_Init.playerRectJump.y = player_Init.playerPos.y;

		player_Init.playerRectDJump.w = config.child("playerJumpDCollider").attribute("width").as_int();
		player_Init.playerRectDJump.h = config.child("playerJumpDCollider").attribute("height").as_int();
		player_Init.playerRectDJump.x = player_Init.playerRect.x;
		player_Init.playerRectDJump.y = player_Init.playerPos.y;

		//PlayerRunningRect(collider)
		player_Init.PlayerRectRunning.w = config.child("PlayerRectRunning").attribute("width").as_int();
		player_Init.PlayerRectRunning.h = config.child("PlayerRectRunning").attribute("height").as_int();
		player_Init.PlayerRectRunning.x = player_Init.playerRect.x;
		player_Init.PlayerRectRunning.y = player_Init.playerPos.y;

		//playerStateOfActionRects(colliders)
		player_Init.PlayerRectAttackCharged.w = config.child("PlayerRectAttackCharged").attribute("width").as_int();
		player_Init.PlayerRectAttackCharged.h = config.child("PlayerRectAttackCharged").attribute("height").as_int();
		player_Init.PlayerRectAttackCharged.x = player_Init.playerRect.x;
		player_Init.PlayerRectAttackCharged.y = player_Init.playerPos.y;

		player_Init.PlayerRectAttackAir.w = config.child("PlayerRectAttackAir").attribute("width").as_int();
		player_Init.PlayerRectAttackAir.h = config.child("PlayerRectAttackAir").attribute("height").as_int();
		player_Init.PlayerRectAttackAir.x = player_Init.playerRect.x;
		player_Init.PlayerRectAttackAir.y = player_Init.playerPos.y;

		 player_Init.colliding.colliderOffsetGodMode.x=config.child("colliderOffsetGodMode").attribute("valueX").as_int();
		 player_Init.colliding.colliderOffsetGodMode.y = config.child("colliderOffsetGodMode").attribute("valueY").as_int();

		 //playerAttackColliders
		 player_Init.ChargedAttackCollider.x=  config.child("ChargedAttackCollider").attribute("Adjustmentx").as_int();
		 player_Init.ChargedAttackCollider.y =  config.child("ChargedAttackCollider").attribute("Adjustmenty").as_int();
		 player_Init.ChargedAttackCollider.w = config.child("ChargedAttackCollider").attribute("w").as_int();
		 player_Init.ChargedAttackCollider.h = config.child("ChargedAttackCollider").attribute("h").as_int();

		 player_Init.AirAttackCollider.x= config.child("AirAttackCollider").attribute("Adjustmentx").as_int();
		 player_Init.AirAttackCollider.y = config.child("AirAttackCollider").attribute("Adjustmenty").as_int();
		 player_Init.AirAttackCollider.w = config.child("AirAttackCollider").attribute("w").as_int();
		 player_Init.AirAttackCollider.h = config.child("AirAttackCollider").attribute("h").as_int();

		 player_Init.BasicAttackCollider.x = config.child("BasicAttackCollider").attribute("Adjustmentx").as_int();
		 player_Init.BasicAttackCollider.y = config.child("BasicAttackCollider").attribute("Adjustmenty").as_int();
		 player_Init.BasicAttackCollider.w = config.child("BasicAttackCollider").attribute("w").as_int();
		 player_Init.BasicAttackCollider.h = config.child("BasicAttackCollider").attribute("h").as_int();
		
		//Player Speeds
		JumpForce = config.child("Jumpforce").attribute("value").as_float();
		Currentacceleration = config.child("Currentacceleration").attribute("value").as_float();
		ChargedDesaceleration= config.child("ChargedDesaceleration").attribute("value").as_float();
		gravity = config.child("gravity").attribute("value").as_float();
		Maxspeed.x =config.child("Maxspeed").attribute("x").as_float();
		Maxspeed.y = config.child("Maxspeed").attribute("y").as_float();
		Impulse.x = config.child("Impulse").attribute("x").as_float();
		Impulse.y = config.child("Impulse").attribute("y").as_float();

		//Player DoubleJump
		player_Init.doubleJump = config.child("doubleJump").attribute("value").as_bool();
		

		//Player collider Control
		player_Init.colliding.wallFront = config.child("collisionControlcolliding").attribute("wallFront").as_bool();
		player_Init.colliding.wallBack = config.child("collisionControlcolliding").attribute("wallBack").as_bool();
		player_Init.colliding.wallDown = config.child("collisionControlcolliding").attribute("wallDown").as_bool();
		player_Init.colliding.wallTop = config.child("collisionControlcolliding").attribute("wallTop").as_bool();

		//PlayerCollision Adjusters;
		player_Init.colliding.x_CollisionAdjuster = config.child("xCollisionAdjuster").attribute("x").as_int();
		player_Init.colliding.y_CollisionController = config.child("vCollision_controller").attribute("y").as_int();

		//integers
		player_Init.colliding.colliderOffsetGroundBasic = config.child("colliderOffsetGroundBasic").attribute("value").as_int();
		player_Init.colliding.colliderOffsetGroundSlash = config.child("colliderOffsetGroundSlash").attribute("value").as_int();
		player_Init.colliding.collisionOffsetY = config.child("colliderRectOffsetY").attribute("value").as_int();
		
		//iPoint
		player_Init.colliding.colliderOffset.x = player_Init.colliding.colliderOffsetGroundBasic;
		player_Init.colliding.colliderOffset.y = player_Init.colliding.collisionOffsetY;
		
		//Player Bools Movement
		ToMoveRight = config.child("ToMoveRight").attribute("value").as_bool();
		 ToMoveLeft = config.child("ToMoveLeft").attribute("value").as_bool();
		 ToMoveUp = config.child("ToMoveUp").attribute("value").as_bool();
		 ToMoveDown = config.child("ToMoveDown").attribute("value").as_bool();
	
		 MovingRight = config.child("MovingRight").attribute("value").as_bool();
		 MovingLeft = config.child("MovingLeft").attribute("value").as_bool();
		 MovingUp = config.child("MovingUp").attribute("value").as_bool();
		 MovingDown = config.child("MovingDown").attribute("value").as_bool();

		 lookingRight = config.child("lookingRight").attribute("value").as_bool();

		 //CurrentState
		 CurrentState =(Player_State)config.child("IntilaPState").attribute("State").as_int();

		//Player landed
		player_Init.landed = config.child("landed").attribute("value").as_bool();

		//Player Death
		player_Init.dead = config.child("dead").attribute("boolDead").as_bool();
		player_Init.deadDelay = config.child("dead").attribute("deadDelay").as_int();
		player_Init.deadCounter = config.child("dead").attribute("deadCounter").as_int();
		player_Init.maximumDeadY_map1 = config.child("maximumDead_Y").attribute("map1").as_int();
		player_Init.maximumDeadY_map2 = config.child("maximumDead_Y").attribute("map2").as_int();

		//Player Godmode
		GodModeB = config.child("godMode").attribute("value").as_bool();
	
		NullifyPlayerColliders(player);

		//LoadFX Paths
		player_fx.jumpSoundPath =config.child("FX").child("jump").attribute("path").as_string();
		player_fx.runningSoundPath =config.child("FX").child("run").attribute("path").as_string();
		player_fx.doublejumpSoundPath = config.child("FX").child("jumpDouble").attribute("path").as_string();
		player_fx.dieSoundPath = config.child("FX").child("die").attribute("path").as_string();


		//Blit Values && frameDataAnimis
		PivotAdjustment = config.child("PivotAdjustment").attribute("value").as_uint();
		


	}
	else 
	{
		NullifyPlayerColliders(player);
		LOG("Could not Load Player data on Awake!");
	}

	return ret;
}

// Load Game State
bool j2Player::Load(pugi::xml_node& data)
{
	player.playerPos.x = data.child("playerPos").attribute("x").as_int();
	player.playerPos.y = data.child("playerPos").attribute("y").as_int();

	Speed.x= data.child("Speed").attribute("x").as_int();
	Speed.y = data.child("Speed").attribute("y").as_int();


	/*playerSave = data.append_child("ToMoveRight");
	playerSave.append_attribute("value") = ToMoveRight;*/
	//bools

	ToMoveRight = data.child("ToMoveRight").attribute("value").as_bool();
	ToMoveLeft = data.child("ToMoveLeft").attribute("value").as_bool();
	ToMoveUp = data.child("ToMoveUp").attribute("value").as_bool();
	ToMoveDown= data.child("ToMoveDown").attribute("value").as_bool();

	MovingRight = data.child("MovingRight").attribute("value").as_bool();
	MovingLeft = data.child("MovingLeft").attribute("value").as_bool();
	MovingUp = data.child("MovingUp").attribute("value").as_bool();
	MovingDown = data.child("MovingDown").attribute("value").as_bool();

	ChargedAttackB = data.child("ChargedAttackB").attribute("value").as_bool();
	BasicAttackB = data.child("BasicAttackB").attribute("value").as_bool();
	AirAttackB = data.child("AirAttackB").attribute("value").as_bool();
	arealAttackUsed = data.child("arealAttackUsed").attribute("value").as_bool();

	player.colliding.wallTop=data.child("WallTop").attribute("value").as_bool();
	player.colliding.wallDown = data.child("WallDown").attribute("value").as_bool();
	player.colliding.wallFront = data.child("wallFront").attribute("value").as_bool();
	player.colliding.wallBack = data.child("wallBack").attribute("value").as_bool();

	player.dead = data.child("dead").attribute("value").as_bool();

	player.landed = data.child("landed").attribute("value").as_bool();

	//counters
	player.deadCounter = data.child("dead").attribute("deadCounter").as_int();

	if (GodModeB == false)
	{
		player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);
		player.fakeHitbox->SetPos(player.playerHitbox->rect.x - 1, player.playerHitbox->rect.y - 1);
	}

	return true;
}

// Save Game State
bool j2Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node playerSave = data.append_child("playerPos");
	
	playerSave.append_attribute("x") = player.playerPos.x;
	playerSave.append_attribute("y") = player.playerPos.y;

	

	//Speeds&&Accelerations
	playerSave = data.append_child("Speed");
	playerSave.append_attribute("x") = Speed.x;
	playerSave.append_attribute("y") = Speed.y;

	//counters
	playerSave.append_attribute("deadCounter") = player.deadCounter;

	//bools

	playerSave = data.append_child("ToMoveRight");
	playerSave.append_attribute("value") = ToMoveRight;

	playerSave = data.append_child("ToMoveLeft");
	playerSave.append_attribute("value") = ToMoveLeft;

	playerSave = data.append_child("ToMoveUp");
	playerSave.append_attribute("value") = ToMoveUp;

	playerSave = data.append_child("ToMoveDown");
	playerSave.append_attribute("value") = ToMoveDown;

	playerSave = data.append_child("MovingRight");
	playerSave.append_attribute("value") = MovingRight;

	playerSave = data.append_child("MovingLeft");
	playerSave.append_attribute("value") = MovingLeft;

	playerSave = data.append_child("MovingUp");
	playerSave.append_attribute("value") = MovingUp;

	playerSave = data.append_child("MovingDown");
	playerSave.append_attribute("value") = MovingDown;

	playerSave = data.append_child("lookingRight");
	playerSave.append_attribute("value") = lookingRight;

	playerSave = data.append_child("FirstJump");
	playerSave.append_attribute("value") = FirstJump;

	playerSave = data.append_child("ChargedAttackB");
	playerSave.append_attribute("value") = ChargedAttackB;

	playerSave = data.append_child("BasicAttackB");
	playerSave.append_attribute("value") = BasicAttackB;

	playerSave = data.append_child("AirAttackB");
	playerSave.append_attribute("value") = AirAttackB;

	playerSave = data.append_child("arealAttackUsed");
	playerSave.append_attribute("arealAttackUsed") = AirAttackB;

	playerSave = data.append_child("GodModeB");
	playerSave.append_attribute("GodModeB") = AirAttackB;

	playerSave = data.append_child("wallFront");
	playerSave.append_attribute("wallFront") = player.colliding.wallFront;

	playerSave = data.append_child("wallDown");
	playerSave.append_attribute("wallDown") = player.colliding.wallDown;

	playerSave = data.append_child("wallTop");
	playerSave.append_attribute("wallTop") = player.colliding.wallTop;

	playerSave = data.append_child("wallBack");
	playerSave.append_attribute("wallBack") = player.colliding.wallBack;

	playerSave = data.append_child("landed");
	playerSave.append_attribute("value") = player.landed;

	playerSave = data.append_child("dead");
	playerSave.append_attribute("value") = player.dead;

	

	return true;
}


bool j2Player::Start()
{
	LOG("Player Start");
	
	//Load all data from player_Init into the player we will be using
	//Player Position
	player.playerPos = player_Init.playerPos;
	//Player SDL_Rects
	player.playerRect = player_Init.playerRect;
	player.playerRectCrouched = player_Init.playerRectCrouched;
	player.playerRectJump= player_Init.playerRectJump;
	player.PlayerRectRunning = player_Init.PlayerRectRunning;
	player.PlayerRectAttackCharged = player_Init.PlayerRectAttackCharged;
	player.PlayerRectAttackAir = player_Init.PlayerRectAttackAir;
	player.playerRectDJump = player_Init.playerRectDJump;

	player.ChargedAttackCollider= player_Init.ChargedAttackCollider;
	player.AirAttackCollider = player_Init.AirAttackCollider;
	player.BasicAttackCollider = player_Init.BasicAttackCollider;
	//Player Speeds

	player.doubleJump = player_Init.doubleJump;
	
	//Player collider Control
	player.colliding = player_Init.colliding;
	//Player landed
	player.landed = player_Init.landed;

	//player Dead
	player.dead = player_Init.dead;
	player.deadDelay = player_Init.deadDelay;
	player.deadCounter = player_Init.deadCounter;
	player.maximumDeadY_map1 = player_Init.maximumDeadY_map1;
	player.maximumDeadY_map2 = player_Init.maximumDeadY_map2;

	//player SoundFX
	player_fx.jumpSound = App->audio->LoadFx(player_fx.jumpSoundPath.GetString());
	player_fx.runningSound = App->audio->LoadFx(player_fx.runningSoundPath.GetString());
	player_fx.doublejumpSound = App->audio->LoadFx(player_fx.doublejumpSoundPath.GetString());
	player_fx.dieSound = App->audio->LoadFx(player_fx.dieSoundPath.GetString());

	
	player.fakeCollisionRect = { player.playerRect.x - 1, player.playerRect.y - 1, player.playerRect.w + 2, player.playerRect.h + 2 };
	

	CreatePlayerColliders(player);

	
	
	playTex = App->tex->Load(folder.GetString());//loading Player textures

	return true;
}

bool j2Player::CleanUp()
{
	LOG("Freeing Player");

	if (player.playerHitbox != nullptr)
	{
		player.playerHitbox->to_delete;
		player.playerHitbox = nullptr;
	}

	if (player.playerGodModeHitbox != nullptr)
	{
		player.playerGodModeHitbox->to_delete;
		player.playerGodModeHitbox = nullptr;
	}

	if (playTex != nullptr)
	{
		App->tex->UnLoad(playTex);

	}

	return true;
}


bool j2Player::PreUpdate()
{
	BROFILER_CATEGORY("Player_PreUpdate", Profiler::Color::Aqua);
	//PREUPDATE is called before any On Collision or Pre-Collision from the player is called
	// so we set vars like landed to false and in case we get a call back that the player is landed it will be changed in said functions.
	player.nextFrameLanded = false;
	
	if (player.colliding.wallDown == false) {
		player.landed = false;

	}

	return true;
}


bool j2Player::Update(float dt)      
{
	BROFILER_CATEGORY("Player_Update", Profiler::Color::Aquamarine);
	if (player.dead == true)
	{
		PlayFXDie = true;

		if (player.deadCounter < player.deadDelay)
		{
			player.deadCounter += 1;

		}
		else
		{

			//Destroy the player Colliders
			player.playerHitbox->to_delete = true;
			player.fakeHitbox->to_delete = true;
			player.playerHitbox = nullptr;
			player.fakeHitbox = nullptr;

			//Player Goes To inital position of the current stage map
			if (App->scene->CurrentMap2 == false)
				App->render->camera.x = App->map->SetPlayerToInitial(App->map->data);

			else
				App->render->camera.x = App->map->SetPlayerToInitial(App->map->data2);

			player.dead = false;
			player.deadCounter = player_Init.deadCounter;
		}
	}

	if (Speed.y == 0) {

		arealAttackUsed = false;
	}
	
	
		//Check inputs
		PlayerMovementInputs();
		//DebugFuncionalities
		PlayerDebugF();
		//CheckMovement
		CheckPlayerMovement();
		//switchStates
		SwithcingStates(dt);
		//Switch the colliders shape depending of the state
		ColliderShapeStates();
		//Delete AttackCOlliders if its needed
		CheckCollidersAttacks();
		//players Effects
		PlayerFX();
		//movePlayer
		PlayerMovement(dt);
		
	
	//If the player falls and surpasses a determined Y position it dies
	if (App->scene->CurrentMap2 == false)
	{
		if (player.playerPos.y > player.maximumDeadY_map1)
		{
			player.dead = true;
		}
	}
	else
	{
		if (player.playerPos.y > player.maximumDeadY_map2)
		{
			player.dead = true;
		}
	}



	//Here we change the values of the rect position
	if (GodModeB == false
		&& player.playerHitbox != nullptr && player.playerHitbox->to_delete == false
		&& player.fakeHitbox != nullptr && player.fakeHitbox->to_delete == false)
	{
		player.playerHitbox->SetPos(player.playerPos.x + player.colliding.colliderOffset.x, player.playerPos.y + player.colliding.colliderOffset.y);
		player.fakeHitbox->SetPos(player.playerHitbox->rect.x -1 , player.playerHitbox->rect.y -1);
		
		if (player.PlayerAttackCollider != nullptr) {

			if (ChargedAttackB == true) {
			
				SetColliderRespectPivot(lookingRight,player.PlayerAttackCollider,player.playerPos, player.ChargedAttackCollider.x, player.ChargedAttackCollider.y);
		
			} 
			else if (AirAttackB==true) {

				SetColliderRespectPivot(lookingRight, player.PlayerAttackCollider, player.playerPos, player.AirAttackCollider.x, player.AirAttackCollider.y);

			}
			else if (BasicAttackB==true) {

				SetColliderRespectPivot(lookingRight, player.PlayerAttackCollider, player.playerPos, player.BasicAttackCollider.x, player.BasicAttackCollider.y);


			}
	
		}
	}
	else if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
		player.playerGodModeHitbox->SetPos(player.playerPos.x + player_Init.colliding.colliderOffsetGodMode.x, player.playerPos.y+ player_Init.colliding.colliderOffsetGodMode.y);

	//App->collision->Update(dt);
	//App->collision->Update(dt);



	//Camera Following player
	App->render->followPlayer(player,dt);
	//AnimationsConditions

	AnimationRect = currentAnimation->GetCurrentFrame(dt);
	

	if (lookingRight) {
		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(playTex, player.playerPos.x- PivotAdjustment, player.playerPos.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}


	return true;

}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	BROFILER_CATEGORY("Player_PostUpdate", Profiler::Color::CadetBlue);
	// We reset the colliders collisions
	

	player.colliding.wallFront = false;
	player.colliding.wallBack = false;
	player.colliding.wallDown = false;
	player.colliding.wallTop = false;
  
	//	//Here we change the values of the rect position
	//if(player.playerHitbox != nullptr && player.playerHitbox->to_delete == false)
	//player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);
	//if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
	//player.playerGodModeHitbox->SetPos(player.playerPos.x, player.playerPos.y);

	
	

	return true;
}


void j2Player::OnCollision(Collider* c1, Collider* c2) 
{
	BROFILER_CATEGORY("Player_OnCollision", Profiler::Color::Cyan);
	//Testing new system of collisions
	SDL_Rect overlay; // SDL_Rect of the intersection between the 2 colliders
	SDL_Rect* col1;
	SDL_Rect* col2;
	col1 = &c1->rect;
	col2 = &c2->rect;

	SDL_IntersectRect(col1, col2, &overlay);
	
	if (c1->type == COLLIDER_PLAYER)  //This collider manages hits by enemies and corrects player position on collision if necessary
	{
		if (c2->type == COLLIDER_WALL
			|| c2->type == COLLIDER_ICE
			|| c2->type == COLLIDER_PLATFORM
			|| c2->type == COLLIDER_CLIMBWALL)
		{
			
			//Conditions to know if the collider that we collided with is in Front of the player
			if (player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x
				&& c2->rect.x - player.playerHitbox->rect.x > 0
				&& c2->rect.y + 8 < player.playerHitbox->rect.y + player.playerHitbox->rect.h
				&& overlay.y < overlay.y + overlay.h
				&& player.colliding.wallTop == false)
			{
				player.colliding.wallFront = true;
				//Before we do anything else, don't allow the collider to enter the tile
				//player.playerHitbox->rect.x -= player.colliding.x_CollisionAdjuster;
				player.playerHitbox->rect.x -= overlay.w;
			}
			//Conditions to know if the collider that we collided with is Behind of the player
			else if (player.playerHitbox->rect.x < c2->rect.x + c2->rect.w
				&& player.playerHitbox->rect.x - c2->rect.x > 0
				&& c2->rect.y + 8 < player.playerHitbox->rect.y + player.playerHitbox->rect.h
				&& player.colliding.wallTop == false)
			{
				player.colliding.wallBack = true;
				//Before we do anything else, don't allow the collider to enter the tile
				//player.playerHitbox->rect.x += player.colliding.x_CollisionAdjuster;
				player.playerHitbox->rect.x += overlay.w;
				//player.playerPos.x += overlay.w;
				//player.playerRect.y = c2->rect.y - player.playerRect.h;
			}
			//Conditions to know if the collider that we collided with is Under the player
			else if (player.playerHitbox->rect.y + player.playerHitbox->rect.h > c2->rect.y
				&& player.playerHitbox->rect.y + player.playerHitbox->rect.h < c2->rect.y + c2->rect.h
				&& player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x
				&& c2->rect.x + c2->rect.w > player.playerHitbox->rect.x
				/*&& Speed.y > 0*/)
				//player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x)
			{
				player.landed = true;
				player.colliding.wallDown = true;

				player.playerHitbox->rect.y -= overlay.h;
				//player.playerHitbox->type-= 
			}

			//Conditions to know if the collider that we collided with is over the player
			//Also, if the collider is a PLATFORM, let us go through it
			else if (player.playerHitbox->rect.y > c2->rect.y
				&& player.playerHitbox->rect.y < c2->rect.y + c2->rect.h
				&& player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x
				&& c2->rect.x + c2->rect.w > player.playerHitbox->rect.x
				&& c2->type != COLLIDER_PLATFORM)
			{

				player.playerHitbox->rect.y += overlay.h;
				Speed.y = -Speed.y; // change the speed to inmediately falling (bouncing off the Top)

				player.landed = false;
				player.colliding.wallTop = true;
			}

			
		}
		//If the collider is a killing obstacle DIE
		if (c2->type == COLLIDER_TRAP)
		{
			
			player.dead = true;
		}
	}
	else if (c1->type == COLLIDER_PLAYER_CHECK)	//This collider is a +1 pixel margin of the player collision, so we can have data on what's on the top,right,left and under the player
	{
		if (overlay.x < c2->rect.x + c2->rect.w && overlay.x > c2->rect.x
			&& c2->rect.y + player.colliding.y_CollisionController < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
			player.colliding.wallBack = true;
		if (overlay.x + overlay.w > c2->rect.x  && overlay.x == c2->rect.x
			&& c2->rect.y + player.colliding.y_CollisionController < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
			player.colliding.wallFront = true;
		
		if (overlay.y + overlay.h > c2->rect.y
			&& overlay.y +overlay.h < c2->rect.y + c2->rect.h
			&& player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x
			&& c2->rect.x + c2->rect.w > player.playerHitbox->rect.x)
		{
			player.landed = true;
			player.colliding.wallDown = true;
		}

		//if (overlay.y < overlay.y + overlay.h
		//	/*&& player.fakeHitbox->rect.y + player.fakeHitbox->rect.h > overlay.y + overlay.h*/
		//	&& player.playerHitbox->rect.x + player.playerHitbox->rect.w < c2->rect.x
		//	&& c2->rect.x + c2->rect.w < player.playerHitbox->rect.x)
		if (player.fakeHitbox->rect.y > c2->rect.y
			&& player.fakeHitbox->rect.y < c2->rect.y + c2->rect.h
			&& player.fakeHitbox->rect.x + player.fakeHitbox->rect.w - 5> c2->rect.x
			&& c2->rect.x + c2->rect.w -5> player.fakeHitbox->rect.x)
		{
			player.colliding.wallTop = true;
		}
	}

	//At the end put the player pos onto the collider Pos THIS IS ONLY FOR TESTING CHANGE/FIX @Dídac
	player.playerPos.x = player.playerHitbox->rect.x - player.colliding.colliderOffset.x;
	player.playerPos.y = player.playerHitbox->rect.y - player.colliding.colliderOffset.y;

	//player.lateralFakeHitbox->rect.y = player.playerHitbox->rect.y -1;
	
	/*player.lateralFakeHitbox->rect.y = player.playerHitbox->rect.y;
	player.lateralFakeHitbox->rect.x = player.playerHitbox->rect.x -1;*/

	


} 

void j2Player::OnPreCollision(int d) 
{


}


void  j2Player::PlayerMovementInputs() {


	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		ToMoveRight = false;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player.colliding.wallFront == false)
	{
		ToMoveRight = true;

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE )
	{
		ToMoveLeft = false;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player.colliding.wallBack == false)
	{
		ToMoveLeft = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		ToMoveUp = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT )
	{
		ToMoveUp = true;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		ToMoveDown = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		ToMoveDown = true;
	}
	



}

void j2Player::CheckPlayerMovement(){

	if (Speed.x > 0.0f) {
		MovingRight = true;
		MovingLeft = false;
	}
	else if (Speed.x < 0.0f) {
		MovingLeft = true;
		MovingRight = false;
	}
	else if (Speed.x == 0.0f) {
		MovingLeft = false;
		MovingRight = false;
	}

	if (player.landed == true)
	{
		Speed.y = 0.0f;
	}
	else if (Speed.y < 0.0f) {
		MovingUp = true;
		MovingDown = false;
	}
	else if (Speed.y > 0.0f) {
		MovingDown = true;
		MovingUp = false;
	}
	else if (Speed.y == 0.0f) {
		MovingUp = false;
		MovingDown = false;
	}

}

void j2Player::SwithcingStates(float dt) {

	switch (CurrentState) {

	case Player_State::IDLE:
		IdleStateTo(dt);
		break;
	case Player_State::CROUCHING:
		CrouchingStateTo(dt);
		break;
	case  Player_State::RUNNING:
		RunningStateTo(dt);
		break;
	case  Player_State::AIR:
		AirStateTo(dt);
		break;
	case Player_State::ATTACK:
		AttackStateTo(dt);
	}


}

void j2Player::IdleStateTo(float dt) {

	if (player.dead == false) {


		if (ToMoveRight == true && ToMoveLeft == false || ToMoveLeft == true && ToMoveRight == false) {
			CurrentState = Player_State::RUNNING;
		}
		else if (ToMoveUp == true && Speed.y==0) {
			//jump
			FirstJump = true;
			PlayFXJump = true;
			Speed.y = -JumpForce;
		//	player.landed = false;
			CurrentState = Player_State::AIR;
			
		}
		else if (ToMoveDown == true && player.landed==true) {
			CurrentState = Player_State::CROUCHING;
			
		}

		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN ) {
			
			
			BasicAttackB = true;
			PlayerAttacks(dt);
			
				

		}

	}

}
void j2Player::CrouchingStateTo(float dt) {

	if (player.landed != true) {

		CurrentState = Player_State::AIR;

	}
	
	if (ToMoveDown == false) {
		
		if (ToMoveRight == true || ToMoveLeft == true || MovingRight == true || MovingLeft == true)
			CurrentState = Player_State::RUNNING;
		else if(ToMoveRight ==false && ToMoveLeft== false && ToMoveUp==false && ToMoveDown==false && player.landed==true ) {
			CurrentState = Player_State::IDLE;
		}
    
	}
	else if (ToMoveUp == true && ToMoveLeft==false && ToMoveRight==false) {
		//jump
		FirstJump = true;
		PlayFXJump = true;
		Speed.y = -JumpForce;
		CurrentState = Player_State::AIR;

	}
	else if(App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {


		BasicAttackB = true;
		PlayerAttacks(dt);


	}

}
void j2Player::RunningStateTo(float dt) {

	if (player.landed != true) {

		CurrentState = Player_State::AIR;

	}

	if (ToMoveUp == true && ToMoveDown==false && Speed.y == 0) {
		//jump

		FirstJump = true;
		PlayFXJump = true;
		Speed.y = -JumpForce;
		CurrentState = Player_State::AIR;

	}
	
	else if (MovingLeft == false && MovingRight == false) {
		
		if (ToMoveRight == false && ToMoveLeft == false || ToMoveRight == true && ToMoveLeft == true) {
			CurrentState = Player_State::IDLE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && (Speed.x == Maxspeed.x || Speed.x == -Maxspeed.x)) {

		ChargedAttackB = true;
		PlayerAttacks(dt);


	}
	else if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && (Speed.x != Maxspeed.x || Speed.x != -Maxspeed.x)) {


		BasicAttackB = true;
		PlayerAttacks(dt);


	}
}
void j2Player::AirStateTo(float dt) {

	//Double jump is true when the doublejump is used, but if this one is false is that it has not been used

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.doubleJump == false && FirstJump == true && player.landed == false) {
		
		FirstJump = false;
		//playconditon sound
		playeFXDoublejump = true;
		//Reset Animation
		jumpDouble.Reset();

		//Doublejump
		Speed.y =-JumpForce;
		player.doubleJump = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && player.landed==false && MovingUp) {

		if (arealAttackUsed == false) {
			AirAttackB = true;
			PlayerAttacks(dt);
		}

	}

	if (player.landed==true ) {		

		//land
		FirstJump == false;
		jump.Reset();
		player.doubleJump = false;

	
		if (player.dead == false) {
			
			if (ToMoveRight == true || ToMoveLeft == true || MovingRight == true || MovingLeft == true  ) {
				
				if (ToMoveDown == false) {
					CurrentState = Player_State::RUNNING;
				}
				else {
					CurrentState = Player_State::CROUCHING;
			

				}
			}
			else {
				CurrentState = Player_State::IDLE;
			}
		}
	}
	

}

void j2Player::AttackStateTo(float dt) {


	if (Guard.Read() >=1200 && BasicAttackB == true ) {

		BasicAttack.Reset();
		BasicAttackB = false;
		CurrentState = Player_State::IDLE;

	}

	if (player.landed != true && ChargedAttackB == true) {

		Speed.y += gravity;

	}

	if (Speed.x == 0 && ChargedAttackB == true) {
	
		ChargedAttackB = false;
		CurrentState = Player_State::IDLE;
		ChargedAttack.Reset();

	}
	else if (MovingDown == true && AirAttackB == true) {

		AirAttackB = false;
		CurrentState = Player_State::AIR;
		AirAttack.Reset();
	}
	

}

void j2Player::PlayerAttacks(float dt) {


	if (ChargedAttackB == true) {
		
		if (MovingRight) {
			Speed.x += Impulse.x*dt;
		}
		else if (MovingLeft) {
			Speed.x -= Impulse.x*dt;

		}

		player.DeleteColliderChargeA = true;
	}
	 else if (AirAttackB == true) {
		Speed.y = -Impulse.y;
		arealAttackUsed = true;

		player.DeleteColliderAirA = true;
	}

	 else if (BasicAttackB==true) {

		Guard.Start();
		player.DeleteColliderBasicA = true;
	}


	CollidersAttacks();
	CurrentState = Player_State::ATTACK;
}

void j2Player::PlayerMovement(float dt) {
	
	if (player.dead == false) {

		if (GodModeB == false) {

			if (ToMoveRight == true && ToMoveLeft == false && player.colliding.wallFront == false && ChargedAttackB == false && BasicAttackB==false) {
				Speed.x += Currentacceleration*dt;
			}
			else if (ToMoveLeft == true && ToMoveRight == false && player.colliding.wallBack == false && ChargedAttackB == false && BasicAttackB == false) {
				Speed.x -= Currentacceleration*dt;
			}
			else if (CurrentState != Player_State::AIR ) {	
				if (MovingRight == true && ChargedAttackB == false && BasicAttackB == false) {
					Speed.x -= Currentacceleration*dt;

					if (Speed.x < 0.0f)
						Speed.x = 0.0f;
				}
				else if (MovingLeft == true && ChargedAttackB == false && BasicAttackB == false) {
					Speed.x += Currentacceleration*dt;

					if (Speed.x > 0.0f)
						Speed.x = 0.0f;
				}
				
				if (ChargedAttackB == true || BasicAttackB == true) {

					if (MovingRight) {
						
						lookingRight = true;
						MovingLeft = false;

						Speed.x -= ChargedDesaceleration*dt;
						if (Speed.x < 0.0f)
							Speed.x = 0.0f;
					}
					else if (MovingLeft) {

						lookingRight = false;
						MovingRight = false;

						Speed.x += ChargedDesaceleration*dt;
						
						if (Speed.x > 0.0f)
							Speed.x = 0.0f;
					
					}
				
				}

			}

			if (CurrentState == Player_State::AIR && !player.landed || CurrentState == Player_State::ATTACK && AirAttackB==true) {
				//Falling
				Speed.y += gravity*dt;
			}

			// Maximum Speeds
			if (ChargedAttackB == false) {
				if (Speed.x > Maxspeed.x)
					Speed.x = Maxspeed.x;
				else if (Speed.x < -Maxspeed.x)
					Speed.x = -Maxspeed.x;
			}
			
			else {
				if (Speed.x > Maxspeed.x + Impulse.x)
					Speed.x = (Maxspeed.x + Impulse.x);
				else if (Speed.x < -Maxspeed.x - Impulse.x)
					Speed.x = (-Maxspeed.x - Impulse.x);
			}

			if (Speed.y > Maxspeed.y)
				Speed.y = Maxspeed.y;
			else if (Speed.y < -Maxspeed.y)
				Speed.y = -Maxspeed.y;


			//new current position
			player.playerPos.x += Speed.x*dt;
			player.playerPos.y += Speed.y*dt;

		}

		if (GodModeB == true)
		{
			//If GodMode Activated, move around FREELY 
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				Speed.y = -Maxspeed.y;
				player.playerPos.y += Speed.y*dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				Speed.y = +Maxspeed.y;
				player.playerPos.y += Maxspeed.y*dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				Speed.x = +Maxspeed.x;
				player.playerPos.x += Maxspeed.x*dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				Speed.x = -Maxspeed.x;
				player.playerPos.x += -Maxspeed.x*dt;
			}

		}
	}

}


void j2Player::PlayerFX() {

	if (ToMoveRight == true && ToMoveLeft == false) {
		lookingRight = true;
	}
	else if (ToMoveLeft == true && ToMoveRight == false) {
		lookingRight = false;
	}
	
	if (player.dead == false) {

		if (GodModeB == false) {

			switch (CurrentState) {
			case Player_State::IDLE:
				IdleFX();
				break;
			case Player_State::CROUCHING:
				CrouchingFX();
				break;
			case Player_State::RUNNING:
				 RunningFX();
				break;
			case Player_State::AIR:
				AirFX();
				break;

			case Player_State::ATTACK:
				AttackFX();
				break;
			}
		}
		else {

			currentAnimation = &GodMode;
		}
	}
	else {
	
		if (PlayFXDie == true) {
			App->audio->PlayFx(player_fx.dieSound, 0);
			PlayFXDie = false;
		}

		die.Reset();
		currentAnimation = &die;
	}
	
}

	void j2Player::IdleFX(){
				
	currentAnimation = &idle;
		
	}
	void j2Player::CrouchingFX() {
	
		ToMoveRight = false;
		ToMoveLeft = false;
		currentAnimation = &crouch;
	}
	void j2Player::RunningFX() {

		if ((player.colliding.wallFront == true || player.colliding.wallBack == true ) && ChargedAttackB==false) {

			currentAnimation = &push;
		}
		
	/*	else if (BasicAttackB == true) {

			currentAnimation = &BasicAttack;
		}*/

		else {
			App->audio->PlayFx(player_fx.runningSound, 0);
			currentAnimation = &run;
		}

	}
	void j2Player::AirFX() {
	

			if (MovingDown == true && Speed.y>0) {

				currentAnimation = &fall;
			}

			else if (player.doubleJump == true) {

				if (playeFXDoublejump == true) {
					App->audio->PlayFx(player_fx.doublejumpSound, 0);
					playeFXDoublejump = false;
				}

				currentAnimation = &jumpDouble;
			}

			else if(MovingUp==true) {
			
				if (PlayFXJump == true) {
					App->audio->PlayFx(player_fx.jumpSound, 0);
					PlayFXJump = false;
				}
				
				currentAnimation = &jump;
			}

	
	}

	void j2Player::AttackFX() {

		if (AirAttackB == true) {
			currentAnimation = &AirAttack;
		}
		else if (ChargedAttackB == true) {

			currentAnimation = &ChargedAttack;
		}
		else if (BasicAttackB == true) {
			currentAnimation = &BasicAttack;
		}
	
	}

	void j2Player::PlayerDebugF() {

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

			GodModeB = !GodModeB;

			if (GodModeB == true)
			{

				player.playerHitbox->to_delete = true;
				player.fakeHitbox->to_delete = true;
				player.playerGodModeHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_GODMODE, this);
			}
			else
			{

				player.playerGodModeHitbox->to_delete = true;
				player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER, this);
				player.fakeHitbox = App->collision->AddCollider(player.fakeCollisionRect, COLLIDER_PLAYER_CHECK, this);
			}

		}

	}

void j2Player::NullifyPlayerColliders(Player & p)
{
	p.playerHitbox = nullptr;
	p.playerGodModeHitbox = nullptr;
	p.fakeHitbox = nullptr;
}

//This function assumes that there are no player hitboxes and creates them
bool j2Player::CreatePlayerColliders(Player &p)
{
	int ret = -1;

	//In case the players colliders are 
	if (GodModeB == true)
	{
		if (p.playerGodModeHitbox == nullptr)
		{
			p.playerGodModeHitbox = App->collision->AddCollider(player_Init.playerRect,COLLIDER_GODMODE, this);
			p.playerHitbox = nullptr;
			p.fakeHitbox = nullptr;

			ret = 1;
		}
	}
	else
	{
		if (p.playerHitbox == nullptr && p.fakeHitbox == nullptr)
		{
			p.fakeHitbox = App->collision->AddCollider(p.fakeCollisionRect, COLLIDER_PLAYER_CHECK, this);
			p.playerHitbox = App->collision->AddCollider(player_Init.playerRect, COLLIDER_PLAYER, this);
			p.playerGodModeHitbox = nullptr;
			ret = 1;
		}
	}

	return ret;
}


void j2Player::ColliderShapeStates() {

	if (player.playerHitbox != nullptr) {

		switch (CurrentState) {
		case Player_State::IDLE:

			IdleColliderShape();

			break;
		case Player_State::CROUCHING:

			CrouchColliderShape();

			break;
		case Player_State::RUNNING:
			RunnigColliderShape();

			break;
		case Player_State::AIR:

			JumpColliderShape();

			break;

		case Player_State::ATTACK:


			ChargedAttackColliderShape();

			break;
		}

	}
}

void j2Player::IdleColliderShape() {

	player.playerHitbox->rect.w = player.playerRect.w;
	player.playerHitbox->rect.h = player.playerRect.h;
	player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
	player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;
	player.colliding.colliderOffset.y = player.colliding.collisionOffsetY;

}

void j2Player::CrouchColliderShape() {

	player.playerHitbox->rect.w = player.playerRectCrouched.w;
	player.playerHitbox->rect.h = player.playerRectCrouched.h;
	player.colliding.colliderOffset.y = (player.playerRect.h - player.playerRectCrouched.h) + player.colliding.collisionOffsetY;
	player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
	player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

}

void j2Player::JumpColliderShape() {

	if (FirstJump == true) {
	
		player.playerHitbox->rect.w = player.playerRectJump.w;
		player.playerHitbox->rect.h = player.playerRectJump.h;
		player.colliding.colliderOffset.y = (player.playerRect.h - player.playerRectJump.h) + player.colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

	}
	else if (player.doubleJump == true) {


		player.playerHitbox->rect.w = player.playerRectDJump.w;
		player.playerHitbox->rect.h = player.playerRectDJump.h;
		player.colliding.colliderOffset.y = (player.playerRect.h - player.playerRectDJump.h) + player.colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

	}

}

void j2Player::RunnigColliderShape() {


	player.playerHitbox->rect.w = player.PlayerRectRunning.w;
	player.playerHitbox->rect.h = player.PlayerRectRunning.h;
	player.colliding.colliderOffset.y = (player.playerRect.h - player.PlayerRectRunning.h) + player.colliding.collisionOffsetY;
	player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
	player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

}

void j2Player::ChargedAttackColliderShape() {

	if (ChargedAttackB == true) {
		player.playerHitbox->rect.w = player.PlayerRectAttackCharged.w;
		player.playerHitbox->rect.h = player.PlayerRectAttackCharged.h;
		player.colliding.colliderOffset.y = (player.playerRect.h - player.PlayerRectAttackCharged.h) + player.colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;
	}

	else if (AirAttackB == true) {
		player.playerHitbox->rect.w = player.PlayerRectAttackAir.w;
		player.playerHitbox->rect.h = player.PlayerRectAttackAir.h;
		player.colliding.colliderOffset.y = (player.playerRect.h - player.PlayerRectAttackAir.h) + player.colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;
	}

}

void j2Player::CollidersAttacks() {


	if (player.PlayerAttackCollider == nullptr) {

		if (ChargedAttackB == true) {

			player.PlayerAttackCollider = App->collision->AddCollider(player.ChargedAttackCollider, COLLIDER_PLAYERATTACK, this);
		}


		else if (AirAttackB == true) {

			player.PlayerAttackCollider = App->collision->AddCollider(player.AirAttackCollider, COLLIDER_PLAYERATTACK, this);

		}

		else if (BasicAttackB == true) {

			player.PlayerAttackCollider = App->collision->AddCollider(player.BasicAttackCollider, COLLIDER_PLAYERATTACK, this);

		}
	}
}
	

void j2Player::CheckCollidersAttacks() {
	
	if (player.PlayerAttackCollider != nullptr) {
	
		if (ChargedAttackB == false && player.DeleteColliderChargeA==true) {
			player.PlayerAttackCollider->to_delete = true;
			player.PlayerAttackCollider = nullptr;
			player.DeleteColliderChargeA = false;
		}
		else if (AirAttackB==false && player.DeleteColliderAirA==true) {
		
			player.PlayerAttackCollider->to_delete = true;
			player.PlayerAttackCollider = nullptr;
			player.DeleteColliderAirA = false;

		}
		else if (BasicAttackB==false && player.DeleteColliderBasicA == true) {

			player.PlayerAttackCollider->to_delete = true;
			player.PlayerAttackCollider = nullptr;
			player.DeleteColliderBasicA = false;

		}
	}
}

void j2Player::SetColliderRespectPivot(bool lookingTo, Collider*col, iPoint CharacterPos, int Displacementx,int Displacementy) {

	if(lookingTo) 
		col->SetPos(CharacterPos.x + Displacementx, CharacterPos.y + Displacementy);
	else
		col->SetPos(CharacterPos.x, CharacterPos.y + Displacementy);

}