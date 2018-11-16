#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j2Collision.h"
#include "j2Player.h"
#include "j1Window.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

j2Collision::j2Collision()
{
	name.create("collisions");
	bool debugMode = false;	//We start the game without seeing any colliders
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_TRAP] = false;
	matrix[COLLIDER_WALL][COLLIDER_ICE] = false;
	matrix[COLLIDER_WALL][COLLIDER_WATER] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_WALL][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_CHECK] = true;
	matrix[COLLIDER_WALL][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_TRAP][COLLIDER_WALL] = false;
	matrix[COLLIDER_TRAP][COLLIDER_TRAP] = false;
	matrix[COLLIDER_TRAP][COLLIDER_ICE] = false;
	matrix[COLLIDER_TRAP][COLLIDER_WATER] = false;
	matrix[COLLIDER_TRAP][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_TRAP][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_TRAP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_TRAP][COLLIDER_PLAYER_CHECK] = false;
	matrix[COLLIDER_TRAP][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_TRAP][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_ICE][COLLIDER_WALL] = false;
	matrix[COLLIDER_ICE][COLLIDER_TRAP] = false;
	matrix[COLLIDER_ICE][COLLIDER_ICE] = false;
	matrix[COLLIDER_ICE][COLLIDER_WATER] = false;
	matrix[COLLIDER_ICE][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_ICE][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_ICE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ICE][COLLIDER_PLAYER_CHECK] = true;
	matrix[COLLIDER_ICE][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_ICE][COLLIDER_PLAYERATTACK] = false;


	matrix[COLLIDER_WATER][COLLIDER_WALL] = false;
	matrix[COLLIDER_WATER][COLLIDER_TRAP] = false;
	matrix[COLLIDER_WATER][COLLIDER_ICE] = false;
	matrix[COLLIDER_WATER][COLLIDER_WATER] = false;
	matrix[COLLIDER_WATER][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_WATER][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_WATER][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WATER][COLLIDER_PLAYER_CHECK] = false;
	matrix[COLLIDER_WATER][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_WATER][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_PLATFORM][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_TRAP] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_ICE] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_WATER] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER_CHECK] = true;
	matrix[COLLIDER_PLATFORM][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_CLIMBWALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_TRAP] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_ICE] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_WATER] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_PLAYER_CHECK] = true;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_CLIMBWALL][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_TRAP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ICE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WATER] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CLIMBWALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_CHECK] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_TRAP] = false;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_ICE] = true;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_WATER] = true;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_CLIMBWALL] = true;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_PLAYER_CHECK] = false;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_PLAYER_CHECK][COLLIDER_PLAYERATTACK] = false;

	matrix[COLLIDER_GODMODE][COLLIDER_WALL] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_TRAP] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_ICE] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_WATER] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_CLIMBWALL] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_PLAYER_CHECK] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_GODMODE] = false;
	matrix[COLLIDER_GODMODE][COLLIDER_PLAYERATTACK] = false;

}

// Destructor
j2Collision::~j2Collision()
{}

bool j2Collision::PreUpdate()
{
	BROFILER_CATEGORY("Collision_PreUpdate", Profiler::Color::Crimson);
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		if (c1->rect.x + c1->rect.w >= (App->render->camera.x / App->win->GetScale())
			&& c1->rect.x < ((App->render->camera.x + App->render->camera.w) / App->win->GetScale()))
		{
			// avoid checking collisions already checked
			for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
			{
				// skip empty colliders
				if (colliders[k] == nullptr)
					continue;

				c2 = colliders[k];

				if (c1->CheckCollision(c2->rect) == true)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);

				}

				/*if (c1->PreCheckCollision(App->player->player) == true)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnPreCollision(c1->ret_d_to_ground(App->player->player));

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnPreCollision(c2->ret_d_to_ground(App->player->player));

				}*/
			}
		}
	}


	return true;
}

// Called before render is available
 bool j2Collision::Update(float dt)
{
	 //Check if we will call debugDraw or not at Update

	 if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	 {
		 debugMode = !debugMode;
	 }
	 if (debugMode == true)
	 {
		 DebugDraw();
	 }

	return true;
}

void j2Collision::DebugDraw()
{


	//if (App->input->coliderView == false || App->input->debugMode == false) //@andres
	//{
	//	return;
	//}


	uint alpha = 80;
	uint alphaHard = 130;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: //yellow
			App->render->DrawQuad(colliders[i]->rect, 225,225, 0, alpha);
			break;
		case COLLIDER_PLAYER: // grey
			App->render->DrawQuad(colliders[i]->rect, 150, 150, 150, alpha);
			break;
		case COLLIDER_PLAYER_CHECK: // darker grey
			App->render->DrawQuad(colliders[i]->rect, 70, 70,70, alpha);
			break;
		case COLLIDER_TRAP: //red
			App->render->DrawQuad(colliders[i]->rect,255, 0, 0, alpha);
			break;

		case COLLIDER_ICE: //cyan
			App->render->DrawQuad(colliders[i]->rect, 0, 183, 235, alpha);
			break;

		case COLLIDER_WATER://blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;

		case COLLIDER_PLATFORM://magenta
			App->render->DrawQuad(colliders[i]->rect, 202, 31, 123, alpha);
			break;

		case COLLIDER_CLIMBWALL: //green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;

		case COLLIDER_GODMODE: //green
			App->render->DrawQuad(colliders[i]->rect, 125, 255, 125, alpha);
			break;
		

		}
	}
}

// Called before quitting
bool j2Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}


Collider* j2Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return !((rect.y + rect.h)< r.y || rect.y > r.y + r.h || (rect.x + rect.w ) < r.x  || rect.x > r.x + r.w );
}

bool Collider::PreCheckCollision(const Player& p) const
{
	if (App->player->Speed.y > (rect.y - (p.playerRect.y + p.playerRect.h)) && p.landed == false)
	{
		return true;
	}
	return false;
}

int Collider::ret_d_to_ground(const Player& p) const
{
	return (rect.y - (p.playerRect.y + p.playerRect.h));
}