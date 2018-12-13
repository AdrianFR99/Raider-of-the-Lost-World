#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Window.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FadeToBlack::j1FadeToBlack() : j1Module()
{
	name.create("FadeToBlack");
	manage_active = false;
	r = g = b = 0;
	a = 255.0f;
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	App->win->GetWindowSize(screen_width, screen_height);
	screen = { 0, 0, int(screen_width * App->win->GetScale()), int(screen_height* App->win->GetScale()) };
	//LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	return true;
}

bool j1FadeToBlack::PostUpdate()
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			if (manage_active)
			{
				fade_out->Disable();
				fade_in->Enable();

				fade_out = nullptr;
				fade_in = nullptr;
			}

			// ---
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;

	}

	// Finally render the color square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, r, g, b, (Uint8)(normalized * a));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		fade_out = module_off;
		fade_in = module_on;

		//Added this for module activation/deactivation and color and alpha management
		manage_active = true;
		r = g = b = 0;
		a = 255.f;

		ret = true;
	}

	return ret;
}

// Fade to the desired color & max alpha occupying all screen.
bool j1FadeToBlack::FadeCustom(int r, int g, int b, float a,float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		//Added this for module activation/deactivation and color and alpha management
		manage_active = false;
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		ret = true;
	}

	return ret;
}