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
#include "j1FadeToBlack.h"

j2FlyingEnemy::j2FlyingEnemy() : j2DynamicEntity()
{
	pugi::xml_parse_result result = configAnim.load_file("Animations.xml");
	pugi::xml_parse_result resultEnemies = configEnemy.load_file("enemies.xml");

	if (result != NULL)
	{
		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Bat").child("idle");//idle
		idle.LoadPushBack(AnimPushBack);

		AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Enemies").child("Bat").child("dead");//dead
		deadAnim.LoadPushBack(AnimPushBack);
	}
	else
	{
		LOG("Could not Load Flying Enemy Animations");
	}

	if (resultEnemies != NULL)
	{
		enemyNode = configEnemy.child("enemies").child("bat");
		active = enemyNode.child("active").attribute("value").as_bool();
		valid_path = enemyNode.child("valid_path").attribute("value").as_bool();
		//Rect
		AnimationRect.x = enemyNode.child("AnimationRect").attribute("x").as_int();
		AnimationRect.x = enemyNode.child("AnimationRect").attribute("y").as_int();
		AnimationRect.w = enemyNode.child("AnimationRect").attribute("w").as_int();
		AnimationRect.h = enemyNode.child("AnimationRect").attribute("h").as_int();
		//Texture
		texturePath.create(enemyNode.child("texture").attribute("path").as_string());
		EntityText = App->tex->Load(texturePath.GetString());

		//MaxDistances
		maxtileDistance = enemyNode.child("maxTileDistance").attribute("value").as_int();
		maxSoundDistance = enemyNode.child("maxSoundDistance").attribute("value").as_int();

		//Speeds
		speed_x = enemyNode.child("speed_x").attribute("value").as_float();
		speed_y = enemyNode.child("speed_y").attribute("value").as_float();

		playerPathPositionAdjuster_x = enemyNode.child("playerPathPositionAdjuster_x").attribute("value").as_int();
		playerPathPositionAdjuster_y = enemyNode.child("playerPathPositionAdjuster_y").attribute("value").as_int();
	
		HittedPath = configEnemy.child("enemies").child("bat").child("FX").child("HittedPath").attribute("path").as_string();
		EntityRect = AnimationRect;

	}
	else
	{
		LOG("Could not Load enemies.xml");
	}

	EntitiesEnable = true;
	//currentAnimation = nullptr;
	type = ENTITY_TYPE::FLYING_ENEMY;
}

j2FlyingEnemy::~j2FlyingEnemy()
{
}

bool j2FlyingEnemy::Start()
{	

	active = false;

	CurrentState = FLYING_ENEMY_STATE::PATROLLING;

	EntityCollider = App->collision->AddCollider(AnimationRect,COLLIDER_ENEMY, App->entities);
	colliders.add(EntityCollider);

	HittedSound = App->audio->LoadFx(HittedPath.GetString());
	


	return true;
}

bool j2FlyingEnemy::PreUpdate()
{
	return true;
}

bool j2FlyingEnemy::Update(float dt,bool do_logic)
{
	
	//If oustide camera, remain inactive
	if (position.x + AnimationRect.w >= (App->render->camera.x / App->win->GetScale())
		&& position.x < ((App->render->camera.x + App->render->camera.w) / App->win->GetScale()))
		active = true;
	else
		active = false;
	if (active==true)	//If Active: Ok let's do Stuff!
	{
		if (do_logic == true)
		{
			if (App->entities->player->player.playerGodModeHitbox == nullptr)
			{
				CheckRelativePosition();
				if (tileDistance < maxtileDistance)	//Get a nice path
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
			//Play the innovation FX function allowing for distance attenuation and Panning
			if (tileDistance*App->map->data.tile_width < maxSoundDistance) 
				App->audio->PlayEnvironmentalFx(App->audio->bat_sound, App->audio->bat_channel, App->map->MapToWorld(enemyPathfindingPosition.x,
					enemyPathfindingPosition.y, App->map->data), App->map->MapToWorld(playerPathfindingPosition.x, playerPathfindingPosition.y, App->map->data));
		}
		
		if (dead==false) {

			EntityMovement(dt);	//Execute the necessary movements
			SwithcingStates(dt); //Switch the states
			EntityFX();	//Get Your Animation!

			position.x += Speed.x;
			position.y += Speed.y;
		}

		if(currentAnimation !=nullptr)
		AnimationRect = currentAnimation->GetCurrentFrame(dt);

		//Assign positionChanges
	

		EntityCollider->SetPos(position.x, position.y);

		if (lookingRight==true) {
			App->render->Blit(EntityText, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);
		}
		else {
			App->render->Blit(EntityText, position.x - PivotAdjustment, position.y, &AnimationRect, SDL_FLIP_HORIZONTAL);
		}
	}



	if (dead==true && currentAnimation->Finished()) {
		EntitiesEnable = false;
	}



	return true;
}

bool j2FlyingEnemy::PostUpdate()
{
	return true;
}

bool j2FlyingEnemy::CleanUp()
{

	/*int i=colliders.find(enemy_collider);
	colliders.At(i)->data->to_delete = true;*/

	
		for (int i = 0; i < colliders.count(); ++i) {
			if (colliders.At(i)->data != nullptr) {
			
				colliders.At(i)->data->to_delete = true;
				colliders.At(i)->data = nullptr;
			}
		}
		EntityCollider = nullptr;
	


	App->tex->UnLoad(EntityText);
	App->entities->DestroyEntity(this);

	return true;
}

bool j2FlyingEnemy::Load(pugi::xml_node & data)
{

	for (pugi::xml_node EntityItem = data.child("BatEntity"); EntityItem; EntityItem = EntityItem.next_sibling("BatEntity")) {

		if (EntityItem.attribute("id").as_int() == id) {


			EntitiesEnable = EntityItem.attribute("Enabled").as_bool();
			position.x = EntityItem.attribute("PositionX").as_int();
			position.y = EntityItem.attribute("PositionY").as_int();

			if (EntityItem.attribute("dead").as_bool() == false && dead == true) {

				EntityCollider = App->collision->AddCollider(EntityRect, COLLIDER_ENEMY, App->entities);
				colliders.add(EntityCollider);

			}

			dead = EntityItem.attribute("dead").as_bool();


			break;
		}
	}







	return true;
}

bool j2FlyingEnemy::Save(pugi::xml_node & data) const
{
	pugi::xml_node EnemyInfo = data.append_child("BatEntity");
	
	EnemyInfo.append_attribute("id") = id;
	EnemyInfo.append_attribute("PositionX") = position.x;
	EnemyInfo.append_attribute("PositionY") = position.y;
	EnemyInfo.append_attribute("dead") = dead;
	EnemyInfo.append_attribute("Enabled") = EntitiesEnable;




	return true;
}

void j2FlyingEnemy::EntityMovement(float dt)
{
	iPoint destination;

	if (tileDistance < maxtileDistance && valid_path == true)
	{
		if (path->Count() > 2)
			destination = App->map->MapToWorld(path->At(2)->x, path->At(2)->y, App->map->data);
		else if (path->Count() > 0)
			destination = App->map->MapToWorld(path->At(0)->x, path->At(0)->y, App->map->data);


		if (path->Count() > 0 && tileDistance < maxtileDistance)
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
	//If we Shouldn't be chasing the player don't move
	if (valid_path == false || tileDistance > maxtileDistance)
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

		if (ToMoveUp)
		{
			Speed.y = floor(-speed_y * dt);
		}
		else if (ToMoveDown)
		{
			Speed.y = ceil(speed_y * dt);
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
{ 
	
	
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
		playerPathfindingPosition = { App->map->WorldToMap(App->entities->player->position.x + playerPathPositionAdjuster_x, App->entities->player->position.y + playerPathPositionAdjuster_y, App->map->data) };
		enemyPathfindingPosition = { App->map->WorldToMap(position.x,position.y, App->map->data) };
		tileDistanceBetweenEntities = { playerPathfindingPosition.x - enemyPathfindingPosition.x, playerPathfindingPosition.y - enemyPathfindingPosition.y };

		tileDistance = sqrt(tileDistanceBetweenEntities.x*tileDistanceBetweenEntities.x + tileDistanceBetweenEntities.y*tileDistanceBetweenEntities.y);
	}

	void j2FlyingEnemy::OnCollision(Collider* c1, Collider* c2)
	{
		if (dead == false){
			if (c2->type == COLLIDER_PLAYER_ATTACK)
			{


				currentAnimation = &deadAnim;
				dead = true;

				App->audio->PlayFx(HittedSound, 0);
				App->fade->FadeCustom(255, 255, 255, 30.0f, 0.01f);
				App->entities->player->Score += 30;

				for (int i = 0; i < colliders.count(); ++i) {

					if(colliders.At(i)->data!=nullptr)
					colliders.At(i)->data->to_delete = true;


				}
				
			

			}
	}
	}