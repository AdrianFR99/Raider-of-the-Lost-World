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
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Input.h"


j2GroundEnemy::j2GroundEnemy() : j2DynamicEntity()
{
	pugi::xml_parse_result result = configAnim.load_file("Animations.xml");
	pugi::xml_parse_result resultEnemies = configEnemy.load_file("enemies.xml");
	if (result != NULL)
	{
		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("idle");//idle
		idle.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Walk");//walk
		walk.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Attack");//attack
		attack.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Hurt");//hurt
		hurtAnim.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Undead").child("Death");//death
		death.LoadPushBack(AnimPushBack);
		

	}
	else
	{
		LOG("Could not Load Flying Enemy Animations");
	}

	if (resultEnemies != NULL)
	{
		enemyNode = configEnemy.child("enemies").child("undead");
		active = enemyNode.child("active").attribute("value").as_bool();
		valid_path = enemyNode.child("valid_path").attribute("value").as_bool();

		//Rect
		AnimationRect.x = enemyNode.child("AnimationRect").attribute("x").as_int();
		AnimationRect.y = enemyNode.child("AnimationRect").attribute("y").as_int();

		////Texture
		//texturePath.create(enemyNode.child("texture").attribute("path").as_string());
		//EntityText = App->tex->Load(texturePath.GetString());

		////MaxDistances
		maxtileDistance = enemyNode.child("maxTileDistance").attribute("value").as_int();
		//maxSoundDistance = enemyNode.child("maxSoundDistance").attribute("value").as_int();


		life = enemyNode.child("life").attribute("value").as_int();
		////Speeds
		speed_x = enemyNode.child("speed_x").attribute("value").as_float();
		speed_y = enemyNode.child("speed_y").attribute("value").as_float();

		KnockBack = enemyNode.child("KnockBack").attribute("value").as_int();

		Maxspeed.x = enemyNode.child("maxSpeed_x").attribute("value").as_int();
		Maxspeed.y = enemyNode.child("maxSpeed_y").attribute("value").as_int();

		gravity = enemyNode.child("gravity").attribute("value").as_float();

		playerPathPositionAdjuster_x = enemyNode.child("playerPathPositionAdjuster_x").attribute("value").as_int();
		playerPathPositionAdjuster_y = enemyNode.child("playerPathPositionAdjuster_y").attribute("value").as_int();

		enemyPathPositionAdjuster_x = enemyNode.child("enemyPathPositionAdjuster_x").attribute("value").as_int();
		enemyPathPositionAdjuster_y = enemyNode.child("enemyPathPositionAdjuster_y").attribute("value").as_int();


		//Rect Values
		AnimationRect.w = idle.frames->w;
		AnimationRect.h = idle.frames->h;

		colliderRect_w = enemyNode.child("colliderRect_w").attribute("value").as_int();
		colliderRect_h = enemyNode.child("colliderRect_h").attribute("value").as_int();

		colliderOffset_x = enemyNode.child("colliderOffset_x").attribute("value").as_int();
		colliderOffset_y = enemyNode.child("colliderOffset_y").attribute("value").as_int();

		//Texture
		texturePath.create(enemyNode.child("texture").attribute("path").as_string());
		EntityText = App->tex->Load(texturePath.GetString());
	}
	else
	{
		LOG("Could not Load enemies.xml");
	}
	currentAnimation = nullptr;
	type = ENTITY_TYPE::GROUND_ENEMY;
}

j2GroundEnemy::~j2GroundEnemy()
{
}

bool j2GroundEnemy::Start()
{
	active = false;
	/*position.x = 300;
	position.y = 574;*/
	Speed.x = 0;
	Speed.y = 0;

	ColliderRect = { position.x,position.y,colliderRect_w,colliderRect_h };
	FakeColliderRect = { ColliderRect.x - 1,ColliderRect.y - 1,ColliderRect.w + 2, ColliderRect.h + 2};
	//EntityText = App->tex->Load("textures/ZombieEnemieSpriteSheet.png");
	CurrentState = GROUND_ENEMY_STATE::PATROLLING;

	groundEnemyCollider = App->collision->AddCollider(ColliderRect,COLLIDER_ENEMY,App->entities);
	groundEnemyFakeCollider = App->collision->AddCollider(FakeColliderRect, COLLIDER_ENEMY_CHECK, App->entities);
	colliders.add(groundEnemyCollider);
	colliders.add(groundEnemyFakeCollider);

	boundaries.wallFront = false;
	boundaries.wallBack = false;
	boundaries.wallDown = false;
	boundaries.wallTop = false;

	landed = true;
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
	if (dead == false) {
		if (hurt == false) {

			if (position.x + ColliderRect.w >= (App->render->camera.x / App->win->GetScale())
				&& position.x - ColliderRect.w < ((App->render->camera.x + App->render->camera.w) / App->win->GetScale())
				&& position.y + ColliderRect.h <= ((App->render->camera.y + App->render->camera.h) / App->win->GetScale())
				&& position.y >(App->render->camera.y / App->win->GetScale()))
			{
				if (active == false)
				{
					landed = true;
				}
				active = true;
			}
			else
			{
				active = false;
				landed = true;
				Speed.y = 0.0f;
			}

			if (active)
			{
				/*if (App->input->GetKey(SDL_SCANCODE_6) == KEY_REPEAT)
				{
					CurrentState = GROUND_ENEMY_STATE::CHASING_PLAYER;
				}
				else if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT)
				{
					CurrentState = GROUND_ENEMY_STATE::ATTACKING;
				}
				else if (hurted ==true)
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
				if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
				{
					position.y -= 40;
				}
	*/

				

				if (dead == false) {

					CheckRelativePosition();
					if (do_logic == true)
					{
						if (App->entities->player->player.playerGodModeHitbox == nullptr)
						{
							if (playerPathfindingPosition.y > enemyPathfindingPosition.y - 1) //Remmber this is in Map tiles
							{
								if (tileDistance < maxtileDistance)
								{
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
								}
							}
						}

					}

					EntityMovement(dt);


					//Change Position Depending on Speed
					position.x += Speed.x;
					position.y += Speed.y;
				}
			}

			SwithcingStates(dt);
		}

		else {
			
			CurrentState = GROUND_ENEMY_STATE::HURT;
			if (hurt == true && HurtTime.Read() > 1000)
				hurt = false;
		}
	}
	else {

		CurrentState = GROUND_ENEMY_STATE::DEATH;

	}

		EntityFX();
		
		groundEnemyCollider->SetPos(position.x + colliderOffset_x, position.y + colliderOffset_y);
		groundEnemyFakeCollider->SetPos(groundEnemyCollider->rect.x - 1, groundEnemyCollider->rect.y - 1);
		colliderPosition = { groundEnemyCollider->rect.x, groundEnemyCollider->rect.y };

		boundaries.wallFront = false;
		boundaries.wallBack = false;
		boundaries.wallDown = false;

		landed = false;
	


	
	
		if (dead==true && currentAnimation->Finished())
			CleanUp();



	AnimationRect = currentAnimation->GetCurrentFrame(dt);

	if (lookingRight) {
		App->render->Blit(EntityText, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);
	}
	else {
		App->render->Blit(EntityText, position.x - PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
	}

	return true;
}

bool j2GroundEnemy::PostUpdate()
{
	return true;
}

bool j2GroundEnemy::CleanUp()
{

	if (groundEnemyCollider != nullptr && groundEnemyFakeCollider != nullptr) {
		
		for (int i = 0; i < colliders.count(); ++i) {

			colliders.At(i)->data->to_delete = true;

		}
	}

	App->entities->DestroyEntity(this);
	return true;
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

	if (c1->type == COLLIDER_ENEMY && c2->type != COLLIDER_PLAYER && c2->type != COLLIDER_PLAYER_CHECK)
	{
		if (overlay.w > 0 && MovingRight == true && overlay.h > 10)
		{
			position.x -= overlay.w;
		}
		else if (overlay.w > 0 && MovingLeft == true && overlay.h > 10)
		{
			position.x += overlay.w;
		}

		if (overlay.h > 5 /*&& overlay.h <10*/ && overlay.w > 5 && MovingDown == true)
		{
			position.y -= overlay.h;

			colliders.start->data->SetPos(position.x, position.y);
			landed = true;
		}
	}

	if (c1->type == COLLIDER_ENEMY_CHECK && c2->type != COLLIDER_PLAYER && c2->type != COLLIDER_PLAYER_CHECK)
	{
		if (overlay.w > 0 && overlay.h > 5 && c1->rect.x < c2->rect.x &&overlay.x + overlay.w > c2->rect.x)
			boundaries.wallFront = true;
		if (overlay.w > 0 && overlay.h > 5 && c1->rect.x > c2->rect.x && c1->rect.x < c2->rect.x + c2->rect.w)
			boundaries.wallBack = true;
		if (overlay.h == 1 && overlay.w > 5)
			landed = true;
	}

	if (dead == false) {
		if (hurt == false) {

			if (c2->type == COLLIDER_PLAYER_ATTACK)
			{

				HurtTime.Start();
				hurtAnim.Reset();
				hurt = true;
				life--;


				if (c2->rect.x > c1->rect.x)
					position.x -= KnockBack;
				else if (c2->rect.x < c1->rect.x)
					position.x += KnockBack;

				else if (MovingRight == false && MovingLeft == false) {
					if (lookingRight == true)
						position.x -= KnockBack;
					else
						position.x += KnockBack;
				}


				if (life == 0) {

					dead = true;
					for (int i = 0; i < colliders.count(); ++i) {

						colliders.At(i)->data->to_delete = true;
						

					}


				}

				App->fade->FadeCustom(255, 255, 255, 30.0f, 0.01f);


			}

		}
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
	if (dead == false) {

		iPoint destination;
		if (tileDistance < maxtileDistance && valid_path == true)
		{
			if (path->Count() > 2)
				destination = App->map->MapToWorld(path->At(2)->x, path->At(2)->y, App->map->data);
			else if (path->Count() > 0)
				destination = App->map->MapToWorld(path->At(0)->x, path->At(0)->y, App->map->data);
		}
		else if (valid_path == false)
		{
			destination = App->map->MapToWorld(playerPathfindingPosition.x, playerPathfindingPosition.y, App->map->data);
		}
		//If the path cannot be done because we are colliding with something
		if (tileDistance < maxtileDistance && App->entities->player->player.playerGodModeHitbox == nullptr)
		{
			if (position.x < destination.x && boundaries.wallFront == false)
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
			Speed.x = ceil(speed_x * dt);
		}
		else if (ToMoveLeft)
		{
			Speed.x = floor(-speed_x * dt);
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

		if (landed == false)
		{
			Speed.y += gravity * dt;
			if (Speed.y < -Maxspeed.y)
			{
				Speed.y = -Maxspeed.y;
			}
			else if (Speed.y > Maxspeed.y)
			{
				Speed.y = Maxspeed.y;
			}
		}
		else
		{
			Speed.y = 0;
		}

	}
}

void j2GroundEnemy::SwithcingStates(float dt) 
{
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
		CurrentState = GROUND_ENEMY_STATE::AIR;
	}
	else if (Speed.y > 0.0f) {
		MovingDown = true;
		MovingUp = false;
		CurrentState = GROUND_ENEMY_STATE::AIR;
	}
	else if (Speed.y == 0.0f) {
		MovingUp = false;
		MovingDown = false;
	}

	if (landed == false)
	{
		CurrentState = GROUND_ENEMY_STATE::AIR;
	}

}



void j2GroundEnemy::EntityFX()
{
	//CHANGE/FIX
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
	case GROUND_ENEMY_STATE::AIR:
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

	attack.Reset();
	currentAnimation = &attack;
}
void j2GroundEnemy::HurtFX()
{
	currentAnimation = &hurtAnim;
}
void j2GroundEnemy::DyingFX()
{

	currentAnimation = &death;
}

void j2GroundEnemy::CheckRelativePosition()
{
	playerPathfindingPosition = App->map->WorldToMap(App->entities->player->position.x + playerPathPositionAdjuster_x, App->entities->player->position.y + playerPathPositionAdjuster_y, App->map->data);
	enemyPathfindingPosition = { App->map->WorldToMap(position.x + enemyPathPositionAdjuster_x,position.y + enemyPathPositionAdjuster_y, App->map->data) };
	tileDistanceBetweenEntities = { playerPathfindingPosition.x - enemyPathfindingPosition.x, playerPathfindingPosition.y - enemyPathfindingPosition.y };

	tileDistance = sqrt(tileDistanceBetweenEntities.x*tileDistanceBetweenEntities.x + tileDistanceBetweenEntities.y*tileDistanceBetweenEntities.y);
}