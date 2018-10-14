#include "j1App.h"
#include "p2Log.h"
#include "j1render.h"
#include "j1window.h"
#include "j1input.h"
#include "j2Collision.h"
#include "j2Animation.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "SDL/include/SDL.h"
#include "j2Player.h"



//CONSTRUCTOR
j2Player::j2Player()
{
	name.create("player");
}

//DESTRUCTOR
j2Player::~j2Player()
{

}

bool j2Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Data");
	bool ret = true;
	
	if (config != NULL)
	{

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

		//Player landed
		player_Init.landed = config.child("landed").attribute("value").as_bool();

		//Player Death
		player_Init.dead = config.child("dead").attribute("boolDead").as_bool();
		player_Init.deadDelay = config.child("dead").attribute("deadDelay").as_int();
		player_Init.deadCounter = config.child("dead").attribute("deadCounter").as_int();
		player_Init.maximumDeadY_map1 = config.child("maximumDead_Y").attribute("map1").as_int();
		player_Init.maximumDeadY_map2 = config.child("maximumDead_Y").attribute("map2").as_int();
		
		
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

	/*lateralTest.h = 48;
	lateralTest.w = 64;
	lateralTest.x = player.playerPos.x + 128;
	lateralTest.y = player.playerPos.y - 16;

	lateralTest_2.h = 16;
	lateralTest_2.w = 32;
	lateralTest_2.x = player.playerPos.x -40;
	lateralTest_2.y = player.playerPos.y +16;

	verticalTest.h = 48;
	verticalTest.w = 3000;
	verticalTest.x = player.playerPos.x + -64;
	verticalTest.y = player.playerPos.y + 32;*/
	
	
	player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER,this);
	
	
	//PUSHBACKS HARDCODED THAT WILL LATER GO INTO CONFIG (just to test first)
	player.animations.idle.PushBack({14,6,19,30});
	player.animations.idle.PushBack({65,6,19,30});
	player.animations.idle.speed = 0.03f;

	player.animations.currentAnimation = &player.animations.idle;

	//Calling the camera to follow the player
	//App->render->camera.x = player.playerRect.x * App->win->GetScale() - App->render->camera.w / 2;
	//App->render->camera.y = player.playerRect.y * App->win->GetScale() - App->render->camera.h / 2;
	 
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

	if (player.animations.playTex != nullptr)
	{
		delete player.animations.playTex;
		player.animations.playTex = nullptr;
	}

	return true;
}


bool j2Player::PreUpdate()
{
	//PREUPDATE is called before any On Collision or Pre-Collision from the player is called
	// so we set vars like landed to false and in case we get a call back that the player is landed it will be changed in said functions.
	player.landed = false;
	player.nextFrameLanded = false;
	
	
	return true;
}


bool j2Player::Update(float dt)
{
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

		if (player.playerHitbox->type != COLLIDER_PLAYER)
		{
			player.playerRect = player_Init.playerRect;
			player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER, this);
		}
		//This variable STORES the distance of the player lowest point of the lowest pixel to the ground
		//d_to_ground = verticalTest.y - (playerRect.y + playerRect.h);

		//Check Horizontal collisions
		/*if ( (CheckCollision(lateralTest) == true || PreCheckCollision(lateralTest) == true)
			&& App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT )
		{
			x_speed = 0;
		}
		else
		{
			x_speed = 4;
		}*/

		//Check vertical collisions and set LANDED
		//if (CheckVerticalCollision(verticalTest) == true /*|| CheckVerticalCollision(lateralTest) == true*/)
		//{
		//	
		//	
		//}
		//else
		//{
		//	landed = false;
		//}

		//Control X speed
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player.colliding.wallFront == false)
		{
			player.playerPos.x += player.x_speed;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player.colliding.wallBack == false)
		{
			player.playerPos.x -= player.x_speed;
		}



		//LANDED LOGIC:  when landed == false the player 
		//is not touching a solid surface with its feet
		//when landed == true the player IS touching a solid surface with its feet



		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.colliding.wallTop == false
			&& player.landed == true)
		{
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
			player.y_speed += 1;

			if (player.y_speed > player.y_max_speed)
			{
				player.y_speed = player.y_max_speed;
			}
			player.doubleJump_counter += 1; // We increase the double Jump counter, as we're not on the ground
		}
		else
		{
			player.gravity_speed = 0.0f;
			player.doubleJump = true;
			player.doubleJump_counter = player_Init.doubleJump_counter;
		}
		/*if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			player.landed = false;
			player.y_speed = -10;
		}

		if (player.landed == false)
		{
			player.playerPos.y += player.y_speed;
			if (player.nextFrameLanded == false)
			{
				player.y_speed += 1;
			}
		}
		else
		{
			player.y_speed = 0;
		}*/
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
	
	//Camera Following player logic
	App->render->followPlayer(player);
	
	//We pass them onto the player Rect
	player.playerRect.x = player.playerPos.x;
	player.playerRect.y = player.playerPos.y;

	
	return true;
}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	//Camera Following player logic
	App->render->followPlayer(player);

	// We reset the colliders collisions
	player.colliding.wallFront = false;
	player.colliding.wallBack = false;
	player.colliding.wallDown = false;
	player.colliding.wallTop = false;
	
		//Here we change the values of the rect position
	

	player.playerHitbox->SetPos(player.playerRect.x, player.playerRect.y);
	
	return true;
}




void j2Player::OnCollision(Collider* c1, Collider* c2) 
{
	if (c2->type == COLLIDER_WALL 
		|| c2->type == COLLIDER_ICE 
		|| c2->type == COLLIDER_PLATFORM 
		|| c2->type == COLLIDER_CLIMBWALL)
	{
		if (player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x 
			&& c2->rect.x - player.playerHitbox->rect.x > 0
			&& c2->rect.y + 8 < player.playerHitbox->rect.y + player.playerHitbox->rect.h)
		{
			player.colliding.wallFront = true;
			/*player.playerHitbox->rect.x -= player.playerHitbox->rect.x + player.playerHitbox->rect.w - c2->rect.x;
			player.playerPos.x = player.playerHitbox->rect.x;*/
		}
		else if (player.playerHitbox->rect.x < c2->rect.x + c2->rect.w 
			&& player.playerHitbox->rect.x - c2->rect.x > 0
			&& c2->rect.y + 8 < player.playerHitbox->rect.y + player.playerHitbox->rect.h )
		{
			player.colliding.wallBack = true;
			/*player.playerHitbox->rect.x += c2->rect.x + c2->rect.w - player.playerHitbox->rect.x;
			player.playerPos.x = player.playerHitbox->rect.x;*/
			player.playerRect.y = c2->rect.y - player.playerRect.h;
		}

		else if(player.playerHitbox->rect.y + player.playerHitbox->rect.h > c2->rect.y
			&& player.playerHitbox->rect.y + player.playerHitbox->rect.h < c2->rect.y + c2->rect.h
			&& player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x)
		{
			player.landed = true;
			player.colliding.wallDown = true;
		}
		else if (player.playerHitbox->rect.y > c2->rect.y + c2->rect.h)
		{
			player.y_speed = -1;
			player.landed = false;
			//player.colliding.wallDown;
			player.colliding.wallTop = true;
		}
		
	}

	if (c2->type == COLLIDER_TRAP)
	{
		player.dead = true;
	}
} 

void j2Player::OnPreCollision(int d) 
{
	/*player.d_to_ground = d;
	player.nextFrameLanded = true;*/
}
//bool j2Player::CheckCollision(const SDL_Rect& r) const
//{
//	return !(playerRect.y + playerRect.h < r.y || playerRect.y > r.y + r.h || playerRect.x + playerRect.w < r.x || playerRect.x > r.x + r.w);
//}
//
//bool j2Player::PreCheckCollision(const SDL_Rect& r) const
//{
//	return !((playerRect.y + playerRect.h + 1) < r.y || (playerRect.y -1) > r.y + r.h || ((playerRect.x + playerRect.w) + 1 )< r.x || playerRect.x > r.x + r.w);
//}
//
//bool j2Player::CheckVerticalCollision(const SDL_Rect& r) const
//{
//	return !( (playerRect.y + playerRect.h +1 )< r.y || playerRect.y > r.y + r.h || playerRect.x + playerRect.w < r.x || playerRect.x > r.x + r.w);
//}

