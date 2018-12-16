#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "ElementGUI.h"
#include "j2GuiSprites.h"
#include "j2GUIText.h"
#include "j2ButtonClass.h"
#include "Brofiler/Brofiler.h"
#include "PugiXml/src/pugixml.hpp"
#include "j2SliderGUI.h"



j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	if (conf != NULL)
	{
		atlas_file_name = conf.child("atlas").attribute("file").as_string("");
		hover_sound_name = conf.child("sfx").attribute("file_hover_start").as_string("");  
		clicked_sound_name = conf.child("sfx").attribute("file_button_clicked").as_string("");
	}

	scale = App->win->GetScale();

	debug = false;
	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{

	atlas = App->tex->Load(atlas_file_name.GetString());

	hover_start = App->audio->LoadFx(hover_sound_name.GetString());
	button_clicked = App->audio->LoadFx(clicked_sound_name.GetString());
	
	/*SDL_Rect defaultRect = { 0,0,0,0 };
	SDL_Rect hoveringRect = { 646,170,226,64 };
	SDL_Rect clickedRect = { 416,170,226,64 };*/

	CreateMainMenuScreen();

	CreateSettingsScreen();
	
	Hide("Settings_Window");

	bool ret = true;
	for (p2List_item<ElementGUI*>* item = ElementList.start; item; item = item->next)
	{
		ret = item->data->Awake();
		if (!ret)
			break;
	}

	if (ret)
	{
		for (p2List_item<ElementGUI*>* item = ElementList.start; item; item = item->next)
		{
			ret = item->data->Start();
			if (!ret)
				break;
		}
	}

	return ret;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	BROFILER_CATEGORY("UI_PreUpdate", Profiler::Color::LightCoral);
	return true;
}

bool j1Gui::Update(float dt) {
	BROFILER_CATEGORY("UI_Update", Profiler::Color::Coral);
	bool ret = true;
	//Check interactivity with the elements
	for (p2List_item<ElementGUI*>* item = ElementList.end; item; item = item->prev)
	{

		if (item->data->interactable == true && item->data->invisible == false)
		{
			ret = item->data->InteractionUpdate();
			if (!ret || item->data->being_used == true)
				break;
		}
	}
	//Do their update
	for (p2List_item<ElementGUI*>* item = ElementList.start; item; item = item->next)
	{
		if (item->data->invisible == false)
		{
			ret = item->data->Update();
		}
	}


	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	BROFILER_CATEGORY("UI_PostUpdate", Profiler::Color::Salmon);
	bool ret = true;

	for (p2List_item<ElementGUI*>* item = ElementList.start; item; item = item->next)
	{
		if (item->data->invisible == false)
		{
			ret = item->data->PostUpdate();
			if (debug)
			{
				item->data->debugDraw();
			}
			if (!ret)
				break;
		}
	}


	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	bool ret = true;

	for (p2List_item<ElementGUI*>* item = ElementList.start; item; item = item->next)
	{
		ret = item->data->CleanUp();
		RELEASE(item->data);
		if (!ret)
			break;
	}

	ElementList.clear();

	return ret;

}


// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

ElementGUI*j1Gui::CreateElement(const char* name, ElementType element, ElementAction action, iPoint position, SDL_Texture* tex, bool interactable, SDL_Rect &rect1, SDL_Rect &rect2, SDL_Rect &rect3, ButtonType button, const char*Text, ElementGUI*Parent, bool draggable, bool invisible)
{

	ElementGUI*ElemGUI = nullptr;


	switch (element)
	{

	case ElementType::SPRITE:

		ElemGUI = new j2GuiSprites(name, element, action, position, rect1, true, tex, interactable, draggable, invisible);
		break;

	case ElementType::TEXT:

		ElemGUI = new j2GUIText(name, element, action, position, rect1, true, tex, Text, interactable, draggable, invisible);
		break;

	case ElementType::BUTTON:

		ElemGUI = new j2ButtonClass(name, element, action, button, position, rect1, rect2, rect3, true, tex, interactable, draggable, invisible);
		break;

	case ElementType::SLIDER:
		ElemGUI = new j2SliderGUI(name, element, action, position, rect1, rect2, true, tex, draggable, interactable, invisible);
		break;
	}

	ElemGUI->Parent = Parent;

	if (ElemGUI->Parent != nullptr)
		ElemGUI->Parent->children.add(ElemGUI);

	if (ElemGUI != nullptr)
		ElementList.add(ElemGUI);
	else
		LOG("ElemGUI is nullptr");


	return ElemGUI;
}

void j1Gui::CreateMainMenuScreen()
{
	SDL_Rect defaultRect = { 0,0,0,0 };
	SDL_Rect hoveringRect = { 646,170,226,64 };
	SDL_Rect clickedRect = { 416,170,226,64 };

	bool continueButton_Interactable = true;

	pugi::xml_document save_file;
	pugi::xml_parse_result res;

	res = save_file.load_file("save_game.xml");

	if (res == NULL)
	{
		continueButton_Interactable = false;
	}

	//Window
	iPoint testPoint = { 0,0 };
	SDL_Rect testRect = { 2, 396, 167, 185 };
	const char* PanelText = "Main_Menu";
	Panel = CreateElement("Main_Menu", ElementType::SPRITE, ElementAction::NONE, testPoint, atlas, false, testRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, nullptr, nullptr, false, true);

	/*iPoint textTestPoint = { 100,600 };
	SDL_Rect textTestRect = { 0,0, 50, 20 };
	const char*Text = "Main_Menu_Label";
	CreateElement(Text, ElementType::TEXT, ElementAction::NONE, textTestPoint, nullptr, false, textTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, Text, Panel, false);
*/

		//BUTTONS
	SDL_Rect unHoveredRect = { 950,441,123,37 };
	iPoint ButtonTestPoint = { 1400,600 };
	//Play
	ElementGUI* PlayButton = CreateElement("Play", ElementType::BUTTON, ElementAction::PLAY, ButtonTestPoint, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	iPoint RRtextTestPoint = { 50,20 };
	SDL_Rect RRtextTestRect = { 0,0, 100, 25 };
	const char* PlayText = "Play";
	CreateElement("Play_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, PlayText, PlayButton, false);

	//Continue
	iPoint ButtonTestPoint_2 = { 1400, 800 };
	ElementGUI* ContinueButton = CreateElement("Continue", ElementType::BUTTON, ElementAction::CONTINUE, ButtonTestPoint_2, atlas, continueButton_Interactable, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);


	const char* ContinueText = "Continue";
	CreateElement("Continue_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, ContinueText, ContinueButton, false);

	//Settings
	iPoint ButtonTestPoint_3 = { 100, 600 };
	ElementGUI* SettingsButton = CreateElement("Settings", ElementType::BUTTON, ElementAction::SETTINGS, ButtonTestPoint_3, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	const char* SettingsText = "Settings";
	CreateElement("Settings_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, SettingsText, SettingsButton, false);

	//Credits

		//Exit
	iPoint ButtonTestPoint_4 = { 1400, 1000 };
	ElementGUI* ExitButton = CreateElement("Exit", ElementType::BUTTON, ElementAction::EXIT, ButtonTestPoint_4, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	const char* ExitText = "Exit";
	CreateElement("Exit_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, ExitText, ExitButton, false);


}

void j1Gui::CreateSettingsScreen()
{
	SDL_Rect defaultRect = { 0,0,0,0 };
	SDL_Rect hoveringRect = { 646,170,226,64 };
	SDL_Rect clickedRect = { 416,170,226,64 };
	//Settings Screen
	iPoint testPoint = { 800,20 };
	SDL_Rect testRect = { 2, 396, 167, 185 };
	const char* PanelText = "Settings_Window";
	Panel = CreateElement(PanelText, ElementType::SPRITE, ElementAction::NONE, testPoint, atlas, true, testRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, nullptr, nullptr, false);

	//Settings Screen Text
	iPoint textTestPoint = { 170,50 };
	SDL_Rect textTestRect = { 0,0, 100, 20 };
	const char*Text = "Settings";
	CreateElement(Text, ElementType::TEXT, ElementAction::NONE, textTestPoint, nullptr, false, textTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, Text, Panel, false);

	//Music Slider
	iPoint SlidertestPos = { 100,300 };
	SDL_Rect unhoveredSlide = { 1278, 296, 6, 18 };
	SDL_Rect SlidertestRect = { 1251, 274, 131, 8 };
	volume_slider = CreateElement("Slider", ElementType::SLIDER, ElementAction::MUSIC_VOL, SlidertestPos, atlas, false, SlidertestRect, unhoveredSlide, hoveringRect, ButtonType::NOT_BUTTON, "None", Panel, false, false);

	//Music Slider Text

	//SFX Slider
	iPoint SlidertestPos_2 = { 100, 500 };
	CreateElement("Slider", ElementType::SLIDER, ElementAction::SFX_VOL, SlidertestPos_2, atlas, false, SlidertestRect, unhoveredSlide, hoveringRect, ButtonType::NOT_BUTTON, "None", Panel, false, false);

	//Music	Slider Text
}


void j1Gui::callbackUiElement(ElementGUI *element)
{
	if (element->type == ElementType::BUTTON)
	{
		if (element->was_clicked && element->clicked == false)
		{
			App->audio->PlayFx(App->gui->button_clicked);
		}
		
		if (element->hovering && element->was_hovered == false)
		{
			App->audio->PlayFx(App->gui->hover_start);
		}
	}
}

void j1Gui::Hide(const char* Window)
{
	for (p2List_item<ElementGUI*>* item = ElementList.start; item != nullptr; item = item->next)
	{
		if (item->data->name == Window)
		{
			RecursiveHide(item->data);
		}
	}
}

void j1Gui::RecursiveHide(ElementGUI* itemToHide)
{
	if (itemToHide->children.count() > 0)
	{
		for (p2List_item<ElementGUI*>* item = itemToHide->children.start; item != nullptr; item = item->next)
		{
			RecursiveHide(item->data);
		}
		itemToHide->invisible = true;
	}

	if (itemToHide->children.count() < 1)
	{
		itemToHide->invisible = true;
	}
}


void j1Gui::Display(const char* Window)
{
	for (p2List_item<ElementGUI*>* item = ElementList.start; item != nullptr; item = item->next)
	{
		if (item->data->name == Window)
		{
			RecursiveDisplay(item->data);
		}
	}
}

void j1Gui::RecursiveDisplay(ElementGUI* itemToDisplay)
{
	if (itemToDisplay->children.count() > 0)
	{
		for (p2List_item<ElementGUI*>* item = itemToDisplay->children.start; item != nullptr; item = item->next)
		{
			RecursiveDisplay(item->data);
		}
		itemToDisplay->invisible = false;
	}

	if (itemToDisplay->children.count() < 1)
	{
		itemToDisplay->invisible = false;
	}
}