#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	//ApplyDistanceAttenuation
	void PlayEnvironmentalFx(unsigned int id, int channel, const iPoint& sound_emmiter, const iPoint& sound_listener, int repeat = 0);

	//UI Callback override
	void callbackUiElement(ElementGUI *element);
public: //Vars

	p2SString music_folder;
	p2SString Chunks_folder;
	//List containing the strings for each song
	p2List<p2SString*> songs_list;

	p2SString bat_fx_path;

	int bat_channel;
	int max_attenuation_distance;


	//Change/Fix Panning and Distance Testing.
	uint bat_sound;

private:

	_Mix_Music*			music = NULL;
	p2List<Mix_Chunk*>	fx;
};

#endif // __j1AUDIO_H__