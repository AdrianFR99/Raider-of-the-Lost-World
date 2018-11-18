#include "j2FlyingEnemy.h"
#include "j2EntityManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j2Collision.h"
#include "j2Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "j1Window.h"

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
{	//Change/Fix @Dídac
	active = false;
	/*position.x = 300;
	position.y = 560;*/
	valid_path = false;
	AnimationRect = {0,0,16,16};

	EntityText = App->tex->Load("textures/bat.png");
	CurrentState = FLYING_ENEMY_STATE::PATROLLING;

	enemy_collider = App->collision->AddCollider(AnimationRect,COLLIDER_ENEMY, App->entities);
	colliders.add(enemy_collider);

	return true;
}

bool j2FlyingEnemy::PreUpdate()
{
	return true;
}

bool j2FlyingEnemy::Update(float dt,bool do_logic)
{
	if (position.x + AnimationRect.w >= (App->render->camera.x / App->win->GetScale())
		&& position.x < ((App->render->camera.x + App->render->camera.w) / App->win->GetScale()))
		active = true;
	else
		active = false;
	if (active)
	{
		if (do_logic == true)
		{
			CheckRelativePosition();
			int ret = App->pathfinding->CreatePath(enemyPathfindingPosition, playerPathfindingPosition);
			if (ret != -1)
			{
				valid_path = true;
				path = App->pathfinding->GetLastPath();
			}
			else
			{
				valid_path = false;
			}
			if (tileDistance*App->map->data.tile_width < 400)
				App->audio->PlayEnvironmentalFx(App->audio->bat_sound, 5, App->map->MapToWorld(enemyPathfindingPosition.x,
					enemyPathfindingPosition.y, App->map->data), App->map->MapToWorld(playerPathfindingPosition.x, playerPathfindingPosition.y, App->map->data));
		}
		EntityMovement(dt);
		SwithcingStates(dt);
		EntityFX();

		AnimationRect = currentAnimation->GetCurrentFrame(dt);

		if (lookingRight) {
			App->render->Blit(EntityText, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);
		}
		else {
			App->render->Blit(EntityText, position.x - PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
		}

		position.x += Speed.x;
		position.y += Speed.y;

		enemy_collider->SetPos(position.x, position.y);

	}
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

void j2FlyingEnemy::EntityMovement(float dt)
{
	iPoint destination;

	if (tileDistance < 15 && valid_path == true)
	{
		if (path->Count() > 2)
			destination = App->map->MapToWorld(path->At(2)->x, path->At(2)->y, App->map->data);
		else if (path->Count() > 1)
			destination = App->map->MapToWorld(path->At(0)->x, path->At(0)->y, App->map->data);
		else if (path->Count() > 0)
			destination = App->map->MapToWorld(enemyPathfindingPosition.x +10, enemyPathfindingPosition.y, App->map->data);

		if (path->Count() > 0 && tileDistance < 15)
		{
			if (position.x < destination.x)
			{
				ToMoveRight = true;
				ToMoveLeft = false;
			}
			else if (position.x > destination.x)
			{
				ToMoveRight = false;
				ToMoveLeft = true;
			}
			else if (position.x == destination.x)
			{
				ToMoveRight = false;
				ToMoveLeft = false;
			}

			if (position.y < destination.y)
			{
				ToMoveDown = true;
				ToMoveUp = false;
			}
			else if (position.y > destination.y)
			{
				ToMoveDown = false;
				ToMoveUp = true;
			}
			else if (position.y == destination.y)
			{
				ToMoveDown = false;
				ToMoveUp = false;
			}

		}
		else
		{
			ToMoveRight = false;
			ToMoveLeft = false;
			ToMoveDown = false;
			ToMoveUp = false;
		}
	}
	if (valid_path == false)
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

		if (ToMoveUp)
		{
			Speed.y = floor(-60 * dt);
		}
		else if (ToMoveDown)
		{
			Speed.y = ceil(60 * dt);
		}
		else
		{
			Speed.y = 0;
		}
}

void j2FlyingEnemy::SwithcingStates(float dt)
{
	if (Speed.x > 0.0f) {
		MovingRight = true;
		MovingLeft = false;
		CurrentState = FLYING_ENEMY_STATE::CHASING_PLAYER;
	}
	else if (Speed.x < 0.0f) {
		MovingLeft = true;
		MovingRight = false;
		CurrentState = FLYING_ENEMY_STATE::CHASING_PLAYER;
	}
	else if (Speed.x == 0.0f) {
		MovingLeft = false;
		MovingRight = false;
		CurrentState = FLYING_ENEMY_STATE::PATROLLING;
	}
	else if (Speed.y < 0.0f) {
		MovingUp = true;
		MovingDown = false;
		CurrentState = FLYING_ENEMY_STATE::CHASING_PLAYER;
	}
	else if (Speed.y > 0.0f) {
		MovingDown = true;
		MovingUp = false;
		CurrentState = FLYING_ENEMY_STATE::CHASING_PLAYER;
	}
	else if (Speed.y == 0.0f) {
		MovingUp = false;
		MovingDown = false;
		CurrentState = FLYING_ENEMY_STATE::PATROLLING;
	}
}

void j2FlyingEnemy::EntityFX()
{ //CHANGE/FIX
	if (MovingRight == true && MovingLeft == false) {
		lookingRight = true;
	}
	else if (MovingLeft == true && MovingRight == false) {
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

	void j2FlyingEnemy::CheckRelativePosition()
	{
		playerPathfindingPosition = { App->map->WorldToMap(App->entities->player->position.x + 16, App->entities->player->position.y +32, App->map->data) };
		enemyPathfindingPosition = { App->map->WorldToMap(position.x,position.y, App->map->data) };
		tileDistanceBetweenEntities = { playerPathfindingPosition.x - enemyPathfindingPosition.x, playerPathfindingPosition.y - enemyPathfindingPosition.y };

		tileDistance = sqrt(tileDistanceBetweenEntities.x*tileDistanceBetweenEntities.x + tileDistanceBetweenEntities.y*tileDistanceBetweenEntities.y);
	}

	void j2FlyingEnemy::OnCollision(Collider* c1, Collider* c2)
	{
		if (c2->type == COLLIDER_PLAYER_ATTACK )
		{
			
		}
	}