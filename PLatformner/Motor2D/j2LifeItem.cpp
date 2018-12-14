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
	EntitiesEnable = true;
	type = ENTITY_TYPE::LIFE_ITEM;
}


j2LifeItem::~j2LifeItem()
{


}
bool j2LifeItem::Start() {
	
	
	EntityText = App->tex->Load("textures/GemRed.png");

	EntityRect = { 0,0,9,9 };
	Offsets.colliderOffset = { 0,0 };
	AnimationRect = { 0,0,9,9 };

	LifeSound = App->audio->LoadFx(PathSound.GetString());

	EntityCollider = App->collision->AddCollider(EntityRect, COLLIDER_ITEM, App->entities);
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

	App->render->Blit(EntityText, position.x, position.y, &AnimationRect, SDL_FLIP_NONE);

	return true;
}

bool j2LifeItem::CleanUp() {

	if (EntityCollider != nullptr) {
		for (int i = 0; i < colliders.count(); ++i) {

			colliders.At(i)->data->to_delete = true;
			colliders.At(i)->data = nullptr;
		}
		EntityCollider = nullptr;
	}
	App->tex->UnLoad(EntityText);
	App->entities->DestroyEntity(this);

	return true;
}

bool j2LifeItem::Load(pugi::xml_node& data) {


	for (pugi::xml_node EntityItem = data.child("Entitylife"); EntityItem; EntityItem = EntityItem.next_sibling("Entitylife")) {

		if (EntityItem.attribute("id").as_int() == id) {

			EntitiesEnable = EntityItem.attribute("Enabled").as_bool();
			position.x = EntityItem.attribute("PositionX").as_int();
			position.y = EntityItem.attribute("PositionY").as_int();
			
			if (EntityItem.attribute("touched").as_bool() == false && touched == true) {

				EntityCollider = App->collision->AddCollider(EntityRect, COLLIDER_ITEM, App->entities);
				colliders.add(EntityCollider);
				EntityCollider->SetPos(position.x + Offsets.colliderOffset.x, position.y + Offsets.colliderOffset.y);
			
			}

			
			touched = EntityItem.attribute("touched").as_bool();

			

			break;
		}
	}





	return true;
}
bool j2LifeItem::Save(pugi::xml_node& data) const {



	pugi::xml_node life = data.append_child("Entitylife");

	life.append_attribute("id") = id;
	life.append_attribute("Enabled") = EntitiesEnable;
	life.append_attribute("PositionX") = position.x;
	life.append_attribute("PositionY") = position.y;
	life.append_attribute("touched") = touched;



	return true;
}


void j2LifeItem::OnCollision(Collider* c1, Collider* c2) {

	if (c2->type == COLLIDER_PLAYER) {


		touched = true;
		App->audio->PlayFx(LifeSound, 0);
		App->entities->player->HitsToRecive++;


		for (int i = 0; i < colliders.count(); ++i) {

			colliders.At(i)->data->to_delete = true;

		}
		EntityCollider = nullptr;

		EntitiesEnable = false;
		


	}




}
