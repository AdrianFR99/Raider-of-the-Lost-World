#include "j1App.h"
#include "p2Log.h"
#include "j1render.h"
#include "j1window.h"
#include "j1input.h"
#include "j2Collision.h"
#include "SDL/include/SDL.h"
#include "j2Player.h"



//CONSTRUCTOR
j2Player::j2Player()
{

}

//DESTRUCTOR
j2Player::~j2Player()
{

}


bool j2Player::Start()
{
	LOG("Player Start");
	
	player.playerPos.x = 64;
	player.playerPos.y = 36 * 16;

	player.playerRect.h = 32;
	player.playerRect.w = 16;
	player.playerRect.x = player.playerPos.x;
	player.playerRect.y = player.playerPos.y;

	lateralTest.h = 48;
	lateralTest.w = 64;
	lateralTest.x = player.playerPos.x + 128;
	lateralTest.y = player.playerPos.y - 16;

	lateralTest_2.h = 48;
	lateralTest_2.w = 32;
	lateralTest_2.x = player.playerPos.x -40;
	lateralTest_2.y = player.playerPos.y - 16;

	verticalTest.h = 48;
	verticalTest.w = lateralTest.x - (player.playerPos.x + -64 );
	verticalTest.x = player.playerPos.x + -64;
	verticalTest.y = player.playerPos.y + 32;
	
	lateralTestHitbox = App->collision->AddCollider(lateralTest, COLLIDER_WALL);
	verticalTestHitbox = App->collision->AddCollider(verticalTest, COLLIDER_WALL);
	player.playerHitbox = App->collision->AddCollider(player.playerRect, COLLIDER_PLAYER,this);
	lateralTestHitbox_2 = App->collision->AddCollider(lateralTest_2, COLLIDER_WALL);
	player.x_speed = 2;
	player.y_speed = -10;


	player.landed = false;


	player.actual_x_speed = 0;
	player.actual_y_speed = 0;
	player.stopped_speed = 0;

	player.colliding.wallFront = false;
	player.colliding.wallBack = false;
	player.colliding.wallDown = false;
	player.colliding.wallTop = false;

	//Calling the camera to follow the player
	App->render->camera.x = player.playerRect.x * App->win->GetScale() - App->render->camera.w / 2;
	App->render->camera.y = player.playerRect.y * App->win->GetScale() - App->render->camera.h / 2;
	 
	return true;
}

bool j2Player::CleanUp()
{
	LOG("Player CleanUp");

	player.playerHitbox = nullptr;

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

	/*if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player.landed == true)
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
	}
	*/
	
	
	//We pass them onto the player Rect
	player.playerRect.x = player.playerPos.x;
	player.playerRect.y = player.playerPos.y;
	return true;
}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	//Camera Following player logic
	//App->render->followPlayer(player);

	// We reset the colliders collisions
	player.colliding.wallFront = false;
	player.colliding.wallBack = false;
	

	

	if (App->render->camera.x < 0)
	{
		App->render->camera.x = 0;
	}



		//Here we change the values of the rect position
	

	player.playerHitbox->SetPos(player.playerRect.x, player.playerRect.y);




	//Here we draw some quads for DEBUG purposes
	/*App->render->DrawQuad(player.playerRect, 255, 0, 0, 200);
	App->render->DrawQuad(lateralTest, 0, 255, 0, 100);
	App->render->DrawQuad(verticalTest, 0, 0, 255, 100);*/
	
	return true;
}


void j2Player::OnCollision(Collider* c1, Collider* c2) 
{
	if (c2->type == COLLIDER_WALL)
	{
		if (player.playerHitbox->rect.x + player.playerHitbox->rect.w > c2->rect.x 
			&& c2->rect.x - player.playerHitbox->rect.x > 0
			&& c2->rect.y < player.playerHitbox->rect.y)
		{
			player.colliding.wallFront = true;
		}
		if (player.playerHitbox->rect.x < c2->rect.x + c2->rect.w 
			&& player.playerHitbox->rect.x - c2->rect.x > 0
			&& c2->rect.y < player.playerHitbox->rect.y)
		{
			player.colliding.wallBack = true;
		}
		
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


