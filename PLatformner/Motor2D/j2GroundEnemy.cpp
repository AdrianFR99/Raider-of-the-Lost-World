#include "j2GroundEnemy.h"
#include "j2DynamicEntity.h"
#include "j2EntityManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"

#include "j1Input.h"

j2GroundEnemy::j2GroundEnemy() : j2DynamicEntity()
{
	pugi::xml_parse_result result = configAnim.load_file("Animations.xml");

	if (result != NULL)
	{
		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("idle");//idle
		idle.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Walk");//walk
		walk.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Attack");//attack
		attack.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Hurt");//hurt
		hurt.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Death");//death
		death.LoadPushBack(AnimPushBack);
		

	}
	else
	{
		LOG("Could not Load Flying Enemy Animations");
	}
	currentAnimation = nullptr;
}

j2GroundEnemy::~j2GroundEnemy()
{
}

bool j2GroundEnemy::Start()
{
	position.x = 250;
	position.y = 570;

	AnimationRect = { 0,0,idle.frames->w,idle.frames->h };

	entityTex = App->tex->Load("textures/ZombieEnemieSpriteSheet.png");
	CurrentState = GROUND_ENEMY_STATE::PATROLLING;
	return true;
}

bool j2GroundEnemy::PreUpdate()
{
	return true;
}

bool j2GroundEnemy::Update(float dt, bool do_logic)
{
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_REPEAT)
	{
		CurrentState = GROUND_ENEMY_STATE::CHASING_PLAYER;
	}
	else if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT)
	{
		CurrentState = GROUND_ENEMY_STATE::ATTACKING;
	}
	else if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT)
	{
		CurrentState = GROUND_ENEMY_STATE::HURT;
	}
	else if (App->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT)
	{
		CurrentState = GROUND_ENEMY_STATE::DEATH;
	}
	else
	{
		CurrentState = GROUND_ENEMY_STATE::PATROLLING;
	}

	/*Speed.x = -60 * dt;

	position.x += Speed.x ;*/
	EntityFX();

	AnimationRect = currentAnimation->GetCurrentFrame(dt);

	if (lookingRight) {
		App->render->Blit(entityTex, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(entityTex, position.x - PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}
	return true;

	return true;
}

bool j2GroundEnemy::PostUpdate()
{
	return true;
}

bool j2GroundEnemy::CleanUp()
{
	App->entities->DestroyEntity(this);
	return false;
}

bool j2GroundEnemy::Load(pugi::xml_node &)
{
	return true;
}

bool j2GroundEnemy::Save(pugi::xml_node &)
{
	return true;
}

void j2GroundEnemy::EntityFX()
{
	//CHANGE/FIX

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

	if (ToMoveRight == true && ToMoveLeft == false) {
		lookingRight = true;
	}
	else if (ToMoveLeft == true && ToMoveRight == false) {
		lookingRight = false;
	}

	switch (CurrentState) {
	case GROUND_ENEMY_STATE::PATROLLING:
		PatrollingFX();
		break;
	case GROUND_ENEMY_STATE::CHASING_PLAYER:
		WalkingFX();
		break;
	case GROUND_ENEMY_STATE::ATTACKING:
		AttackingFX();
		break;
	case GROUND_ENEMY_STATE::HURT:
		HurtFX();
		break;
	case GROUND_ENEMY_STATE::DEATH:
		DyingFX();
		break;
	}
}

void j2GroundEnemy::PatrollingFX()
{
	currentAnimation = &idle;
}

void j2GroundEnemy::WalkingFX()
{
	currentAnimation = &walk;
}
void j2GroundEnemy::AttackingFX()
{
	currentAnimation = &attack;
}
void j2GroundEnemy::HurtFX()
{
	currentAnimation = &hurt;
}
void j2GroundEnemy::DyingFX()
{
	currentAnimation = &death;
}