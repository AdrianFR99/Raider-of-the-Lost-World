#include "j2Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"

j2Enemy::j2Enemy() : j2DynamicEntity()
{
}

j2Enemy::~j2Enemy()
{
}

bool j2Enemy::Start()
{
	position.x = 300;
	position.y = 560;

	EntityRect = {0,0,48,16};

	entityTex = App->tex->Load("textures/bat.png");
	return true;
}

bool j2Enemy::PreUpdate()
{
	return true;
}

bool j2Enemy::Update(float dt,bool do_logic)
{
	App->render->Blit(entityTex,position.x,position.y,&EntityRect);
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
