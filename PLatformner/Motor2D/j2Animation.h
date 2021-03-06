#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "P2Defs.h"
#include "SDL/include/SDL.h"
#define MAX_FRAMES 60

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		current_frame += speed*dt;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		if (current_frame >= MAX_FRAMES)
		{
			Reset();
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
	}

	void ResetLoops()
	{
		loops = 0;
	}


	void LoadPushBack(pugi::xml_node& config) {

		loop = config.attribute("loop").as_bool();
		speed = config.attribute("speed").as_float();

		SDL_Rect Rect;

		for (config = config.child("PushBack"); config; config = config.next_sibling("PushBack"))
		{
			Rect.x = config.attribute("x").as_int();
			Rect.y = config.attribute("y").as_int();
			Rect.w = config.attribute("w").as_int();
			Rect.h = config.attribute("h").as_int();

			this->PushBack({ Rect });
		}


	}

};

#endif
