#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "ElementGUI.h"
#include "j2GuiSprites.h"
#include "j2GUIText.h"
#include "j2ButtonClass.h"
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

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	debug = false;
	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	scale = App->win->GetScale();

	atlas = App->tex->Load("gui/atlas.png");
	game_atlas = App->tex->Load("gui/atlas_GUI.png");
	
	SDL_Rect defaultRect = { 0,0,0,0 };
	SDL_Rect hoveringRect = { 646,170,226,64 };
	SDL_Rect clickedRect = { 416,170,226,64 };

	iPoint testPoint = { 100,50 };
	SDL_Rect testRect = { 2, 396, 167, 185 };
	const char* PanelText = "Window";
	Panel = CreateElement(PanelText, ElementType::SPRITE, ElementAction::NONE, testPoint, game_atlas, true, testRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, nullptr, nullptr, true);

	
	iPoint textTestPoint = { 190,20 };
	SDL_Rect textTestRect = { 0,0, 50, 20 };
	const char*Text = "Window";
	CreateElement(Text, ElementType::TEXT, ElementAction::NONE, textTestPoint, nullptr, false, textTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, Text, Panel, false);


	SDL_Rect unHoveredRect = { 2,112,226,64 };
	/*iPoint ButtonTestPoint = { 100,100 };

	CreateElement("Fade_Button", ElementType::BUTTON, ElementAction::FADE, ButtonTestPoint, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	ElementGUI*ElemGUI2 = nullptr;
	iPoint ButtonTestPoint_2 = { 100, 200 };
	ElementGUI* RRButton = CreateElement("Rick Roll", ElementType::BUTTON, ElementAction::RICK_ROLL, ButtonTestPoint_2, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	ButtonTestPoint_2 = { 100, 300 };
	ElementGUI* RRButton2 = CreateElement("Rick Roll", ElementType::BUTTON, ElementAction::RICK_ROLL, ButtonTestPoint_2, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	ButtonTestPoint_2 = { 100, 400 };
	ElementGUI* RRButton3 = CreateElement("Rick Roll", ElementType::BUTTON, ElementAction::RICK_ROLL, ButtonTestPoint_2, atlas, true, unHoveredRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	iPoint RRtextTestPoint = { 50,20 };
	SDL_Rect RRtextTestRect = { 0,0, 100, 25 };
	const char* RRText = "Rick_Roll_LABEL";
	CreateElement(RRText, ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, RRText, RRButton, false);
*/
	iPoint SlidertestPos = { 500, 1200 };
	SDL_Rect SlidertestRect = { 0, 12, 308, 12 };
	CreateElement("Slider", ElementType::SLIDER, ElementAction::MUSIC_VOL, SlidertestPos, atlas, false, SlidertestRect, unHoveredRect, hoveringRect, ButtonType::NOT_BUTTON, "None", nullptr, false, false);


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
	return true;
}

bool j1Gui::Update(float dt) {

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