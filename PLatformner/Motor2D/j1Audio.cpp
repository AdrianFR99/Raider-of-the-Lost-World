#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"
#include "ElementGUI.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize the Audio Strings list from config.xml

	if (config != NULL)
	{
		music_folder = config.child("music").child_value("folder");
		Chunks_folder = config.child("FX").child_value("folderFX");

		//This fills the list of Strings with the names of the songs declared in config.xml
		for (pugi::xml_node music = config.child("music").child("song"); ret && music; music = music.next_sibling("song"))
		{
			p2SString* aux = new p2SString;
			aux->create(music.attribute("name").as_string());
			songs_list.add(aux);
		}
		//MaxAttenuation assignment
		max_attenuation_distance = config.child("FX").child("max_attenuation_distance").attribute("value").as_int();
		assert((max_attenuation_distance > 1 && max_attenuation_distance < 255),"max_attenuation_distance value is out of bounds! Distance attenuation won't work! Remember values from 1 to 255 only");
		p2SString loadBatSound = config.child("FX").child("bat_fx").attribute("value").as_string();
		p2SString bat_fx("%s%s", App->audio->Chunks_folder.GetString(), loadBatSound.GetString());
		bat_sound = LoadFx(bat_fx.GetString());
		bat_channel = config.child("FX").child("bat_cdhannel").attribute("value").as_int();
	}
	
	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	//Unregistering all Effects (Panning and Distance)
	Mix_UnregisterAllEffects(bat_channel); //Change/Fix Dídac
	//Clear Music strings
	songs_list.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat, int channel)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	return ret;
}

void j1Audio::PlayEnvironmentalFx(unsigned int id, int channel, const iPoint& sound_emmiter, const iPoint& sound_listener, int repeat)
{
	iPoint distance;
	distance.x = sound_emmiter.x - sound_listener.x;
	distance.y = sound_emmiter.y - sound_listener.y;
	//Calculate distance to sound_listener and adjust the value for the function Thresshold
	int hypotenuse = sqrt(distance.x*distance.x + distance.y*distance.y);
	if (hypotenuse > max_attenuation_distance)
		hypotenuse = max_attenuation_distance;
	
	//Calculate X distance for panning purposes
	int aux_distance_x = distance.x;

	if (aux_distance_x > max_attenuation_distance)
		aux_distance_x = max_attenuation_distance;
	else if (aux_distance_x < -max_attenuation_distance)
		aux_distance_x = max_attenuation_distance;
	else if (aux_distance_x == 0)
		aux_distance_x = 1;
	else
		aux_distance_x = abs(aux_distance_x);

	//Set the Panning: for the situation in which the emmiter and listener have very similar positions in x axis, we want the total volume to equal 255
	//					if both speakers/Headphones where playing at 255 in the function for Panning we would play the volume twice as loud.
	uint left = 127; //In case the listener and the emitter share the same exact x pos

	if (distance.x > 0)
		left = 127 - (aux_distance_x / 2);
	else
		left = 127 + (abs(aux_distance_x / 2));

	//Play the Effect
	PlayFx(id,repeat,channel);
	//Process the information for the Effect
	Mix_SetDistance(channel, hypotenuse);			//Attenuation depending on distance
	Mix_SetPanning(channel, left, 255 - left);		//Panning (Right or Left speakers/headphones volumes)
}



void j1Audio::callbackUiElement(ElementGUI *element)
{
	if (element->type == ElementType::BUTTON )
	{
		if (element->action == ElementAction::MUSIC_VOL && element->Parent!=nullptr)
		{
			Mix_VolumeMusic(128* element->Parent->slider_value);
		}
		if (element->action == ElementAction::SFX_VOL && element->Parent != nullptr)
		{
			for (p2List_item<Mix_Chunk*>* item = fx.start; item != nullptr; item = item->next)
			{
				Mix_VolumeChunk(item->data,int(255 * element->Parent->slider_value));
			}
		}
	}
}


