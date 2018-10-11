#ifndef __j2Collision_H__
#define __j2Collision_H__

#define MAX_COLLIDERS 3000	// Make dymanic array for final game?

#include "j1Module.h"

struct Player;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	bool PreCheckCollision(const Player& p) const;
	int ret_d_to_ground(const Player& p) const;
};

class j2Collision : public j1Module
{
public:

	j2Collision();
	~j2Collision();

	bool PreUpdate() ;
	bool Update( float dt);
	bool CleanUp() ;

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();

	
private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX]; 
	SDL_Rect screen;
	bool debugMode;
};

#endif // __ModuleCollision_H__
