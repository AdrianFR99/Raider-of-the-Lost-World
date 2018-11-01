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



//CONSTRUCTOR
j2Player::j2Player()
{
	name.create("player");

	pugi::xml_parse_result result =configAnim.load_file("Animations.xml");


	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("idle");//idle
	player.animations.idle.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("run");//run
	player.animations.run.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("jump");//jump
	player.animations.jump.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("jumpDouble");//DoubleJump
	player.animations.jumpDouble.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("slide");//slide
	player.animations.jumpDouble.LoadPushBack(AnimPushBack);

	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("die");//die
	player.animations.die.LoadPushBack(AnimPushBack);
	
	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("GodMode");//GodMode
	player.animations.GodMode.LoadPushBack(AnimPushBack);
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
		player_Init.x_speed = config.child("x_speed").attribute("value").as_int();
		player_Init.y_speed = config.child("y_speed").attribute("value").as_int();
		player_Init.gravity_speed = config.child("gravity_speed").attribute("value").as_int();
		player_Init.actual_x_speed = config.child("actual_x_speed").attribute("value").as_int();
		player_Init.actual_y_speed = config.child("actual_y_speed").attribute("value").as_int();
		player_Init.stopped_speed = config.child("stopped_speed").attribute("value").as_int();
		player_Init.y_max_speed = config.child("y_max_speed").attribute("value").as_int();
		//Player DoubleJump
		player_Init.doubleJump = config.child("doubleJump").attribute("value").as_bool();
		player_Init.doubleJump_counter = config.child("doubleJump_counter").attribute("value").as_int();
		player_Init.doubleJump_delay = config.child("doubleJump_delay").attribute("value").as_int();

		//Player collider Control
		player_Init.colliding.wallFront = config.child("collisionControlcolliding").attribute("wallFront").as_bool();
		player_Init.colliding.wallBack = config.child("collisionControlcolliding").attribute("wallBack").as_bool();
		player_Init.colliding.wallDown = config.child("collisionControlcolliding").attribute("wallDown").as_bool();
		player_Init.colliding.wallTop = config.child("collisionControlcolliding").attribute("wallTop").as_bool();
		//PlayerCollision Adjusters
		player_Init.colliding.x_CollisionAdjuster = config.child("xCollisionAdjuster").attribute("x").as_int();
		player_Init.colliding.y_CollisionController = config.child("vCollision_controller").attribute("y").as_int();

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

	player.y_speed = data.child("speeds").attribute("y_speed").as_int();

	player.dead = data.child("dead").attribute("value").as_bool();
	player.deadCounter = data.child("dead").attribute("deadCounter").as_int();


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
	playerSave.append_attribute("y_speed") = player.y_speed;

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
	player.x_speed = player_Init.x_speed;
	player.y_speed = player_Init.actual_y_speed;
	player.y_max_speed = player_Init.y_max_speed;
	player.gravity_speed = player_Init.gravity_speed;

	player.actual_x_speed = player_Init.actual_x_speed;
	player.actual_y_speed = player_Init.actual_y_speed;
	player.stopped_speed = player_Init.stopped_speed;
	player.doubleJump = player_Init.doubleJump;
	player.doubleJump_counter = player_Init.doubleJump_counter;
	player.doubleJump_delay = player_Init.doubleJump_delay;
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

	if(player.playerHitbox==nullptr)
	player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER, this);
	
	
	playTex = App->tex->Load(folder.GetString());//loading Player textures

	
	player.idle_Bool_Right =true;


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
	/*player.playerRect.x = player.playerHitbox->rect.x;
	player.playerRect.y = player.playerHitbox->rect.y;

	player.playerPos = { player.playerRect.x , player.playerRect.y };
	player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);*/

	if (player.dead == true)
	{
		if (player.deadCounter < player.deadDelay)
		{
			player.deadCounter += 1;
		}
		else
		{
			//Player Goes To inital position of the current stage map
			if (App->scene->CurrentMap2 == false)
				App->render->camera.x = App->map->SetPlayerToInitial(App->map->data);

			else
				App->render->camera.x = App->map->SetPlayerToInitial(App->map->data2);

			player.dead = false;
			player.deadCounter = player_Init.deadCounter;
		}
	}
	else 
	{
		//If the player is alive
		if (player.playerHitbox->type != COLLIDER_PLAYER && player.godMode == false)
		{
				player.playerRect = player_Init.playerRect;
			
				player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER, this);
		}
	
		  
		//Control X speed
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player.colliding.wallFront == false)
		{

			player.run_Bool_Right = true;
			player.playerPos.x += player.x_speed;
			
			player.GodMode_Left = false;
       
			
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player.colliding.wallBack == false)
		{
			//player.run_Bool_Left = true;
			player.playerPos.x -= player.x_speed;

			player.GodMode_Left = true;


		}

		//GODMODE LOGIC
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			player.godMode = !player.godMode;
			if (player.godMode == true)
			{
				player.idle_Bool_Right = false;
				player.playerHitbox->to_delete = true;
				player.playerGodModeHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_GODMODE, this);
			}
			else
			{
				player.idle_Bool_Right = true;
				player.playerGodModeHitbox->to_delete = true;
				player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER, this);
			}
		}

		if (player.godMode == true)
		{
			//If GodMode Activated, move around FREELY (X vlues aren't affected)
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				player.playerPos.y -= player.y_max_speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				player.playerPos.y += player.y_max_speed;
		}
		else
		{

			//LANDED LOGIC:  when landed == false the player 
			//is not touching a solid surface with its feet
			//when landed == true the player IS touching a solid surface with its feet

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.colliding.wallTop == false
				&& player.landed == true)
			{
				player.jump_Bool = true;
				player.landed = false;
				player.y_speed = player_Init.y_speed;
			}

			if (player.landed == false)
			{
				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.colliding.wallTop == false
					&& player.doubleJump == true && player.doubleJump_counter > player.doubleJump_delay)
					// if the player isn't landed and at least doubleJump_counter is bigger than doubleJump_delay frames
					//This was done to avoid that the player performs accidentally a doubleJump right after performing the first one 
				{
					
					player.doubleJump = false;
					player.y_speed = player_Init.y_speed;
				

				}
				
				player.playerPos.y += player.y_speed;
				
				player.y_speed += player.gravity_speed;


				if (player.y_speed > player.y_max_speed)
				{
					player.y_speed = player.y_max_speed;
				}
				player.doubleJump_counter += 1; // We increase the double Jump counter, as we're not on the ground
			}
			else
			{
				player.doubleJump = true;
				player.doubleJump_counter = player_Init.doubleJump_counter;
			}
		}
	}

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
	if (player.playerHitbox != nullptr && player.playerHitbox->to_delete == false)
		player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);
	if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
		player.playerGodModeHitbox->SetPos(player.playerPos.x, player.playerPos.y);

	//App->collision->Update(dt);
	//App->collision->Update(dt);







//AnimationsConditions

	//idle
	if (player.idle_Bool_Right && !player.run_Bool_Left && !player.run_Bool_Right && !player.jump_Bool  && !player.dead) {

		player.animations.currentAnimation = &player.animations.idle;
		
		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect);

	}
	//run right
	if (player.idle_Bool_Right && !player.run_Bool_Left && player.run_Bool_Right && !player.jump_Bool  && !player.dead && player.landed)
	{

		player.animations.currentAnimation = &player.animations.run;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect);


	}

	//run left
	if (player.idle_Bool_Right && player.run_Bool_Left && !player.run_Bool_Right && !player.jump_Bool && !player.dead && player.landed) {
	
		player.animations.currentAnimation = &player.animations.run;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect,SDL_FLIP_HORIZONTAL);
	
	}
	//Jump &Run to right and Jump
	if ((player.idle_Bool_Right && !player.run_Bool_Left && !player.run_Bool_Right && player.jump_Bool &&  !player.dead )||
		(player.idle_Bool_Right && !player.run_Bool_Left && player.run_Bool_Right && player.jump_Bool &&  !player.dead ))
	
	{

		player.animations.currentAnimation = &player.animations.jump;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect, SDL_FLIP_NONE);

	}
	//Run to left and Jump
	if (player.idle_Bool_Right && player.run_Bool_Left && !player.run_Bool_Right && player.jump_Bool &&  !player.dead )

	{

		player.animations.currentAnimation = &player.animations.jump;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect, SDL_FLIP_HORIZONTAL);

	}


	if (player.dead) {

		player.animations.currentAnimation = &player.animations.die;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect);




	}


	if (player.godMode && !player.idle_Bool_Right && !player.GodMode_Left) {


		player.animations.currentAnimation = &player.animations.GodMode;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect);


	}

	if (player.godMode && !player.idle_Bool_Right && player.GodMode_Left) {


		player.animations.currentAnimation = &player.animations.GodMode;

		AnimationRect = player.animations.currentAnimation->GetCurrentFrame();

		App->render->Blit(playTex, player.playerPos.x, player.playerPos.y, &AnimationRect,SDL_FLIP_HORIZONTAL);


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

	//player.landed = false;

	//	//Here we change the values of the rect position
	//if(player.playerHitbox != nullptr && player.playerHitbox->to_delete == false)
	//player.playerHitbox->SetPos(player.playerPos.x, player.playerPos.y);
	//if (player.playerGodModeHitbox != nullptr && player.playerGodModeHitbox->to_delete == false)
	//player.playerGodModeHitbox->SetPos(player.playerPos.x, player.playerPos.y);

	
	player.run_Bool_Right = false;
	player.run_Bool_Left = false;


	if (player.landed) {
		player.jump_Bool = false;
	}
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
	
	if (c2->type == COLLIDER_WALL
		|| c2->type == COLLIDER_ICE
		|| c2->type == COLLIDER_PLATFORM
		|| c2->type == COLLIDER_CLIMBWALL)
	{
		//Conditions to know if the collider that we collided with is in Front of the player
		if (player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x
			&& c2->rect.x - player.playerHitbox->rect.x > 0
			&& c2->rect.y + player.colliding.y_CollisionController < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
		{
			player.colliding.wallFront = true;
			//Before we do anything else, don't allow the collider to enter the tile
			//player.playerHitbox->rect.x -= player.colliding.x_CollisionAdjuster;
			player.playerHitbox->rect.x -= overlay.w;
		}
		//Conditions to know if the collider that we collided with is Behind of the player
		else if (player.playerHitbox->rect.x < c2->rect.x + c2->rect.w
			&& player.playerHitbox->rect.x - c2->rect.x > 0
			&& c2->rect.y + player.colliding.y_CollisionController < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
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
			&& player.y_speed > 0)
			//player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x)
		{
			player.landed = true;
			player.colliding.wallDown = true;

			player.playerHitbox->rect.y -= overlay.h;
			//player.playerHitbox->type-= 
		}
		//Conditions to know if the collider that we collided with is over the player
		//Also, if the collider is a PLATFORM, let us go through it
		else if (player.playerHitbox->rect.y > c2->rect.y + c2->rect.h && c2->type != COLLIDER_PLATFORM)
		{
			player.y_speed = -1; // change the speed to inmediately falling (bouncing off the Top)
			player.landed = false;
			player.colliding.wallTop = true;
		}

	}
	//If the collider is a killing obstacle DIE
	if (c2->type == COLLIDER_TRAP)
	{
		player.dead = true;
	}

	//At the end put the player pos onto the collider Pos THIS IS ONLY FOR TESTIN CHANGE/FIX @Dídac
	player.playerPos.x = player.playerHitbox->rect.x;
	//player.playerPos.y = player.playerHitbox->rect.y;

} 

void j2Player::OnPreCollision(int d) 
{
	/*player.d_to_ground = d;
	player.nextFrameLanded = true;*/
}

void j2Player::HorCollisionCheck(Collider* c1, Collider* c2)
{
	int a = 0;
}

