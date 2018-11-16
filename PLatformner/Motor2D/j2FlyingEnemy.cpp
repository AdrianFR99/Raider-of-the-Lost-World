#include "j2FlyingEnemy.h"
#include "j2EntityManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

j2FlyingEnemy::j2FlyingEnemy() : j2DynamicEntity()
{
	pugi::xml_parse_result result = configAnim.load_file("Animations.xml");

	if (result != NULL)
	{
		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Bat").child("idle");//idle
		idle.LoadPushBack(AnimPushBack);

	}
	else
	{
		LOG("Could not Load Flying Enemy Animations");
	}
	currentAnimation = nullptr;
}

j2FlyingEnemy::~j2FlyingEnemy()
{
}

bool j2FlyingEnemy::Start()
{
	position.x = 300;
	position.y = 560;

	AnimationRect = {0,0,16,16};

	entityTex = App->tex->Load("textures/bat.png");
	CurrentState = FLYING_ENEMY_STATE::PATROLLING;
	return true;
}

bool j2FlyingEnemy::PreUpdate()
{
	return true;
}

bool j2FlyingEnemy::Update(float dt,bool do_logic)
{
	
	EntityFX();
	
	AnimationRect = currentAnimation->GetCurrentFrame(dt);
	App->render->Blit(entityTex,position.x,position.y,&AnimationRect);

	return true;
}

bool j2FlyingEnemy::PostUpdate()
{
	return true;
}

bool j2FlyingEnemy::CleanUp()
{
	App->entities->DestroyEntity(this);
	return false;
}

bool j2FlyingEnemy::Load(pugi::xml_node &)
{
	return true;
}

bool j2FlyingEnemy::Save(pugi::xml_node &)
{
	return true;
}

void j2FlyingEnemy::EntityFX()
{
	if (ToMoveRight == true && ToMoveLeft == false) {
		lookingRight = true;
	}
	else if (ToMoveLeft == true && ToMoveRight == false) {
		lookingRight = false;
	}

	switch (CurrentState) {
	case FLYING_ENEMY_STATE::PATROLLING:
		PatrollingFX();
		break;
	case FLYING_ENEMY_STATE::CHASING_PLAYER:
		PatrollingFX();
		break;
	}
}

	void j2FlyingEnemy::PatrollingFX()
{
		currentAnimation = &idle;
}