#include "j2GroundEnemy.h"
#include "j2DynamicEntity.h"
#include "j2EntityManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include <math.h>
#include "j1App.h"
#include "j2EntityManager.h"
#include "j1Pathfinding.h"
#include "j2Player.h"
#include "j1Scene.h"

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
	position.x = 300;
	position.y = 574;
	Speed.x = 0;
	Speed.y = 0;

	Maxspeed.x = 5;

	AnimationRect = { 0,0,idle.frames->w,idle.frames->h };
	ColliderRect = {position.x,position.y,16,26};

	EntityText = App->tex->Load("textures/ZombieEnemieSpriteSheet.png");
	CurrentState = GROUND_ENEMY_STATE::PATROLLING;

	groundEnemyCollider = App->collision->AddCollider(ColliderRect,COLLIDER_ENEMY,App->entities);
	colliders.add(groundEnemyCollider);

	boundaries.wallBack = false;
	boundaries.wallBack = false;
	boundaries.wallBack = false;
	boundaries.wallBack = false;

	ToMoveDown = false;
	ToMoveUp = false;
	ToMoveRight = false;
	ToMoveLeft = false;

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

	if (do_logic == true)
	{
		CheckRelativePosition();
		if (tileDistance < 15)
		{
			App->pathfinding->CreatePath(enemyPathfindingPosition, playerPathfindingPosition);
			path = App->pathfinding->GetLastPath();
		}
		EntityMovement(dt);
	}

	//Change Position Depending on Speed
	position.x += Speed.x;
	position.y += Speed.y;
	

	EntityFX();

	AnimationRect = currentAnimation->GetCurrentFrame(dt);

	if (lookingRight) {
		App->render->Blit(EntityText, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(EntityText, position.x - PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}

	groundEnemyCollider->SetPos(position.x + 16, position.y+8);
	colliderPosition = { groundEnemyCollider->rect.x, groundEnemyCollider->rect.y };
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

void j2GroundEnemy::OnCollision(Collider * c1, Collider * c2)
{
	SDL_Rect overlay; // SDL_Rect of the intersection between the 2 colliders

	SDL_IntersectRect(&c1->rect, &c2->rect, &overlay);

	if (overlay.w > 0 && MovingRight == true && overlay.h > 10)
	{
		position.x -= overlay.w;
	}
	else if (overlay.w > 0 && MovingLeft == true && overlay.h > 10)
	{
		position.x += overlay.w;
	}
}

void j2GroundEnemy::CheckPreCollision()
{
		iPoint cell;
		if(App->scene->CurrentMap2 == false)
		colliderPosition = App->map->WorldToMap(colliderPosition.x, colliderPosition.y, App->map->data);
		else
		colliderPosition = App->map->WorldToMap(colliderPosition.x, colliderPosition.y, App->map->data2);

		// north
		cell.create(colliderPosition.x, colliderPosition.y + 1);
		if (App->pathfinding->IsWalkable(cell))
			


		// south
		cell.create(colliderPosition.x, colliderPosition.y - 1);
		if (App->pathfinding->IsWalkable(cell))


		// Right
		cell.create(colliderPosition.x + 1 , colliderPosition.y );
		if (App->pathfinding->IsWalkable(cell))
			boundaries.wallFront = true;
		else
			boundaries.wallFront = false;
		// west
		cell.create(colliderPosition.x - 1, colliderPosition.y);
		if (App->pathfinding->IsWalkable(cell))
			boundaries.wallBack = true;
		else
			boundaries.wallBack = false;
}

void j2GroundEnemy::EntityMovement(float dt)
{
	iPoint destination;
	if (path->Count() > 2)
		destination = App->map->MapToWorld(path->At(2)->x, path->At(2)->y, App->map->data);

	if (path->Count() > 2 && tileDistance < 15)
	{
		if (position.x < destination.x && boundaries.wallFront == false )
		{
			ToMoveRight = true;
			ToMoveLeft = false;
		}
		else if (position.x > destination.x && boundaries.wallBack == false)
		{
			ToMoveRight = false;
			ToMoveLeft = true;
		}
		/*else if (position.x == destination.x)
		{
			ToMoveRight = false;
			ToMoveLeft = false;
		}*/
		else
		{
			ToMoveRight = false;
			ToMoveLeft = false;
		}
	}
	else
	{
		ToMoveRight = false;
		ToMoveLeft = false;
		ToMoveDown = false;
		ToMoveUp = false;
	}


	if (ToMoveRight)
	{
		Speed.x = ceil(60 * dt);
	}
	else if (ToMoveLeft)
	{
		Speed.x = floor(-60 * dt);
	}
	else
	{
		Speed.x = 0;
	}

	if (Speed.x < -Maxspeed.x)
	{
		Speed.x = -Maxspeed.x;
	}
	else if (Speed.x < -Maxspeed.x)
	{
		Speed.x = -Maxspeed.x;
	}
}



void j2GroundEnemy::EntityFX()
{
	//CHANGE/FIX

	if (!(Speed.x == 0.0f && Speed.y == 0.0f))
	{
		CheckPreCollision();
	}
	if (Speed.x > 0.0f) {
		MovingRight = true;
		MovingLeft = false;
		CurrentState = GROUND_ENEMY_STATE::WALKING;
	}
	else if (Speed.x < 0.0f) {
		MovingLeft = true;
		MovingRight = false;
		CurrentState = GROUND_ENEMY_STATE::WALKING;
	}
	else if (Speed.x == 0.0f) {
		MovingLeft = false;
		MovingRight = false;
		CurrentState = GROUND_ENEMY_STATE::IDLE;
	}


	 if (Speed.y < 0.0f) {
		MovingUp = true;
		MovingDown = false;
		CurrentState = GROUND_ENEMY_STATE::IDLE;
	}
	else if (Speed.y > 0.0f) {
		MovingDown = true;
		MovingUp = false;
		CurrentState = GROUND_ENEMY_STATE::IDLE;
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
	case GROUND_ENEMY_STATE::IDLE:
		PatrollingFX();
		break;
	case GROUND_ENEMY_STATE::WALKING:
		WalkingFX();
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

void j2GroundEnemy::CheckRelativePosition()
{
	playerPathfindingPosition = App->map->WorldToMap(App->entities->player->position.x, App->entities->player->position.y, App->map->data);
	enemyPathfindingPosition = { App->map->WorldToMap(position.x,position.y, App->map->data) };
	tileDistanceBetweenEntities = { playerPathfindingPosition.x - enemyPathfindingPosition.x, playerPathfindingPosition.y - enemyPathfindingPosition.y };

	tileDistance = sqrt(tileDistanceBetweenEntities.x*tileDistanceBetweenEntities.x + tileDistanceBetweenEntities.y*tileDistanceBetweenEntities.y);
}