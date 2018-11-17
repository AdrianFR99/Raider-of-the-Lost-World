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
	position.x = 300;
	position.y = 560;
	
	AnimationRect = {0,0,16,16};

	entityTex = App->tex->Load("textures/bat.png");
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

	if (do_logic == true)
	{
		CheckRelativePosition();
		if (tileDistance < 15)
		{
			App->pathfinding->CreatePath(flyingEnemyPathfindingPosition, playerPathfindingPosition);
			path = App->pathfinding->GetLastPath();
		}
		if (tileDistance*App->map->data.tile_width < 400)
		App->audio->PlayEnvironmentalFx(App->audio->bat_sound, 5, App->map->MapToWorld(flyingEnemyPathfindingPosition.x, 
									flyingEnemyPathfindingPosition.y,App->map->data), App->map->MapToWorld(playerPathfindingPosition.x, playerPathfindingPosition.y,App->map->data));
	}
	EntityMovement(dt);

	EntityFX();
	
	AnimationRect = currentAnimation->GetCurrentFrame(dt);
	
	if (lookingRight) {
		App->render->Blit(entityTex, position.x, position.y , &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(entityTex, position.x - PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}

	position.x += Speed.x;
	position.y += Speed.y;

	enemy_collider->SetPos(position.x,position.y);

	
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
	if (path->Count() > 2)
		destination = App->map->MapToWorld(path->At(2)->x, path->At(2)->y, App->map->data);

	if ( path->Count() > 2 && tileDistance < 15 )
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
		else if (position.y  > destination.y)
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

void j2FlyingEnemy::EntityFX()
{ //CHANGE/FIX

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
		//Change Player Vars for entity vars (@Adri) 
		playerPathfindingPosition = { App->map->WorldToMap(App->player->player.playerPos.x, App->player->player.playerPos.y, App->map->data).x,App->map->WorldToMap(App->player->player.playerPos.x, App->player->player.playerPos.y, App->map->data).y };
		flyingEnemyPathfindingPosition = { App->map->WorldToMap(position.x,position.y, App->map->data) };
		tileDistanceBetweenEntities = { playerPathfindingPosition.x - flyingEnemyPathfindingPosition.x, playerPathfindingPosition.y - flyingEnemyPathfindingPosition.y };

		tileDistance = sqrt(tileDistanceBetweenEntities.x*tileDistanceBetweenEntities.x + tileDistanceBetweenEntities.y*tileDistanceBetweenEntities.y);

	}

	void j2FlyingEnemy::OnCollision(Collider* c1, Collider* c2)
	{
		if (c2->type == COLLIDER_PLAYERATTACK )
		{
			
		}
	}