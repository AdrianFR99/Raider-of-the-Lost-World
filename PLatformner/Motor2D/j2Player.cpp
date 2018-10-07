#include "j1App.h"
#include "p2Log.h"
#include "j1render.h"
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
	
	playerRect.h = 32;
	playerRect.w = 16;
	playerRect.x = -32;
	playerRect.y = -32;
	 
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
	
	App->render->DrawQuad(playerRect, 255, 0, 0, 255);


	return true;
}

// Called each loop iteration
bool j2Player::PostUpdate()
{
	//App->render->DrawQuad(playerRect, 255, 0, 0, 130, true, false);
	return true;
}


