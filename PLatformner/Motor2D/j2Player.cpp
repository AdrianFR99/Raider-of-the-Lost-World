#include "j2Player.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1render.h"
#include "j1window.h"
#include "j1input.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "SDL/include/SDL.h"
#include "j1Map.h"
#include "j1audio.h"
#include "j2EntityManager.h"
#include "j1FadeToBlack.h"

#include "Brofiler/Brofiler.h"



//CONSTRUCTOR
j2Player::j2Player(): j2DynamicEntity()
{
	/*name.create("player");*/
	pugi::xml_parse_result result = configAnim.load_file("Animations.xml");


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

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("dieMidAir");//die
	dieMidAir.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("GodMode");//GodMode
	GodMode.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Player").child("Hurted");
	Hurted.LoadPushBack(AnimPushBack);

	type = ENTITY_TYPE::PLAYER;

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
	config = config.child("player");


	if (config != NULL)
	{
		//We load everything player_Init so Player will always have a reference for the initial conditions
		//Player Position
		position.x = config.child("playerPos").attribute("x").as_int();
		position.y = config.child("playerPos").attribute("y").as_int();

		//Player SDL_Rect
		EntityRect.w = config.child("playerRect").attribute("width").as_int();
		EntityRect.h = config.child("playerRect").attribute("height").as_int();
		EntityRect.x = position.x;
		EntityRect.y = position.y;
		
		//PlayerCrouchedRect(collider)
		player.playerRectCrouched.w = config.child("playerCrouchCollider").attribute("width").as_int();
		player.playerRectCrouched.h = config.child("playerCrouchCollider").attribute("height").as_int();
		player.playerRectCrouched.x = EntityRect.x;
		player.playerRectCrouched.y = position.y;
		
		//playerJumpRects(colliders)
		player.playerRectJump.w = config.child("playerJumpCollider").attribute("width").as_int();
		player.playerRectJump.h = config.child("playerJumpCollider").attribute("height").as_int();
		player.playerRectJump.x = EntityRect.x;
		player.playerRectJump.y = position.y;

		player.playerRectDJump.w = config.child("playerJumpDCollider").attribute("width").as_int();
		player.playerRectDJump.h = config.child("playerJumpDCollider").attribute("height").as_int();
		player.playerRectDJump.x = EntityRect.x;
		player.playerRectDJump.y = position.y;

		//PlayerRunningRect(collider)
		player.PlayerRectRunning.w = config.child("PlayerRectRunning").attribute("width").as_int();
		player.PlayerRectRunning.h = config.child("PlayerRectRunning").attribute("height").as_int();
		player.PlayerRectRunning.x = EntityRect.x;
		player.PlayerRectRunning.y = position.y;

		//playerStateOfActionRects(colliders)
		player.PlayerRectAttackCharged.w = config.child("PlayerRectAttackCharged").attribute("width").as_int();
		player.PlayerRectAttackCharged.h = config.child("PlayerRectAttackCharged").attribute("height").as_int();
		player.PlayerRectAttackCharged.x = EntityRect.x;
		player.PlayerRectAttackCharged.y = position.y;

		player.PlayerRectAttackAir.w = config.child("PlayerRectAttackAir").attribute("width").as_int();
		player.PlayerRectAttackAir.h = config.child("PlayerRectAttackAir").attribute("height").as_int();
		player.PlayerRectAttackAir.x =EntityRect.x;
		player.PlayerRectAttackAir.y = position.y;

		colliding.colliderOffsetGodMode.x=config.child("colliderOffsetGodMode").attribute("valueX").as_int();
		colliding.colliderOffsetGodMode.y = config.child("colliderOffsetGodMode").attribute("valueY").as_int();

		 //playerAttackColliders
		 player.ChargedAttackCollider.x=  config.child("ChargedAttackCollider").attribute("Adjustmentx").as_int();
		 player.ChargedAttackCollider.y =  config.child("ChargedAttackCollider").attribute("Adjustmenty").as_int();
		 player.ChargedAttackCollider.w = config.child("ChargedAttackCollider").attribute("w").as_int();
		 player.ChargedAttackCollider.h = config.child("ChargedAttackCollider").attribute("h").as_int();

		 player.AirAttackCollider.x= config.child("AirAttackCollider").attribute("Adjustmentx").as_int();
		 player.AirAttackCollider.y = config.child("AirAttackCollider").attribute("Adjustmenty").as_int();
		 player.AirAttackCollider.w = config.child("AirAttackCollider").attribute("w").as_int();
		 player.AirAttackCollider.h = config.child("AirAttackCollider").attribute("h").as_int();

		 player.BasicAttackCollider.x = config.child("BasicAttackCollider").attribute("Adjustmentx").as_int();
		 player.BasicAttackCollider.y = config.child("BasicAttackCollider").attribute("Adjustmenty").as_int();
		 player.BasicAttackCollider.w = config.child("BasicAttackCollider").attribute("w").as_int();
		 player.BasicAttackCollider.h = config.child("BasicAttackCollider").attribute("h").as_int();
		
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
		player.doubleJump = config.child("doubleJump").attribute("value").as_bool();
		

		//Player collider Control
		colliding.wallFront = config.child("collisionControlcolliding").attribute("wallFront").as_bool();
		colliding.wallBack = config.child("collisionControlcolliding").attribute("wallBack").as_bool();
		colliding.wallDown = config.child("collisionControlcolliding").attribute("wallDown").as_bool();
		colliding.wallTop = config.child("collisionControlcolliding").attribute("wallTop").as_bool();

		//PlayerCollision Adjusters;
		colliding.x_CollisionAdjuster = config.child("xCollisionAdjuster").attribute("x").as_int();
		colliding.y_CollisionController = config.child("vCollision_controller").attribute("y").as_int();

		//integers
		colliding.colliderOffsetGroundBasic = config.child("colliderOffsetGroundBasic").attribute("value").as_int();
		colliding.colliderOffsetGroundSlash = config.child("colliderOffsetGroundSlash").attribute("value").as_int();
		colliding.collisionOffsetY = config.child("colliderRectOffsetY").attribute("value").as_int();
		
		//iPoint
		colliding.colliderOffset.x = colliding.colliderOffsetGroundBasic;
		colliding.colliderOffset.y = colliding.collisionOffsetY;
		
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

		landed = config.child("landed").attribute("value").as_bool();

		//Player Death
		dead = config.child("dead").attribute("boolDead").as_bool();
		player.deadDelay = config.child("dead").attribute("deadDelay").as_int();
		player.deadCounter = config.child("dead").attribute("deadCounter").as_int();
		player.maximumDeadY_map1 = config.child("maximumDead_Y").attribute("map1").as_int();
		player.maximumDeadY_map2 = config.child("maximumDead_Y").attribute("map2").as_int();

		//Player Godmode
		GodModeB = config.child("godMode").attribute("value").as_bool();
	
		NullifyPlayerColliders(player);

		//LoadFX Paths
		player_fx.jumpSoundPath =config.child("FX").child("jump").attribute("path").as_string();
		player_fx.runningSoundPath =config.child("FX").child("run").attribute("path").as_string();
		player_fx.doublejumpSoundPath = config.child("FX").child("jumpDouble").attribute("path").as_string();
		player_fx.dieSoundPath = config.child("FX").child("die").attribute("path").as_string();
		player_fx.SlashSoundPath = config.child("FX").child("Slash").attribute("path").as_string();
		player_fx.StrongSlashPath= config.child("FX").child("StrongSlash").attribute("path").as_string();

		//Blit Values && frameDataAnimis
		PivotAdjustment = config.child("PivotAdjustment").attribute("value").as_uint();
		//HitsToRecive
		HitsToRecive = config.child("hitToRecive").attribute("value").as_int();
			
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


	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

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

	colliding.wallTop=data.child("WallTop").attribute("value").as_bool();
	colliding.wallDown = data.child("WallDown").attribute("value").as_bool();
	colliding.wallFront = data.child("wallFront").attribute("value").as_bool();
	colliding.wallBack = data.child("wallBack").attribute("value").as_bool();

	dead = data.child("dead").attribute("value").as_bool();
	HitsToRecive= data.child("HitsToReceive").attribute("value").as_int();
	Coins= data.child("Coins").attribute("value").as_int();

	landed = data.child("landed").attribute("value").as_bool();

	//counters
	player.deadCounter = data.child("dead").attribute("deadCounter").as_int();

	if (GodModeB == false)
	{
		player.playerHitbox->SetPos(position.x, position.y);
		player.fakeHitbox->SetPos(player.playerHitbox->rect.x - 1, player.playerHitbox->rect.y - 1);
	}

	return true;
}

// Save Game State
bool j2Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node playerSave = data.append_child("player");
	

	playerSave.append_attribute("x") = position.x;
	playerSave.append_attribute("y") = position.y;

	
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
	playerSave.append_attribute("wallFront") =colliding.wallFront;

	playerSave = data.append_child("wallDown");
	playerSave.append_attribute("wallDown") =colliding.wallDown;

	playerSave = data.append_child("wallTop");
	playerSave.append_attribute("wallTop") = colliding.wallTop;

	playerSave = data.append_child("wallBack");
	playerSave.append_attribute("wallBack") = colliding.wallBack;

	playerSave = data.append_child("landed");
	playerSave.append_attribute("value") = landed;

	playerSave = data.append_child("dead");
	playerSave.append_attribute("value") = dead;

	playerSave = data.append_child("HitsToReceive");
	playerSave.append_attribute("value") = HitsToRecive;

	playerSave = data.append_child("Coins");
	playerSave.append_attribute("value") = Coins;


	return true;
}


bool j2Player::Start()
{
	LOG("Player Start");
	


	//player SoundFX
	player_fx.jumpSound = App->audio->LoadFx(player_fx.jumpSoundPath.GetString());
	player_fx.runningSound = App->audio->LoadFx(player_fx.runningSoundPath.GetString());
	player_fx.doublejumpSound = App->audio->LoadFx(player_fx.doublejumpSoundPath.GetString());
	player_fx.dieSound = App->audio->LoadFx(player_fx.dieSoundPath.GetString());
	player_fx.SlashSwordSound = App->audio->LoadFx(player_fx.SlashSoundPath.GetString());
	player_fx.StrongSlashSound = App->audio->LoadFx(player_fx.StrongSlashPath.GetString());
	
	player.fakeCollisionRect = { EntityRect.x - 1, EntityRect.y - 1, EntityRect.w + 2, EntityRect.h + 2 };
	

	

	CreatePlayerColliders(player);
	CurrentState = Player_State::IDLE;
	if(EntityText==nullptr)
	EntityText = App->tex->Load("textures/adventure.png");//loading Player textures

	FixedHits = HitsToRecive;

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

	if (EntityText != nullptr)
	{
		App->tex->UnLoad(EntityText);

	}

	return true;
}


bool j2Player::PreUpdate()
{
	BROFILER_CATEGORY("Player_PreUpdate", Profiler::Color::Aqua);
	//PREUPDATE is called before any On Collision or Pre-Collision from the player is called
	// so we set vars like landed to false and in case we get a call back that the player is landed it will be changed in said functions.
	player.nextFrameLanded = false;
	
	if (colliding.wallDown == false) {
		landed = false;

	}
	

	return true;
}


bool j2Player::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Player_Update", Profiler::Color::Aquamarine);
	if (dead == true)
	{
		landed = true;
		

		if (player.deadCounter < player.deadDelay)
		{
			player.deadCounter += 1;

		}
		else
		{


			if (DeathTime.Read() > 1000) {
				
				//Destroy the player Colliders

				player.playerHitbox->to_delete = true;
				player.fakeHitbox->to_delete = true;
				player.playerHitbox = nullptr;
				player.fakeHitbox = nullptr;
				AttackReset();
				//Player Goes To inital position of the current stage map
				
				if (App->scene->CurrentMap2 == false)
					App->render->camera.x = App->map->SetPlayerToInitial(App->map->data);

				else
					App->render->camera.x = App->map->SetPlayerToInitial(App->map->data2);


				dead = false;
			}

		/*	player.deadCounter = player_Init.deadCounter;*/
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
		CheckEntityMovement();
		//switchStates
		SwithcingStates(dt);
		//Switch the colliders shape depending of the state
		ColliderShapeStates();
		//Delete AttackCOlliders if its needed
		CheckCollidersAttacks();
		//players Effects
		 EntityFX();
		//movePlayer
		 EntityMovement(dt);
		
	
	//If the player falls and surpasses a determined Y position it dies
	if (App->scene->CurrentMap2 == false)
	{
		if (position.y > player.maximumDeadY_map1)
		{
			dead = true;
		}
	}
	else
	{
		if (position.y > player.maximumDeadY_map2)
		{
			dead = true;
		}
	}



	//Here we change the values of the rect position
	if (GodModeB == false
		&& player.playerHitbox != nullptr && player.playerHitbox->to_delete == false
		&& player.fakeHitbox != nullptr && player.fakeHitbox->to_delete == false)
	{
		player.playerHitbox->SetPos(position.x + colliding.colliderOffset.x, position.y + colliding.colliderOffset.y);
		player.fakeHitbox->SetPos(player.playerHitbox->rect.x -1 , player.playerHitbox->rect.y -1);
		
		if (player.PlayerAttackCollider != nullptr) {

			if (ChargedAttackB == true) {
			
				SetColliderRespectPivot(lookingRight,player.PlayerAttackCollider, position, player.ChargedAttackCollider.x, player.ChargedAttackCollider.y);
		
			} 
			else if (AirAttackB==true) {

				SetColliderRespectPivot(lookingRight, player.PlayerAttackCollider, position, player.AirAttackCollider.x, player.AirAttackCollider.y);

			}
			else if (BasicAttackB==true) {

				SetColliderRespectPivot(lookingRight, player.PlayerAttackCollider, position, player.BasicAttackCollider.x, player.BasicAttackCollider.y);


			}
	
		}
	}
	else if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
		player.playerGodModeHitbox->SetPos(position.x + colliding.colliderOffsetGodMode.x, position.y+ colliding.colliderOffsetGodMode.y);

	//App->collision->Update(dt);
	//App->collision->Update(dt);



	//Camera Following player
	App->render->followPlayer(player,dt);
	//AnimationsConditions

	AnimationRect = currentAnimation->GetCurrentFrame(dt);
	

	if (lookingRight) {
		App->render->Blit(EntityText, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(EntityText, position.x- PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}


	return true;

}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	BROFILER_CATEGORY("Player_PostUpdate", Profiler::Color::CadetBlue);
	// We reset the colliders collisions
	if (hurt == true) {
		ToMoveRight = false;
		ToMoveLeft = false;
	}

	colliding.wallFront = false;
	colliding.wallBack = false;
	colliding.wallDown = false;
	colliding.wallTop = false;
  
	


	return true;
}


void j2Player::OnCollision(Collider* c1, Collider* c2) 
{
	BROFILER_CATEGORY("Player_OnCollision", Profiler::Color::Cyan);
	//Testing new system of collisions
	SDL_Rect overlay; // SDL_Rect of the intersection between the 2 colliders

	SDL_IntersectRect(&c1->rect, &c2->rect, &overlay);
	
	
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
					&& colliding.wallTop == false)
				{
					colliding.wallFront = true;
					//Before we do anything else, don't allow the collider to enter the tile
					//player.playerHitbox->rect.x -= player.colliding.x_CollisionAdjuster;
					player.playerHitbox->rect.x -= overlay.w;
				}
				//Conditions to know if the collider that we collided with is Behind of the player
				else if (player.playerHitbox->rect.x < c2->rect.x + c2->rect.w
					&& player.playerHitbox->rect.x - c2->rect.x > 0
					&& c2->rect.y + 8 < player.playerHitbox->rect.y + player.playerHitbox->rect.h
					&& colliding.wallTop == false)
				{
					colliding.wallBack = true;
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
					landed = true;
					colliding.wallDown = true;

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

					landed = false;
					colliding.wallTop = true;
				}


			}
			//If the collider is a killing obstacle DIE
			if (dead == false) {
				if (c2->type == COLLIDER_TRAP)
				{
					DeathTime.Start();
					dead = true;
					PlayFXDie = true;
				}
			}
		}
		else if (c1->type == COLLIDER_PLAYER_CHECK)	//This collider is a +1 pixel margin of the player collision, so we can have data on what's on the top,right,left and under the player
		{
			if (overlay.x < c2->rect.x + c2->rect.w && overlay.x > c2->rect.x
				&& c2->rect.y + colliding.y_CollisionController < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
				colliding.wallBack = true;
			if (overlay.x + overlay.w > c2->rect.x  && overlay.x == c2->rect.x
				&& c2->rect.y + colliding.y_CollisionController < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
				colliding.wallFront = true;

			if (overlay.y + overlay.h > c2->rect.y
				&& overlay.y + overlay.h < c2->rect.y + c2->rect.h
				&& player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x
				&& c2->rect.x + c2->rect.w > player.playerHitbox->rect.x)
			{
				landed = true;
				colliding.wallDown = true;
			}

			//if (overlay.y < overlay.y + overlay.h
			//	/*&& player.fakeHitbox->rect.y + player.fakeHitbox->rect.h > overlay.y + overlay.h*/
			//	&& player.playerHitbox->rect.x + player.playerHitbox->rect.w < c2->rect.x
			//	&& c2->rect.x + c2->rect.w < player.playerHitbox->rect.x)
			if (player.fakeHitbox->rect.y > c2->rect.y
				&& player.fakeHitbox->rect.y < c2->rect.y + c2->rect.h
				&& player.fakeHitbox->rect.x + player.fakeHitbox->rect.w - 5> c2->rect.x
				&& c2->rect.x + c2->rect.w - 5 > player.fakeHitbox->rect.x)
			{
				colliding.wallTop = true;
			}
		}

		//At the end put the player pos onto the collider Pos 
		if (player.playerHitbox != nullptr)
		{
			position.x = player.playerHitbox->rect.x - colliding.colliderOffset.x;
			position.y = player.playerHitbox->rect.y - colliding.colliderOffset.y;
		}

		//player.lateralFakeHitbox->rect.y = player.playerHitbox->rect.y -1;

		/*player.lateralFakeHitbox->rect.y = player.playerHitbox->rect.y;
		player.lateralFakeHitbox->rect.x = player.playerHitbox->rect.x -1;*/


		if (dead == false && hurt==false) {
			if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY)
			{
				HurtTime.Start();
				hurt = true;
				HitsToRecive--;
				App->fade->FadeCustom(255,0,0,30.0f,0.1f);
			
				if (c2->rect.x > c1->rect.x)
					Speed.x = -Currentacceleration;
				else if(c2->rect.x < c1->rect.x)
					Speed.x = Currentacceleration;

				else if (MovingRight == false && MovingLeft == false) {
					if(lookingRight==true)
						Speed.x = -Currentacceleration;
					else
						Speed.x = Currentacceleration;


				}

				if (HitsToRecive == 0) {
					DeathTime.Start();
					dead = true;
					PlayFXDie = true;
					HitsToRecive = FixedHits;
				}

			}
		}
} 

void  j2Player::PlayerMovementInputs() {


	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		ToMoveRight = false;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && colliding.wallFront == false)
	{
		ToMoveRight = true;

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE )
	{
		ToMoveLeft = false;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && colliding.wallBack == false)
	{
		ToMoveLeft = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		ToMoveUp = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && hurt == false)
	{
		ToMoveUp = true;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		ToMoveDown = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && hurt == false)
	{
		ToMoveDown = true;
	}
	



}



//void j2Player::CheckEntityMovement(){
//
//	if (Speed.x > 0.0f) {
//		MovingRight = true;
//		MovingLeft = false;
//	}
//	else if (Speed.x < 0.0f) {
//		MovingLeft = true;
//		MovingRight = false;
//	}
//	else if (Speed.x == 0.0f) {
//		MovingLeft = false;
//		MovingRight = false;
//	}
//
//	if (landed == true)
//	{
//		Speed.y = 0.0f;
//	}
//	else if (Speed.y < 0.0f) {
//		MovingUp = true;
//		MovingDown = false;
//	}
//	else if (Speed.y > 0.0f) {
//		MovingDown = true;
//		MovingUp = false;
//	}
//	else if (Speed.y == 0.0f) {
//		MovingUp = false;
//		MovingDown = false;
//	}
//
//}

void j2Player::SwithcingStates(float dt) {
	if (dead == false) {
		
		if (hurt == true && HurtTime.Read() > 500) {

			hurt = false;

		}
		
		
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
}

void j2Player::IdleStateTo(float dt) {

	


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
		else if (ToMoveDown == true && landed==true) {
			CurrentState = Player_State::CROUCHING;
			
		}
	

		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN ) {
			
			PlayFxSwordSwing = true;
			BasicAttackB = true;
			EntityAttacks(dt);

		}
	
		

	

}
void j2Player::CrouchingStateTo(float dt) {

	if (landed != true) {

		CurrentState = Player_State::AIR;

	}
	
	if (ToMoveDown == false) {
		
		if (ToMoveRight == true || ToMoveLeft == true || MovingRight == true || MovingLeft == true)
			CurrentState = Player_State::RUNNING;
		else if(ToMoveRight ==false && ToMoveLeft== false && ToMoveUp==false && ToMoveDown==false && landed==true ) {
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

		PlayFxSwordSwing = true;
		BasicAttackB = true;
		EntityAttacks(dt);


	}

}
void j2Player::RunningStateTo(float dt) {

	if (landed != true) {

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

		playFxSwordStrongSwing = true;
		ChargedAttackB = true;
		EntityAttacks(dt);


	}
	else if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && (Speed.x != Maxspeed.x || Speed.x != -Maxspeed.x)) {

		PlayFxSwordSwing = true;
		BasicAttackB = true;
		EntityAttacks(dt);


	}
}
void j2Player::AirStateTo(float dt) {

	//Double jump is true when the doublejump is used, but if this one is false is that it has not been used

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.doubleJump == false && FirstJump == true && landed == false) {
		
		FirstJump = false;
		//playconditon sound
		playeFXDoublejump = true;
		//Reset Animation
		jumpDouble.Reset();

		//Doublejump
		Speed.y =-JumpForce;
		player.doubleJump = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && landed==false && MovingUp) {

		if (arealAttackUsed == false) {
			PlayFxSwordSwing = true;
			AirAttackB = true;
			EntityAttacks(dt);
		}

	}

	if (landed==true ) {		

		//land
		FirstJump == false;
		jump.Reset();
		player.doubleJump = false;

	
		if (dead == false) {
			
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

	

		if (Guard.Read() >= 1200 && BasicAttackB == true) {

			
				AttackReset();
		}

		if (landed != true && (ChargedAttackB == true || BasicAttackB == true)) {

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

void j2Player::EntityAttacks(float dt) {


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

void j2Player::EntityMovement(float dt) {
	
	if (dead == false) {

		if (GodModeB == false) {

			if (ToMoveRight == true && ToMoveLeft == false && colliding.wallFront == false && ChargedAttackB == false && BasicAttackB==false) {
				Speed.x += Currentacceleration*dt;
			}
			else if (ToMoveLeft == true && ToMoveRight == false && colliding.wallBack == false && ChargedAttackB == false && BasicAttackB == false) {
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

		
			


			if (CurrentState == Player_State::AIR && landed==false || CurrentState == Player_State::ATTACK && AirAttackB==true) {
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
			position.x += Speed.x*dt;
			position.y += Speed.y*dt;

		}

		if (GodModeB == true)
		{
			//If GodMode Activated, move around FREELY 
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				Speed.y = -Maxspeed.y;
				position.y += Speed.y*dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				Speed.y = +Maxspeed.y;
				position.y += Maxspeed.y*dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				Speed.x = +Maxspeed.x;
				position.x += Maxspeed.x*dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				Speed.x = -Maxspeed.x;
				position.x += -Maxspeed.x*dt;
			}

		}
	}

}


void j2Player::EntityFX() {

	if (dead == false) {
			if(hurt==false){

		if (ToMoveRight == true && ToMoveLeft == false) {
			lookingRight = true;
		}
		else if (ToMoveLeft == true && ToMoveRight == false) {
			lookingRight = false;
		}



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
			else{

				Hurted.Reset();
				currentAnimation = &Hurted;

			}

}

	else{
		if (PlayFXDie == true) {
			
			App->audio->PlayFx(player_fx.dieSound, 0);
			PlayFXDie = false;

			if (CurrentState != Player_State::AIR) {
				die.Reset();
				currentAnimation = &die;
				}
			else {

				dieMidAir.Reset();
				currentAnimation = &dieMidAir;

			}

			 }
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

		if ((colliding.wallFront == true || colliding.wallBack == true ) && ChargedAttackB==false) {

			currentAnimation = &push;
		}
		
	/*	else if (BasicAttackB == true) {

			currentAnimation = &BasicAttack;
		}*/

		else {

		//	App->audio->PlayFx(player_fx.runningSound, 0);
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

			if (PlayFxSwordSwing == true) {
				App->audio->PlayFx(player_fx.SlashSwordSound, 0);
				PlayFxSwordSwing = false;
			}
			currentAnimation = &AirAttack;
		}
		else if (ChargedAttackB == true) {

			if (playFxSwordStrongSwing == true) {

				App->audio->PlayFx(player_fx.StrongSlashSound, 0);

				playFxSwordStrongSwing = false;
			
			}
			currentAnimation = &ChargedAttack;
		
		}
		else if (BasicAttackB == true) {
			
			if (PlayFxSwordSwing == true) {
			
				App->audio->PlayFx(player_fx.SlashSwordSound, 0);
				
				PlayFxSwordSwing = false;
			}		

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
				player.playerHitbox = nullptr;
				player.fakeHitbox = nullptr;
				player.playerGodModeHitbox = App->collision->AddCollider(EntityRect, COLLIDER_GODMODE,App->entities);
			}
			else
			{

				player.playerGodModeHitbox->to_delete = true;
				player.playerGodModeHitbox = nullptr;
				player.playerHitbox = App->collision->AddCollider(EntityRect, COLLIDER_PLAYER, App->entities);
				player.fakeHitbox = App->collision->AddCollider(player.fakeCollisionRect, COLLIDER_PLAYER_CHECK, App->entities);
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
			p.playerGodModeHitbox = App->collision->AddCollider(EntityRect,COLLIDER_GODMODE, App->entities);
			p.playerHitbox = nullptr;
			p.fakeHitbox = nullptr;

			ret = 1;
		}
	}
	else
	{
		if (p.playerHitbox == nullptr && p.fakeHitbox == nullptr)
		{
			p.fakeHitbox = App->collision->AddCollider(p.fakeCollisionRect, COLLIDER_PLAYER_CHECK, App->entities);
			p.playerHitbox = App->collision->AddCollider(EntityRect, COLLIDER_PLAYER, App->entities);
			p.playerGodModeHitbox = nullptr;
			ret = 1;
		}
	}

	return ret;
}


void j2Player::ColliderShapeStates() {

	if (player.playerHitbox != nullptr) {
		if (colliding.wallFront==false || colliding.wallBack==false) {
			
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
}

void j2Player::IdleColliderShape() {

	player.playerHitbox->rect.w = EntityRect.w;
	player.playerHitbox->rect.h = EntityRect.h;
	player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
	player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;
	colliding.colliderOffset.y =colliding.collisionOffsetY;

}

void j2Player::CrouchColliderShape() {

	player.playerHitbox->rect.w = player.playerRectCrouched.w;
	player.playerHitbox->rect.h = player.playerRectCrouched.h;
	colliding.colliderOffset.y = (EntityRect.h - player.playerRectCrouched.h) + colliding.collisionOffsetY;
	player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
	player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

}

void j2Player::JumpColliderShape() {

	if (FirstJump == true) {
	
		player.playerHitbox->rect.w = player.playerRectJump.w;
		player.playerHitbox->rect.h = player.playerRectJump.h;
		colliding.colliderOffset.y = (EntityRect.h - player.playerRectJump.h) + colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

	}
	else if (player.doubleJump == true) {


		player.playerHitbox->rect.w = player.playerRectDJump.w;
		player.playerHitbox->rect.h = player.playerRectDJump.h;
	colliding.colliderOffset.y = (EntityRect.h - player.playerRectDJump.h) + colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

	}

}

void j2Player::RunnigColliderShape() {


	player.playerHitbox->rect.w = player.PlayerRectRunning.w;
	player.playerHitbox->rect.h = player.PlayerRectRunning.h;
colliding.colliderOffset.y = (EntityRect.h - player.PlayerRectRunning.h) + colliding.collisionOffsetY;
	player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
	player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;

}

void j2Player::ChargedAttackColliderShape() {

	if (ChargedAttackB == true) {
		player.playerHitbox->rect.w = player.PlayerRectAttackCharged.w;
		player.playerHitbox->rect.h = player.PlayerRectAttackCharged.h;
	colliding.colliderOffset.y = (EntityRect.h - player.PlayerRectAttackCharged.h) + colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;
	}

	else if (AirAttackB == true) {
		player.playerHitbox->rect.w = player.PlayerRectAttackAir.w;
		player.playerHitbox->rect.h = player.PlayerRectAttackAir.h;
		colliding.colliderOffset.y = (EntityRect.h - player.PlayerRectAttackAir.h) + colliding.collisionOffsetY;
		player.fakeHitbox->rect.h = player.playerHitbox->rect.h + 2;
		player.fakeHitbox->rect.y = player.playerHitbox->rect.y - 1;
	}

}

void j2Player::CollidersAttacks() {


	if (player.PlayerAttackCollider == nullptr) {

		if (ChargedAttackB == true) {

			player.PlayerAttackCollider = App->collision->AddCollider(player.ChargedAttackCollider, COLLIDER_PLAYER_ATTACK, App->entities);
		}


		else if (AirAttackB == true) {

			player.PlayerAttackCollider = App->collision->AddCollider(player.AirAttackCollider, COLLIDER_PLAYER_ATTACK, App->entities);

		}

		else if (BasicAttackB == true) {

			player.PlayerAttackCollider = App->collision->AddCollider(player.BasicAttackCollider, COLLIDER_PLAYER_ATTACK, App->entities);
			
		}
	}
}
	

void j2Player::CheckCollidersAttacks() {
	
	if (player.PlayerAttackCollider != nullptr) {
	
		if (ChargedAttackB == false && player.DeleteColliderChargeA==true || dead==true) {
			player.PlayerAttackCollider->to_delete = true;
			player.PlayerAttackCollider = nullptr;
			player.DeleteColliderChargeA = false;
		}
		else if (AirAttackB==false && player.DeleteColliderAirA==true || dead == true) {
		
			player.PlayerAttackCollider->to_delete = true;
			player.PlayerAttackCollider = nullptr;
			player.DeleteColliderAirA = false;

		}
		else if (BasicAttackB==false && player.DeleteColliderBasicA == true || dead == true) {

			player.PlayerAttackCollider->to_delete = true;
			player.PlayerAttackCollider = nullptr;
			player.DeleteColliderBasicA = false;

		}
	}
}
void j2Player::AttackReset() {

	BasicAttack.Reset();
	BasicAttackB = false;
	CurrentState = Player_State::IDLE;


}
