#include "j2Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j2Collision.h"
#include "j2EntityManager.h"
#include "j1Audio.h"
#include "j2Player.h"


j2Coin::j2Coin()
{
	pugi::xml_parse_result result = configAnim.load_file("Animations.xml");
	pugi::xml_parse_result result2 = config.load_file("config.xml");
	
	AnimPushBack = configAnim.child("Anim").child("AnimationsPushBacks").child("Items").child("Coin");//idle
	CoinAnim.LoadPushBack(AnimPushBack);

	PathSound = config.child("config").child("entities").child("Items").child("FX").child("Coin").attribute("path").as_string();
	
	
	EntityText = App->tex->Load("textures/Coin.png");

	EntitiesEnable = true;
	type = ENTITY_TYPE::COIN;
}


j2Coin::~j2Coin()
{
}
bool j2Coin::Start() {

	
	

	EntityRect = { 0,0,12,12 };
	Offsets.colliderOffset = { 2,7 };
	
	CoinSound = App->audio->LoadFx(PathSound.GetString());

	EntityCollider = App->collision->AddCollider(EntityRect, COLLIDER_ITEM, App->entities);
	colliders.add(EntityCollider);
	EntityCollider->SetPos(position.x + Offsets.colliderOffset.x, position.y + Offsets.colliderOffset.y);
	
	CurrentAnimation = &CoinAnim;

	return true;
}

bool j2Coin::PreUpdate() {

	return true;
}

bool j2Coin::Update(float dt, bool do_logic) {


	if (touched == true)
	{
		touched = false;
		EntitiesEnable = false;

	}


	AnimationRect = CurrentAnimation->GetCurrentFrame(dt);


	return true;
}
bool j2Coin::PostUpdate() {

	

	App->render->Blit(EntityText, position.x-3, position.y, &AnimationRect, SDL_FLIP_NONE);

	return true;
}

bool j2Coin::CleanUp() {


	if (EntityCollider != nullptr) {
		for (int i = 0; i < colliders.count(); ++i) {

			colliders.At(i)->data->to_delete = true;
			colliders.At(i)->data = nullptr;
		}
	}
	App->tex->UnLoad(EntityText);
	App->entities->DestroyEntity(this);

	

	return true;
}

bool j2Coin::Load(pugi::xml_node& data) {

	//EntitiesEnable = data.child("EntityCoin").attribute("Enabled").as_bool();
	//position.x = data.child("EntityCoin").attribute("PositionX").as_int();
	//position.y = data.child("EntityCoin").attribute("PositionY").as_int();



	return true;
}
bool j2Coin::Save(pugi::xml_node& data) const {

	//pugi::xml_node Coin = data.append_child("EntityCoin");

	//Coin.append_attribute("Enabled") = EntitiesEnable;
	//Coin.append_attribute("PositionX") = position.x;
	//Coin.append_attribute("PositionY") = position.y;


	return true;
}


void j2Coin::OnCollision(Collider* c1, Collider* c2) {

	
	if (c2->type == COLLIDER_PLAYER) {


		touched = true;
		App->audio->PlayFx(CoinSound, 0);
		App->entities->player->Coins++;
		App->entities->player->Score += 10;

		for (int i = 0; i < colliders.count(); ++i) {

			colliders.At(i)->data->to_delete = true;

		}

	
		

	}




}
