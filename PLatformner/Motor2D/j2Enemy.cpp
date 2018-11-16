#include "j2Enemy.h"

bool j2Enemy::Start()
{
	return true;
}

bool j2Enemy::PreUpdate()
{
	return true;
}

bool j2Enemy::Update(float dt,bool do_logic)
{
	return true;
}

bool j2Enemy::PostUpdate()
{
	return true;
}

bool j2Enemy::CleanUp()
{
	return false;
}

bool j2Enemy::Load(pugi::xml_node &)
{
	return true;
}

bool j2Enemy::Save(pugi::xml_node &)
{
	return true;
}
