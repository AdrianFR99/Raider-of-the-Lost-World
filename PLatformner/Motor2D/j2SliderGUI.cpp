#include "j2SliderGUI.h"
#include "j1Gui.h"
#include "j1App.h"


j2SliderGUI::j2SliderGUI(const char* name, ElementType Element, ElementAction action, iPoint position, SDL_Rect rect, SDL_Rect buttonRect, bool isStatic, SDL_Texture*tex, bool draggable, bool interactable, bool invisible)
	: ElementGUI(name, Element, action, position, rect, isStatic, draggable, interactable, invisible, tex)
{
	SDL_Rect defRect = { 0,0,0,0 };
	ElementGUI* SliderImage = App->gui->CreateElement(name, ElementType::SPRITE, ElementAction::NONE, { 0,0 }, tex, true, rect, defRect, defRect, ButtonType::NOT_BUTTON, nullptr, this, false);
	App->gui->CreateElement(name, ElementType::BUTTON, action, { 0,0 - buttonRect.h / 2 }, tex, true, buttonRect, buttonRect, defRect, ButtonType::SLIDER, nullptr, this, true);

	was_hovered = false;
	was_clicked = false;
	clicked = false;
	hovering = false;
	dragging = false;
}


j2SliderGUI::~j2SliderGUI()
{
}

bool j2SliderGUI::Start()
{
	length = rect.w - children.end->data->InterRect.w;
	slider_unitVal = float(1000 / length);
	slider_unitVal *= 0.001;
	return true;
}

bool j2SliderGUI::PreUpdate()
{

	return true;
}

bool j2SliderGUI::Update()
{
	//If horizontal
	slider_value = slider_unitVal * (children.end->data->GlobalPosition.x - children.start->data->GlobalPosition.x);

	//As we needed to do an aproximation of the  slider_unitVal because it can be a really small value we will 
	if (slider_value > 0.98f)
	{
		slider_value = ceil(slider_value);
	}
	//If vertical

	UpdatePos();
	return true;
}

bool j2SliderGUI::PostUpdate()
{
	return true;
}


void j2SliderGUI::UpdatePos()
{
	if (Parent != nullptr) {
		if (dragging == true)
		{
			position.x = MousePos.x - (LastMousePos.x - Parent->InterRect.x);
			position.y = MousePos.y - (LastMousePos.y - Parent->InterRect.y);
		}
		GlobalPosition.x = Parent->GlobalPosition.x + position.x;
		GlobalPosition.y = Parent->GlobalPosition.y + position.y;
	}
	else
	{
		if (dragging == true)
		{
			position.x = MousePos.x - (LastMousePos.x - InterRect.x);
			position.y = MousePos.y - (LastMousePos.y - InterRect.y);
		}

		GlobalPosition.x = position.x;
		GlobalPosition.y = position.y;
	}

	InterRect.x = GlobalPosition.x;
	InterRect.y = GlobalPosition.y;
}
