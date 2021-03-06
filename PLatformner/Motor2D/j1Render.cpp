#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1map.h"
#include "j2player.h"
#include "j2EntityManager.h"
#include "Brofiler/Brofiler.h"

#define VSYNC true

j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = config.child("cameraInit").attribute("x").as_int();
		camera.y = config.child("cameraInit").attribute("y").as_int();

		cameraDisplacement = config.child("cameraDisplacement").attribute("value").as_int();
		cameraOffset_right = config.child("cameraOffset").attribute("right_x").as_int();
		cameraOffset_left = config.child("cameraOffset").attribute("left_x").as_int();

		map1_cameraLimit_y = config.child("cameraLimits_map1").attribute("y_max").as_int();
		map2_cameraLimit_y = config.child("cameraLimits_map2").attribute("y_max").as_int();
		map2_cameraLimit_x = config.child("cameraLimits_map2").attribute("x_max").as_int();
		scale = App->win->GetScale();

	}
	
	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	//App->render->camera.y = player.playerRect.y * App->win->GetScale() - App->render->camera.h / 2;
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	BROFILER_CATEGORY("Render_PreUpdate", Profiler::Color::MediumSeaGreen);
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	BROFILER_CATEGORY("Render_Update", Profiler::Color::PaleGreen);
	return true;
}

bool j1Render::PostUpdate()
{
	BROFILER_CATEGORY("Render_PostUpdate", Profiler::Color::SeaGreen);
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint j1Render::ScreenToWorld(int x, int y,MapData& DataAux) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	if (DataAux.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = (x + camera.x / scale);
		ret.y = (y + camera.y / scale);
	}
	else if (DataAux.type == MapTypes::MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - camera.x / scale);
		ret.y = (y - camera.y / scale);
	}

	return ret;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section,SDL_RendererFlip flip, float speed, double angle, int pivot_x, int pivot_y, bool use_camera, bool ignore_size) const
{
	bool ret = true;
		uint scale = App->win->GetScale();

		SDL_Rect rect;
		if (use_camera == true)
		{
			rect.x = (int)-(camera.x * speed) + x * scale;
			rect.y = (int)(-camera.y * speed) + y * scale;
		}
		else
		{
			rect.x = x;
			rect.y = y;
		}

		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}

		if (ignore_size == false)
		{
			rect.w *= scale;
			rect.h *= scale;
		}

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivot_x != INT_MAX && pivot_y != INT_MAX)
		{
			pivot.x = pivot_x;
			pivot.y = pivot_y;
			p = &pivot;
		}

		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}

	
	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{

		rec.x = (int)(-camera.x + rect.x *scale); // Changed this scale thing 
		rec.y = (int)(-camera.y + rect.y * scale);

		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}


//Follow the player around
void j1Render::followPlayer(const Player &p,float dt)
{
	//Follow exactly the player's movement
	//camera.x = p.playerRect.x * App->win->GetScale() - camera.w / 2;
	//camera.y = p.playerRect.y * App->win->GetScale() - camera.h /2;

	if ((App->entities->player->position.x - camera.x / scale) >= cameraOffset_right * scale &&
		(App->entities->player->Speed.x > 0 ||
			App->entities->player->Speed.x == App->entities->player->Maxspeed.x ))
	{
		//App->render->camera.x = p.playerRect.x - 100 * App->win->GetScale() + p.x_speed * App->win->GetScale();
		camera.x +=((App->entities->player->Speed.x) * scale)*dt;
	
		//parallax
		if (App->scene->CurrentMap2 == false) {
			
				App->map->data.imagelayers.At(1)->data->SpeedBack -= App->map->data.imagelayers.At(1)->data->PropImg.GetProperty("speed", 0.5);
				App->map->data.imagelayers.At(0)->data->SpeedBack -= App->map->data.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.25);
		}
		else {

				App->map->data2.imagelayers.At(0)->data->SpeedBack -= App->map->data2.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.0);
				App->map->data2.imagelayers.At(3)->data->SpeedBack -= App->map->data2.imagelayers.At(3)->data->PropImg.GetProperty("speed", 0.2);
				App->map->data2.imagelayers.At(2)->data->SpeedBack -= App->map->data2.imagelayers.At(2)->data->PropImg.GetProperty("speed", 0.15);


		}

		
	}

	if ((App->entities->player->position.x - camera.x / scale) <= cameraOffset_left / scale && (App->entities->player->Speed.x < 0 || App->entities->player->Speed.x == -App->entities->player->Maxspeed.x))
	{
		//App->render->camera.x = player.playerRect.x - App->render->camera.w / 2 - 200;
		
		App->render->camera.x +=((App->entities->player->Speed.x) * scale)*dt;

		//parallax
		if (App->scene->CurrentMap2 == false) {
		
				App->map->data.imagelayers.At(1)->data->SpeedBack += App->map->data.imagelayers.At(1)->data->PropImg.GetProperty("speed", 0.5);
				App->map->data.imagelayers.At(0)->data->SpeedBack += App->map->data.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.25);
		}
		else {
				
				App->map->data2.imagelayers.At(0)->data->SpeedBack += App->map->data2.imagelayers.At(0)->data->PropImg.GetProperty("speed", 0.0);
				App->map->data2.imagelayers.At(3)->data->SpeedBack += App->map->data2.imagelayers.At(3)->data->PropImg.GetProperty("speed", 0.2);
				App->map->data2.imagelayers.At(2)->data->SpeedBack += App->map->data2.imagelayers.At(2)->data->PropImg.GetProperty("speed", 0.15);


		}
		
	}


		if (App->entities->player->position.y < (camera.y + camera.h /3) / scale && App->entities->player->position.y != camera.y + (camera.h / 2))
		{
		
				if (App->entities->player->Speed.y < 0 || App->entities->player->Speed.y==-App->entities->player->Maxspeed.y)
					camera.y += ((App->entities->player->Speed.y) * scale)*dt;

				else if (App->entities->player->Speed.y == 0) {

					camera.y -= ((App->entities->player->Maxspeed.y) * scale)*dt;

				}


			
		}

		if (App->entities->player->position.y > (camera.y + (camera.h /3)*2) / scale && App->entities->player->position.y != camera.y + (camera.h / 2))
		{
		
			
				if (App->entities->player->Speed.y > 0 || App->entities->player->Speed.y == +App->entities->player->Maxspeed.y)
					camera.y += ((App->entities->player->Speed.y) * scale)*dt;

				else if (App->entities->player->Speed.y == 0) {

					camera.y += ((App->entities->player->Maxspeed.y) * scale)*dt;
				}

			
		}
	
	

	//Controlling that the camera does not leave the map
	if (camera.x < 0)
	{
		camera.x = 0;
	}

	if (camera.y < 0)
	{
		camera.y = 0;
	}
	//If first map
	if (App->scene->CurrentMap2 == false)
	{		
		if (camera.y > map1_cameraLimit_y / scale && camera.y > 0 && App->entities->player->EntityRect.y > 0)
		{
			camera.y = map1_cameraLimit_y / scale;
		}
	}
	else //If 2nd Map
	{
		
		if (camera.x > map2_cameraLimit_x / scale)
		{
			camera.x = map2_cameraLimit_x / scale;
		}
		
		if (camera.y > map2_cameraLimit_y / scale)
		{
			camera.y = map2_cameraLimit_y / scale;
		}
	}

	
}