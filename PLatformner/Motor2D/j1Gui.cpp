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
#include "j2HealthBarGui.h"
#include "j2VarsPlayerGUI.h"
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
		licenseText = conf.child("License").attribute("text").as_string("");
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

	CreateCreditsScreen();

	CreatePlayerGui();

	Hide("Settings_Window");
	Hide("Credits_Window");
	Hide("InGameUI");

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
	case ElementType::HEALTH_BAR:

		ElemGUI = new j2HealthBarGui(name,element,action,position,true,tex, draggable, interactable, invisible);
		
		break;
	case ElementType::VARS_PLAYER:

		ElemGUI = new j2VarsPlayerGUI(name, element, action, position, true, tex,draggable, interactable, invisible);
		
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
	iPoint ButtonTestPoint_3 = { 50, 1400 };
	SDL_Rect SettingsButtonRect = {1103,283,25,25};
	CreateElement("Settings", ElementType::BUTTON, ElementAction::SETTINGS, ButtonTestPoint_3, atlas, true, SettingsButtonRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	//Credits
	iPoint ButtonTestPoint_4 = { 200, 1400 };
	SDL_Rect CreditsButtonRect = { 950,331,92,26 };
	ElementGUI* CreditsButton = CreateElement("Settings", ElementType::BUTTON, ElementAction::CREDITS, ButtonTestPoint_4, atlas, true, CreditsButtonRect, CreditsButtonRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	const char* CreditsText = "Credits";
	CreateElement("Credits_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, CreditsText, CreditsButton, false);

		//Exit
	iPoint ButtonTestPoint_5 = { 1400, 1000 };
	ElementGUI* ExitButton = CreateElement("Exit", ElementType::BUTTON, ElementAction::EXIT, ButtonTestPoint_5, atlas, true, CreditsButtonRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);

	const char* ExitText = "Exit";
	CreateElement("Exit_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, ExitText, ExitButton, false);


}

void j1Gui::CreateSettingsScreen()
{
	SDL_Rect defaultRect = { 0,0,0,0 };
	SDL_Rect hoveringRect = { 646,170,226,64 };
	SDL_Rect clickedRect = { 416,170,226,64 };
	//Settings Screen
	iPoint testPoint = { 400,20 };
	SDL_Rect testRect = { 468, 204, 236, 185 };
	const char* PanelText = "Settings_Window";
	Panel = CreateElement(PanelText, ElementType::SPRITE, ElementAction::NONE, testPoint, atlas, true, testRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, nullptr, nullptr, false);

	//Settings Screen Text
	iPoint textTestPoint = { 170,50 };
	SDL_Rect textTestRect = { 0,0, 150, 20 };
	const char*Text = "Settings";
	CreateElement(Text, ElementType::TEXT, ElementAction::NONE, textTestPoint, nullptr, false, textTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, Text, Panel, false);

	//Music Slider
	iPoint SlidertestPos = { 200,300 };
	SDL_Rect unhoveredSlide = { 1278, 296, 6, 18 };
	SDL_Rect SlidertestRect = { 1251, 274, 131, 8 };
	volume_slider = CreateElement("Music_Slider", ElementType::SLIDER, ElementAction::MUSIC_VOL, SlidertestPos, atlas, false, SlidertestRect, unhoveredSlide, hoveringRect, ButtonType::NOT_BUTTON, "None", Panel, false, false);

	//Music Slider Text
	iPoint musicSliderTestPoint = { 200, 230 };
	SDL_Rect MusicTestRect = { 0,0, 140, 20 };
	const char* MusicText = "Music Vol";
	CreateElement(MusicText, ElementType::TEXT, ElementAction::NONE, musicSliderTestPoint, nullptr, false, MusicTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, MusicText, Panel, false);

	//SFX Slider
	iPoint SlidertestPos_2 = { 200, 500 };
	CreateElement("FX_Slider", ElementType::SLIDER, ElementAction::SFX_VOL, SlidertestPos_2, atlas, false, SlidertestRect, unhoveredSlide, hoveringRect, ButtonType::NOT_BUTTON, "None", Panel, false, false);

	//Music	Slider Text
	iPoint SlidertestPos_3 = { 200, 400 };
	const char* SFXText = "FX Vol";
	CreateElement(SFXText, ElementType::TEXT, ElementAction::NONE, SlidertestPos_3, nullptr, false, textTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, SFXText, Panel, false);

	//Settings back button
	iPoint ButtonBackPos = { 400, 700};
	SDL_Rect idleButtonRect = {1204,283,25,25};
	CreateElement("Settings_Back", ElementType::BUTTON, ElementAction::SETTINGS_BACK, ButtonBackPos, atlas, true, idleButtonRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);
	
}

void j1Gui::CreateCreditsScreen()
{
	SDL_Rect defaultRect = { 0,0,0,0 };
	SDL_Rect hoveringRect = { 646,170,226,64 };
	SDL_Rect clickedRect = { 416,170,226,64 };

	iPoint testPoint = { 0,20 };
	SDL_Rect testRect = { 2, 2, 360, 185 };

	//Credits Window
	const char* PanelText = "Credits_Window";
	Panel = CreateElement(PanelText, ElementType::SPRITE, ElementAction::NONE, testPoint, atlas, true, testRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, nullptr, nullptr, false);

	iPoint LicenseTesxtPos = { 100, 170 };
	SDL_Rect LicenseRect = {0,0,600, 200};
	CreateElement("License", ElementType::TEXT, ElementAction::NONE, LicenseTesxtPos, nullptr, false, LicenseRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, licenseText, Panel, false);

	//Web
	iPoint ButtonTestPoint_5 = { 500, 1000 };
	SDL_Rect CreditsButtonRect = { 950,331,92,26 };
	ElementGUI* WebButton = CreateElement("Web", ElementType::BUTTON, ElementAction::WEB, ButtonTestPoint_5, atlas, true, CreditsButtonRect, hoveringRect, clickedRect, ButtonType::DEFAULT, nullptr, Panel, false, false);


	iPoint RRtextTestPoint = { 50,20 };
	SDL_Rect RRtextTestRect = { 0,0, 100, 25 };
	const char* WebText = "Web";
	CreateElement("Web_Label", ElementType::TEXT, ElementAction::NONE, RRtextTestPoint, nullptr, false, RRtextTestRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, WebText, WebButton, false);


}
void j1Gui::CreatePlayerGui() {

	SDL_Rect defaultRect = { 0,0,0,0 };
	iPoint testPoint = { 0,0 };
	SDL_Rect testRect = { 2, 396, 167, 185 };
	const char* PanelText = "InGameUI";
	Panel = CreateElement(PanelText, ElementType::SPRITE, ElementAction::NONE, testPoint, atlas, false, testRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, nullptr, nullptr, false, true);


	
	SDL_Rect HealthFrameRect = { 1082,409,161,52 };
	const char*HealthFrameBar = "HealthFrameBar";
	iPoint PosFrameHealth = { 0,1356 };
	
	HealthFrame=CreateElement(HealthFrameBar, ElementType::SPRITE, ElementAction::NONE, PosFrameHealth, atlas, false, HealthFrameRect, defaultRect, defaultRect,ButtonType::NOT_BUTTON,nullptr,Panel);

	iPoint PosHealth = {49,45};
	const char*HealthBar = "HealthBar";
	CreateElement(HealthBar, ElementType::HEALTH_BAR, ElementAction::NONE, PosHealth, atlas, false, defaultRect, defaultRect, defaultRect, ButtonType::NOT_BUTTON, "None", HealthFrame);


	iPoint CoinGUIPos = { 100,100 };
	const char*Coins = "Coins&Score";
	CreateElement(Coins, ElementType::VARS_PLAYER,ElementAction::NONE, CoinGUIPos,atlas,false,defaultRect, defaultRect, defaultRect,ButtonType::NOT_BUTTON, "None",Panel);

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

//Hide an element and all it's children (make them invisible)
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
//UnHide an element and all it's children (make them visible)
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