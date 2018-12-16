#include "j2ButtonClass.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"

j2ButtonClass::j2ButtonClass(const char* name, ElementType type, ElementAction action, ButtonType buttonType, iPoint position, SDL_Rect &rect1, SDL_Rect &rect2, SDL_Rect &rect3, bool isStatic, SDL_Texture* tex, bool interactable, bool draggable, bool invisible)
	: ElementGUI(name, type, action, position, rect1, isStatic, interactable, draggable, invisible, tex), bType(buttonType) {
	
	if (bType == ButtonType::DEFAULT)
	{
		hoveringRect = { 950,487,122,37 };
		clickedRect = { 950,541,122,37 };
	}
	else if (bType == ButtonType::ROUND || bType == ButtonType::SLIDER)
	{
		hoveringRect = rect;
		clickedRect = rect;
	}
	else if (bType == ButtonType::SMALL)
	{
		hoveringRect = { 950,366,92,26 };
		clickedRect = { 950,403,92,26 };
	}


}


j2ButtonClass::~j2ButtonClass()
{
}


bool j2ButtonClass::Awake() {

	position.x /= scale;
	position.y /= scale;

	

	if (Parent != nullptr) {
		GlobalPosition.x = Parent->GlobalPosition.x + position.x;
		GlobalPosition.y = Parent->GlobalPosition.y + position.y;

		InterRect.x = GlobalPosition.x;
		InterRect.y = GlobalPosition.y;
		InterRect.w = rect.w *scale;
		InterRect.h = rect.h *scale;

	}
	else
	{
		GlobalPosition.x = position.x;
		GlobalPosition.y = position.y;
		InterRect.x = GlobalPosition.x;
		InterRect.y = GlobalPosition.y;
		InterRect.w = rect.w * scale;
		InterRect.h = rect.h * scale;
	}

	if (draggable)
	{
		switch (bType)
		{
		case ButtonType::DEFAULT:
			draggable_x = true;
			draggable_y = true;
			break;
		case ButtonType::SLIDER:
			draggable_x = true;
			draggable_y = false;
			break;
		}
	}


	return true;
}

//Start
bool j2ButtonClass::Start() {

	was_hovered = false;
	was_clicked = false;
	clicked = false;
	hovering = false;
	dragging = false;

	return true;
}

//PreUpdate		
bool j2ButtonClass::PreUpdate() {

	return true;
}

//Update		
bool j2ButtonClass::Update() {

	LastMousePos = MousePos;
	App->input->GetMousePosition(MousePos.x, MousePos.y);

	UpdatePos();
	return true;
}

//PostUpdate	
bool j2ButtonClass::PostUpdate() {
	DisplayButton();
	return true;
}

void j2ButtonClass::DisplayButton()
{
	//FIX must update functions and modules
	if (clicked)
		App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &clickedRect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	else if (hovering)
		App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &hoveringRect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
	else
		App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &rect, SDL_FLIP_NONE, 1.0f, 0.0, 0, 0, !isStatic);
}

bool j2ButtonClass::InteractionUpdate()
{
	if (hovering != was_hovered)
	{
		was_hovered = hovering;
	}

	if (clicked != was_clicked)
	{
		was_clicked = clicked;
	}

	if (!(MousePos.x < InterRect.x /scale || MousePos.x > (InterRect.x + InterRect.w) /scale || MousePos.y < InterRect.y /scale|| MousePos.y >(InterRect.y + InterRect.h)/scale))
	{
		hovering = true;
	}
	else
	{
		hovering = false;
		clicked = false;
	}

	if (hovering == true)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			clicked = true;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			clicked = false;
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (draggable == true && clicked == true)
			{
				dragging = true;
			}
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE)
	{
		dragging = false;
	}

	if (hovering || clicked || dragging)
	{
		being_used = true;

		//Once we know everything that is happening (and we are interacting with the button 
		//(hovering is necessary for any other action on the button)) we can pass that info to the modules
		
		App->UiElementCallback(this);
	}
	else
	{
		being_used = false;
	}
	//App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE;

	return true;
}

void j2ButtonClass::UpdatePos()
{
	if (Parent != nullptr) {
		if (dragging == true)
		{
			if (draggable_x)
				position.x = MousePos.x *scale - (LastMousePos.x *scale - (InterRect.x -Parent->InterRect.x));

			if (draggable_y)
				position.y = MousePos.y * scale- (LastMousePos.y *scale - (InterRect.y - Parent->InterRect.y /scale));
		}
		GlobalPosition.x = Parent->GlobalPosition.x + position.x;
		GlobalPosition.y = Parent->GlobalPosition.y + position.y;
	}
	else
	{
		if (dragging == true)
		{
			if (draggable_x == true)
				position.x = MousePos.x *scale - (LastMousePos.x *scale - InterRect.x);

			if (draggable_y == true)
				position.y = MousePos.y *scale - (LastMousePos.y *scale - InterRect.y);
		}

		GlobalPosition.x = position.x;
		GlobalPosition.y = position.y;

	}

	if (bType == ButtonType::SLIDER && Parent != nullptr)
	{
		if (GlobalPosition.x + InterRect.w > Parent->GlobalPosition.x + Parent->rect.w *scale)
			GlobalPosition.x = Parent->GlobalPosition.x + Parent->rect.w * scale - InterRect.w;

		if (GlobalPosition.x < Parent->GlobalPosition.x)
			GlobalPosition.x = Parent->GlobalPosition.x;

	}

	InterRect.x = GlobalPosition.x;
	InterRect.y = GlobalPosition.y;

}
