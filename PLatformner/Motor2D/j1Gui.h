#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2SString.h"
#include "p2List.h"
#include "p2Point.h"


#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

// ---------------------------------------------------

class ElementGUI;
class j2SliderGUI;
struct SDL_Texture;
struct SDL_Rect;


enum class ElementType
{
	BUTTON,
	TEXT,
	SPRITE,
	SLIDER,
	HEALTH_BAR,
	VARS_PLAYER,
	NOT_ELEMENT

};

enum class ButtonType {

	CHECKLIST,
	DEFAULT,
	SLIDER,
	ROUND,
	SMALL,
	NOT_BUTTON


};

enum class ElementAction
{
	NONE = -1,
	PLAY,
	CONTINUE,
	SETTINGS,
	SETTINGS_BACK,
	SETTINGS_RESUME,
	CREDITS,
	CREDITS_BACK,
	EXIT,
	MUSIC_VOL,
	SFX_VOL,
	WEB,
	BACK_TO_MENU,
	

};

class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	//Called to Update
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Functions to Hide and Display hidden
	void Hide(const char* Window);
	void RecursiveHide(ElementGUI* itemToHide);

	void Display(const char* Window);
	void RecursiveDisplay(ElementGUI* itemToDisplay);


	// Gui creation functions
	const SDL_Texture* GetAtlas() const;

	ElementGUI*CreateElement(const char* name, ElementType element, ElementAction action, iPoint position, SDL_Texture* tex, bool interactable, SDL_Rect &rect1, SDL_Rect &rect2, SDL_Rect &rect3, ButtonType button = ButtonType::NOT_BUTTON, const char*Text = nullptr, ElementGUI*Parent = nullptr, bool draggable = false, bool invisible = false);

	//Functions to create the different screens
	void CreateMainMenuScreen();
	void CreateSettingsScreen();

	void CreatePlayerGui();

	void CreateCreditsScreen();

	void CreateInGameScreen();


	//UI Callbacks
	void callbackUiElement(ElementGUI *element);

public:
	p2List<ElementGUI*> ElementList;

	bool debug;

	uint scale;

	//Button sounds
	uint hover_start;
	uint button_clicked;

	ElementGUI* volume_slider = nullptr;

	ElementGUI*Panel;
	ElementGUI*HealthFrame;
	
	const char* licenseText = nullptr;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2SString hover_sound_name;
	p2SString clicked_sound_name;

};

#endif // __j1GUI_H__
