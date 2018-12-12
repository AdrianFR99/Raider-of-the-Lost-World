#include "j2LifeItem.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j2Collision.h"
#include "j2EntityManager.h"
#include "j1Audio.h"
#include "j2Player.h"

j2LifeItem::j2LifeItem():j2StaticEntity()
{
	name = "RedGem";
	pugi::xml_parse_result result2 = config.load_file("config.xml");
	
	PathSound = config.child("config").child("entities").child("Items").child("FX").child("Life").attribute("path").as_string();

	type = ENTITY_TYPE::LIFE_ITEM;
}


j2LifeItem::~j2LifeItem()
{


}
bool j2LifeItem::Start() {
	
	
	EntityText = App->tex->Load("textures/GemRed.png");

	ColliderRect = { 0,0,16,16 };
	Offsets.colliderOffset = { 8,8 };
	EntityRect = { 0,0,32,32 };

	LifeSound = App->audio->LoadFx(PathSound.GetString());

	EntityCollider = App->collision->AddCollider(ColliderRect, COLLIDER_ITEM, App->entities);
	colliders.add(EntityCollider);
	EntityCollider->SetPos(position.x + Offsets.colliderOffset.x , position.y + Offsets.colliderOffset.y);

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



		App->audio->PlayFx(LifeSound, 0);
		App->entities->player->HitsToRecive++;

		CleanUp();


	}




}
