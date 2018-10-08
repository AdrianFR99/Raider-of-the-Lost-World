#include "j1App.h"
#include "p2Log.h"
#include "j1render.h"
#include "j1input.h"
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
	
	playerPos.x = 64;
	playerPos.y = 36 * 16;

	playerRect.h = 32;
	playerRect.w = 16;
	playerRect.x = playerPos.x;
	playerRect.y = playerPos.y;

	lateralTest.h = 48;
	lateralTest.w = 64;
	lateralTest.x = playerPos.x + 128;
	lateralTest.y = playerPos.y - 16;

	verticalTest.h = 48;
	verticalTest.w = 512;
	verticalTest.x = playerPos.x + -64;
	verticalTest.y = playerPos.y + 32;
	

	x_speed = 4;
	y_speed = -15;

	landed = true;
	 
	return true;
}

bool j2Player::CleanUp()
{
	return true;
}


bool j2Player::PreUpdate()
{
	//App->render->DrawQuad(playerRect, 255, 0, 0, 130, true, false);
	
	return true;
}


bool j2Player::Update(float dt)
{
	//This variable STORES the distance of the player lowest point of the lowest pixel to the ground
	d_to_ground = verticalTest.y - (playerRect.y + playerRect.h);

	//Check Horizontal collisions
	if ( (CheckCollision(lateralTest) == true || PreCheckCollision(lateralTest) == true)
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT )
	{
		x_speed = 0;
	}
	else
	{
		x_speed = 4;
	}

	//Check vertical collisions and set LANDED
	if (CheckVerticalCollision(verticalTest) == true /*|| CheckVerticalCollision(lateralTest) == true*/)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			landed = false;
			y_speed = -15;
		}
		else
		{
			landed = true;
		}
		
	}
	else
	{
		landed = false;
	}
	

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		playerPos.x += x_speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		playerPos.x -= x_speed;
	}


	//LANDED LOGIC:  when landed == false the player 
	//is not touching a solid surface with its feet
	//when landed == true the player IS touching a solid surface with its feet

	if (landed == false)
	{
		playerPos.y += y_speed;
		y_speed += 1;
	}
	else
	{
		y_speed = 0;
	}

	
	//Here we change the values of the rect position
	playerRect.x = playerPos.x;
	playerRect.y = playerPos.y;


	//Here we draw some quads for DEBUG purposes
	App->render->DrawQuad(playerRect, 255, 0, 0, 200);
	App->render->DrawQuad(lateralTest, 0, 255, 0, 100);
	App->render->DrawQuad(verticalTest, 0, 0, 255, 100);
	return true;
}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	
	//App->render->DrawQuad(playerRect, 255, 0, 0, 130, true, false);
	return true;
}


bool j2Player::CheckCollision(const SDL_Rect& r) const
{
	return !(playerRect.y + playerRect.h < r.y || playerRect.y > r.y + r.h || playerRect.x + playerRect.w < r.x || playerRect.x > r.x + r.w);
}

bool j2Player::PreCheckCollision(const SDL_Rect& r) const
{
	return !((playerRect.y + playerRect.h + 1) < r.y || (playerRect.y -1) > r.y + r.h || ((playerRect.x + playerRect.w) + 1 )< r.x || playerRect.x > r.x + r.w);
}

bool j2Player::CheckVerticalCollision(const SDL_Rect& r) const
{
	return !( (playerRect.y + playerRect.h +1 )< r.y || playerRect.y > r.y + r.h || playerRect.x + playerRect.w < r.x || playerRect.x > r.x + r.w);
}