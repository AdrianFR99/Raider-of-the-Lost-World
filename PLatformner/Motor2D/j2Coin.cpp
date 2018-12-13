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


	type = ENTITY_TYPE::COIN;
}


j2Coin::~j2Coin()
{
}
bool j2Coin::Start() {

	
	

	ColliderRect = { 0,0,12,12 };
	Offsets.colliderOffset = { 2,7 };
	
	CoinSound = App->audio->LoadFx(PathSound.GetString());

	EntityCollider = App->collision->AddCollider(ColliderRect, COLLIDER_ITEM, App->entities);
	colliders.add(EntityCollider);
	EntityCollider->SetPos(position.x + Offsets.colliderOffset.x, position.y + Offsets.colliderOffset.y);
	
	CurrentAnimation = &CoinAnim;

	return true;
}

bool j2Coin::PreUpdate() {

	return true;
}

bool j2Coin::Update(float dt, bool do_logic) {

	EntityRect = CurrentAnimation->GetCurrentFrame(dt);


	return true;
}
bool j2Coin::PostUpdate() {

	

	App->render->Blit(EntityText, position.x-3, position.y, &EntityRect, SDL_FLIP_NONE);

	return true;
}

bool j2Coin::CleanUp() {


	for (int i = 0; i < colliders.count(); ++i) {
		
			colliders.At(i)->data->to_delete = true;
			colliders.At(i)->data = nullptr;
	}

	App->tex->UnLoad(EntityText);
	App->entities->DestroyEntity(this);

	

	return true;
}

bool j2Coin::Load(pugi::xml_node&) {

	return true;
}
bool j2Coin::Save(pugi::xml_node&) {

	return true;
}


void j2Coin::OnCollision(Collider* c1, Collider* c2) {

	if (c2->type == COLLIDER_PLAYER) {

		App->audio->PlayFx(CoinSound, 0);
		App->entities->player->Coins++;
		App->entities->player->Score += 10;

		CleanUp();

	}




}
