#include "j2EntityManager.h"
//#include "j2Entity.h"

#include "j2Player.h"
#include "j2Enemy.h"
#include "j1App.h"

j2EntityManager::j2EntityManager() : j1Module()
{
	name.create("entities");
}

j2EntityManager::~j2EntityManager()
{
}

bool j2EntityManager::Awake(pugi::xml_node & config)
{
	return true;
}

bool j2EntityManager::Start()
{
	CreateEntity(ENTITY_TYPE::ENEMY);
	return true;
}

bool j2EntityManager::PreUpdate()
{
	return true;
}

bool j2EntityManager::Update(float dt)
{
	return true;
}

bool j2EntityManager::PostUpdate()
{
	return true;
}

bool j2EntityManager::CleanUp()
{
	return true;
}

j2Entity* j2EntityManager::CreateEntity(ENTITY_TYPE type)
{
	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(2), "code needs update");
	j2Entity* ret = nullptr;
	switch (type) {
		case ENTITY_TYPE::ENEMY : ret = new j2Enemy(); break;
	}
	if (ret != nullptr)
		entities.add(ret);
	return ret;
}
