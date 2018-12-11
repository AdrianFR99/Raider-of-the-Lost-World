#include "j2LifeItem.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j2Collision.h"
#include "j2EntityManager.h"
#include "j2Player.h"

j2LifeItem::j2LifeItem():j2StaticEntity()
{
	name = "RedGem";
	
}


j2LifeItem::~j2LifeItem()
{


}
bool j2LifeItem::Start() {
	
	type = ENTITY_TYPE::LIFE_ITEM;
	EntityText = App->tex->Load("textures/GemRed.png");

	ColliderRect = { 0,0,16,16 };
	Offsets.colliderOffset = { 8,8 };
	EntityRect = { 0,0,32,32 };

	LifeItemCollider = App->collision->AddCollider(ColliderRect, COLLIDER_ITEM, App->entities);
	colliders.add(LifeItemCollider);
	LifeItemCollider->SetPos(position.x + Offsets.colliderOffset.x , position.y + Offsets.colliderOffset.y);

	return true;
}

bool j2LifeItem::PreUpdate() {

	return true;
}

bool j2LifeItem::Update(float dt, bool do_logic) {

	

	return true;
}
bool j2LifeItem::PostUpdate() {

	App->render->Blit(EntityText, position.x, position.y, &EntityRect, SDL_FLIP_NONE);

	return true;
}

bool j2LifeItem::CleanUp() {

	
	for (int i = 0; i < colliders.count(); ++i) {

		colliders.At(i)->data->to_delete = true;

	}


	App->entities->DestroyEntity(this);

	return true;
}

bool j2LifeItem::Load(pugi::xml_node&) {

	return true;
}
bool j2LifeItem::Save(pugi::xml_node&) {

	return true;
}


void j2LifeItem::OnCollision(Collider* c1, Collider* c2) {

	if (c2->type == COLLIDER_PLAYER) {

		App->entities->player->HitsToRecive++;

		CleanUp();


	}




}
void j2LifeItem::CheckPreCollision() {


}