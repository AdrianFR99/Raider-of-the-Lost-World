#include "j1App.h"
#include "p2Log.h"
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
	playerRect = {0,0,16,32};
	return true;
}

bool j2Player::CleanUp()
{
	return true;
}

bool j2Player::Update()
{
	return true;
}


