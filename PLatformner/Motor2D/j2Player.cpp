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


//CONSTRUCTOR
j2Player::j2Player()
{
	name.create("player");

	pugi::xml_parse_result result =configAnim.load_file("Animations.xml");


	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("idle");//idle
	idle.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("run");//run
	run.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("jump");//jump
	jump.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("jumpDouble");//DoubleJump
	jumpDouble.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("fall");//fall
	fall.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("crouch");//crouch
	crouch.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("push");//push
	push.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("attack");//Charged attack
	ChargedAttack.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("slide");//slide
	slide.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("die");//die
	die.LoadPushBack(AnimPushBack);
	
	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("GodMode");//GodMode
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

		//Player Speeds
		JumpForce = config.child("Jumpforce").attribute("value").as_float();
		Currentacceleration = config.child("Currentacceleration").attribute("value").as_float();
		gravity = config.child("gravity").attribute("value").as_float();
		Maxspeed.x =config.child("Maxspeed").attribute("x").as_float();
		Maxspeed.y = config.child("Maxspeed").attribute("y").as_float();

		//Player DoubleJump
		player_Init.doubleJump = config.child("doubleJump").attribute("value").as_bool();
		

		//Player collider Control
		player_Init.colliding.wallFront = config.child("collisionControlcolliding").attribute("wallFront").as_bool();
		player_Init.colliding.wallBack = config.child("collisionControlcolliding").attribute("wallBack").as_bool();
		player_Init.colliding.wallDown = config.child("collisionControlcolliding").attribute("wallDown").as_bool();
		player_Init.colliding.wallTop = config.child("collisionControlcolliding").attribute("wallTop").as_bool();
		//PlayerCollision Adjusters
		player_Init.colliding.x_CollisionAdjuster = config.child("xCollisionAdjuster").attribute("x").as_int();
		player_Init.colliding.y_CollisionController = config.child("vCollision_controller").attribute("y").as_int();

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

		//Player landed
		player_Init.landed = config.child("landed").attribute("value").as_bool();

		//Player Death
		player_Init.dead = config.child("dead").attribute("boolDead").as_bool();
		player_Init.deadDelay = config.child("dead").attribute("deadDelay").as_int();
		player_Init.deadCounter = config.child("dead").attribute("deadCounter").as_int();
		player_Init.maximumDeadY_map1 = config.child("maximumDead_Y").attribute("map1").as_int();
		player_Init.maximumDeadY_map2 = config.child("maximumDead_Y").attribute("map2").as_int();

		//Player Godmode
		player_Init.godMode = config.child("godMode").attribute("value").as_bool();
	
		player.playerHitbox = nullptr;
		player.playerGodModeHitbox = nullptr;
		player.fakeHitbox = nullptr;


		//player SoundFX
		jumpSound.path=config.child("FX").child("jump").attribute("path").as_string();
		runningSound.path=config.child("FX").child("run").attribute("path").as_string();
		DoublejumpSound.path= config.child("FX").child("jumpDouble").attribute("path").as_string();
		DieSound.path= config.child("FX").child("die").attribute("path").as_string();
	}
	else 
	{
		LOG("Could not Load Player data on Awake!");
	}

	return ret;
}

// Load Game State
bool j2Player::Load(pugi::xml_node& data)
{
	player.playerPos.x = data.child("playerPos").attribute("x").as_int();
	player.playerPos.y = data.child("playerPos").attribute("y").as_int();

	player.landed = data.child("landed").attribute("value").as_bool();


	player.dead = data.child("dead").attribute("value").as_bool();
	player.deadCounter = data.child("dead").attribute("deadCounter").as_int();

	if (player.godMode == false)
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

	playerSave = data.append_child("landed");
	playerSave.append_attribute("value") = player.landed;

	playerSave = data.append_child("speeds");
	//playerSave.append_attribute("y_speed") = player.y_speed;

	playerSave = data.append_child("dead");
	playerSave.append_attribute("value") = player.dead;
	playerSave.append_attribute("deadCounter") = player.deadCounter;

	return true;
}


bool j2Player::Start()
{
	LOG("Player Start");
	
	//Load all data from player_Init into the player we will be using
	//Player Position
	player.playerPos = player_Init.playerPos;
	//Player SDL_Rect
	player.playerRect = player_Init.playerRect;
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



	player.fakeCollisionRect = { player.playerRect.x - 1, player.playerRect.y - 1, player.playerRect.w + 2, player.playerRect.h + 2 };
	

	CreatePlayerColliders(player);

	
	
	playTex = App->tex->Load(folder.GetString());//loading Player textures

	//Loading SoundEffects
	jumpSound.ChunkSize = App->audio->LoadFx(jumpSound.path.GetString());
	runningSound.ChunkSize= App->audio->LoadFx(runningSound.path.GetString());
	DoublejumpSound.ChunkSize= App->audio->LoadFx(DoublejumpSound.path.GetString());
	DieSound.ChunkSize= App->audio->LoadFx(DieSound.path.GetString());

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
	//PREUPDATE is called before any On Collision or Pre-Collision from the player is called
	// so we set vars like landed to false and in case we get a call back that the player is landed it will be changed in said functions.
	player.nextFrameLanded = false;
	
	
	return true;
}


bool j2Player::Update(float dt)
{
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
	
	

		//Check inputs
		PlayerMovementInputs();
		//DebugFuncionalities
		PlayerDebugF();
		//CheckMovement
		CheckPlayerMovement();
		//switchStates
		SwithcingStates();
		//players Effects
		PlayerFX();
		//movePlayer
		PlayerMovement();
	
	
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



	//Camera Following player
	App->render->followPlayer(player);

	//Here we change the values of the rect position
	if (player.godMode == false
		&& player.playerHitbox != nullptr && player.playerHitbox->to_delete == false
		&& player.fakeHitbox != nullptr && player.fakeHitbox->to_delete == false)
	{
		player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);
		player.fakeHitbox->SetPos(player.playerHitbox->rect.x -1, player.playerHitbox->rect.y -1);
	}
	else if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
		player.playerGodModeHitbox->SetPos(player.playerPos.x, player.playerPos.y);

	//App->collision->Update(dt);
	//App->collision->Update(dt);




//AnimationsConditions


AnimationRect = currentAnimation->GetCurrentFrame();

	if (lookingRight) {
		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}


	return true;
		
}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	// We reset the colliders collisions
	player.colliding.wallFront = false;
	player.colliding.wallBack = false;
	player.colliding.wallDown = false;
	player.colliding.wallTop = false;

	player.landed = false;

	//	//Here we change the values of the rect position
	//if(player.playerHitbox != nullptr && player.playerHitbox->to_delete == false)
	//player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);
	//if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
	//player.playerGodModeHitbox->SetPos(player.playerPos.x, player.playerPos.y);

	
	

	return true;
}


void j2Player::OnCollision(Collider* c1, Collider* c2) 
{
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
				&& c2->rect.y + 8< player.playerHitbox->rect.y + player.playerHitbox->rect.h
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
	player.playerPos.x = player.playerHitbox->rect.x;
	player.playerPos.y = player.playerHitbox->rect.y;

	//player.lateralFakeHitbox->rect.y = player.playerHitbox->rect.y -1;
	
	/*player.lateralFakeHitbox->rect.y = player.playerHitbox->rect.y;
	player.lateralFakeHitbox->rect.x = player.playerHitbox->rect.x -1;*/

} 

void j2Player::OnPreCollision(int d) 
{
	/*player.d_to_ground = d;
	player.nextFrameLanded = true;*/
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

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
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

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
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

void j2Player::SwithcingStates() {

	switch (CurrentState) {
	case Player_State::IDLE:
		IdleStateTo();
		break;
	case Player_State::CROUCHING:
		CrouchingStateTo();
		break;
	case  Player_State::RUNNING:
		RunningStateTo();
		break;
	case  Player_State::AIR:
		AirStateTo();
		break;
	
	}


}

void j2Player::IdleStateTo() {

	if (player.dead == false) {
		if (ToMoveRight == true && ToMoveLeft == false || ToMoveLeft == true && ToMoveRight == false) {
			CurrentState = Player_State::RUNNING;
		}
		else if (ToMoveUp == true) {
			//jump
			PlayFXJump = true;
			Speed.y = -JumpForce;

			player.landed = false;
			CurrentState = Player_State::AIR;
		}
		else if (ToMoveDown == true) {
			CurrentState = Player_State::CROUCHING;
		}
	}

}
void j2Player::CrouchingStateTo() {

	if (ToMoveDown == false) {
		if (ToMoveRight == true || ToMoveLeft == true || MovingRight == true || MovingLeft == true)
			CurrentState = Player_State::RUNNING;
		else {
			CurrentState = Player_State::IDLE;
		}
	}
	else if (ToMoveUp == true) {
		//jump
		PlayFXJump = true;
		Speed.y = -JumpForce;

		player.landed = false;
		CurrentState = Player_State::AIR;
	}


}
void j2Player::RunningStateTo() {

	if (ToMoveUp == true) {
		//jump
		PlayFXJump = true;
		Speed.y = -JumpForce;

		player.landed = false;
		CurrentState = Player_State::AIR;
	}
	else if (MovingLeft == false && MovingRight == false) {
		if (ToMoveRight == false && ToMoveLeft == false || ToMoveRight == true && ToMoveLeft == true) {
			CurrentState = Player_State::IDLE;
		}
	}

}
void j2Player::AirStateTo() {

	//Double jump is true when the doublejump is used, but if this one is false is that it has not been used

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.doubleJump == false ) {
		
		//playconditon sound
		playeFXDoublejump = true;
		//Reset Animation
		jumpDouble.Reset();
		//Doublejump
		Speed.y =-JumpForce;
		player.doubleJump = true;
	}

	if (player.landed) {		

		//land
		Speed.y = 0.0f;
		jump.Reset();
		player.doubleJump = false;

	
		if (player.dead == false) {
			
			if (ToMoveRight == true || ToMoveLeft == true || MovingRight == true || MovingLeft == true) {
				if (ToMoveDown == false) {
					CurrentState = Player_State::RUNNING;
				}
			
			}
			else if (ToMoveDown == true) {
				CurrentState = Player_State::CROUCHING;
			}
			else {
				CurrentState = Player_State::IDLE;
			}
		}
	}
}

void j2Player::PlayerMovement() {
	
	if (player.dead == false) {

		if (player.godMode == false) {

			if (ToMoveRight == true && ToMoveLeft == false && player.colliding.wallFront == false) {
				Speed.x += Currentacceleration;
			}
			else if (ToMoveLeft == true && ToMoveRight == false && player.colliding.wallBack == false) {
				Speed.x -= Currentacceleration;
			}
			else if (CurrentState != Player_State::AIR) {	
				if (MovingRight == true) {
					Speed.x -= Currentacceleration;

					if (Speed.x < 0.0f)
						Speed.x = 0.0f;
				}
				else if (MovingLeft == true) {
					Speed.x += Currentacceleration;

					if (Speed.x > 0.0f)
						Speed.x = 0.0f;
				}
			}

			
			if ((CurrentState == Player_State::AIR || CurrentState == Player_State::RUNNING || CurrentState == Player_State::IDLE || CurrentState == Player_State::CROUCHING) && !player.landed) {
				//Falling
				Speed.y += gravity;
			}

			// Maximum Speeds
			if (Speed.x > Maxspeed.x)
				Speed.x = Maxspeed.x;
			else if (Speed.x < -Maxspeed.x)
				Speed.x = -Maxspeed.x;

			if (Speed.y > Maxspeed.y)
				Speed.y = Maxspeed.y;
			else if (Speed.y < -Maxspeed.y)
				Speed.y = -Maxspeed.y;


			//new current position
			player.playerPos.x += Speed.x;
			player.playerPos.y += Speed.y;

		}

		if (player.godMode == true)
		{
			//If GodMode Activated, move around FREELY 
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				player.playerPos.y -= Maxspeed.y;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				player.playerPos.y += Maxspeed.y;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				player.playerPos.x += Maxspeed.x;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				player.playerPos.x += -Maxspeed.x;

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

		if (player.godMode == false) {

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
			}
		}
		else {

			currentAnimation = &GodMode;

		}
	}
	else {
		

	
		if (PlayFXDie == true) {
			App->audio->PlayFx(DieSound.ChunkSize, 0);
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

		if (player.colliding.wallFront == true || player.colliding.wallBack == true) {

			currentAnimation = &push;
		}

		else {
		//	App->audio->PlayFx(runningSound.ChunkSize, 0);
			currentAnimation = &run;
		}
	}
	void j2Player::AirFX() {
	
		if (MovingDown == true) {
		
			currentAnimation = &fall;
		}
		else if (player.doubleJump == true) {
		
			if (playeFXDoublejump == true) {
				App->audio->PlayFx(DoublejumpSound.ChunkSize, 0);
				playeFXDoublejump = false;
			}

			currentAnimation = &jumpDouble;
		}
		else {
			if (PlayFXJump == true) {
				App->audio->PlayFx(jumpSound.ChunkSize, 0);
				PlayFXJump = false;
			}
			currentAnimation = &jump;
		}
	}

	void j2Player::PlayerDebugF() {

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

			player.godMode = !player.godMode;

			if (player.godMode == true)
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
	if (p.godMode == true)
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


