#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j2Collision.h"
#include "j1Window.h"
#include "p2Defs.h"
#include "p2Log.h"

j2Collision::j2Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;
}

// Destructor
j2Collision::~j2Collision()
{}

bool j2Collision::PreUpdate()
{
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
		}
	}

	return true;
}

// Called before render is available
 bool j2Collision::Update(float dt)
{

	DebugDraw();

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
		//case COLLIDER_WALL: // blue
		//	App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
		//	break;
		//case COLLIDER_PLAYER: // green
		//	App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
		//	break;
		

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
	return !(rect.y + rect.h < r.y || rect.y > r.y + r.h || rect.x + rect.w < r.x || rect.x > r.x + r.w);
}