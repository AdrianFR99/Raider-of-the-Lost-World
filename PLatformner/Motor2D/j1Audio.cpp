#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"

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
	}
	//Change/FIX

	bat_sound = LoadFx("audio/FX/bat_flying.wav");

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
	Mix_UnregisterAllEffects(5); //Change/Fix D�dac
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
	if (hypotenuse > 230)
		hypotenuse = 230;
	
	//Work around function limitations
	int aux_distance = distance.x;

	if (aux_distance > 230)
		aux_distance = 230;
	else if (aux_distance < -230)
		aux_distance = 230;
	else if (aux_distance == 0)
		aux_distance = 1;
	else
		aux_distance = abs(aux_distance);

	//Set the Panning 
	uint left = 255;

	if (distance.x > 0)
		left = 127 - (aux_distance / 2);
	if (distance.x < 0)
		left = 127 + (abs(aux_distance / 2));

	//Play the Effect
	PlayFx(id,repeat,channel);
	//Process the information for the Effect
	Mix_SetDistance(channel, hypotenuse);			//Attenuation depending on distance
	Mix_SetPanning(channel, left, 255 - left);		//Panning (Right or Left speakers/headphones volumes)
}
