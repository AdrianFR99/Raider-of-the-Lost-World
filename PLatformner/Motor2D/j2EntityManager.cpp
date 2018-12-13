#include "j2EntityManager.h"
#include "j2Player.h"
#include "j2FlyingEnemy.h"
#include "j2GroundEnemy.h"
#include "j2LifeItem.h"
#include "j2Coin.h"
#include "j1App.h"
#include "Brofiler/Brofiler.h"


j2EntityManager::j2EntityManager() : j1Module()
{
	name.create("entities");


	update_ms_cycle = 0.200f;  //Change/Fix @D�dac must calculate value not hardcode

	player = (j2Player*)CreateEntity(ENTITY_TYPE::PLAYER);
	
	//CreateEntity(ENTITY_TYPE::FLYING_ENEMY);
	//CreateEntity(ENTITY_TYPE::GROUND_ENEMY);
}

j2EntityManager::~j2EntityManager()
{
}

bool j2EntityManager::Awake(pugi::xml_node & config)
{


	bool ret = true;
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		ret = item->data->Awake(config);
		if (!ret)
			break;
	}
	
	
	return ret;

}

bool j2EntityManager::Start()
{


	bool ret = true;
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		
		if (item->data->type != ENTITY_TYPE::GROUND_ENEMY && item->data->type != ENTITY_TYPE::FLYING_ENEMY ) {

			ret = item->data->Start();
			if (!ret)
				break;
		}
		return ret;

	}
}

bool j2EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("Entities_PreUpdate", Profiler::Color::GhostWhite);
	bool ret = true;
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		ret = item->data->PreUpdate();
		if (!ret)
			break;
	}
	return ret;
}

bool j2EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entities_Update", Profiler::Color::Gainsboro);
	bool ret = true;
	
	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;
	
	
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		ret = item->data->Update(dt,do_logic);
		if (!ret)
			break;
	}

	if (do_logic == true) {
		accumulated_time = 0.0f;
		do_logic = false;
	}

	return ret;
}

bool j2EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("Entities_PostUpdate", Profiler::Color::Azure);
	bool ret = true;
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		ret = item->data->PostUpdate();
		if (!ret)
			break;
	}
	return ret;
}

bool j2EntityManager::CleanUp()
{
	bool ret = true;
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
	

			ret = item->data->CleanUp();
		
			if (!ret)
				break;
		
	}
	return ret;
}

bool j2EntityManager::Load(pugi::xml_node &save_game_manager)
{
	bool ret = true;
	
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		ret = item->data->Load(save_game_manager);
		if (!ret)
			break;
	}
	return ret;
}

bool j2EntityManager::Save(pugi::xml_node &save_game_manager) const
{
	bool ret = true;

	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		ret = item->data->Save(save_game_manager);
		if (!ret)
			break;
	}
	return ret;
}

void j2EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		//Some Entities manage/have more than 1 collider
		for (p2List_item<Collider*>* entity_collider = item->data->colliders.start; entity_collider; entity_collider = entity_collider->next)
		{
			if (c1 == entity_collider->data)
			{
				item->data->OnCollision(c1, c2);
			}
		}
	}


	player->OnCollision(c1,c2);

}

j2Entity* j2EntityManager::CreateEntity(ENTITY_TYPE type)
{
	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(5), "code needs update");
	j2Entity* ret = nullptr;
	switch (type) {

		case ENTITY_TYPE::FLYING_ENEMY : ret = new j2FlyingEnemy(); break;
		case ENTITY_TYPE::GROUND_ENEMY: ret = new j2GroundEnemy(); break;
		case ENTITY_TYPE::PLAYER : ret = new j2Player(); break;
		case ENTITY_TYPE::LIFE_ITEM: ret = new j2LifeItem(); break;
		case ENTITY_TYPE::COIN: ret = new j2Coin(); break;
	}
	if (ret != nullptr)
		entities.add(ret);
	return ret;
}

void j2EntityManager::DestroyEntity(j2Entity* entity_to_destroy)
{
	for (p2List_item<j2Entity*>* item = entities.start; item; item = item->next)
	{
		if (item->data == entity_to_destroy)
		{
			entities.del(item);
			RELEASE(item->data);
			break;
		}
	}
}

