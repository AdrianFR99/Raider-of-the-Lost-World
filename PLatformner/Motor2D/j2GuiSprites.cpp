#include "j2GuiSprites.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"


j2GuiSprites::j2GuiSprites(const char* name, ElementType type, ElementAction action, iPoint position, SDL_Rect &rect, bool isStatic, SDL_Texture* tex, bool draggable, bool interactuable, bool invisible)
	: ElementGUI(name, type, action, position, rect, isStatic, interactuable, draggable, invisible, tex) {


}


j2GuiSprites::~j2GuiSprites()
{
}



bool j2GuiSprites::Awake() {

	position.x /= scale;
	position.y /= scale;

	if (Parent != nullptr) {
		GlobalPosition.x = Parent->GlobalPosition.x + position.x;
		GlobalPosition.y = Parent->GlobalPosition.y + position.y;
	}
	else
	{
		GlobalPosition.x = position.x;
		GlobalPosition.y = position.y;
	}
	InterRect.x = GlobalPosition.x;
	InterRect.y = GlobalPosition.y;
	InterRect.w = rect.w * scale;
	InterRect.h = rect.h * scale;


	return true;
}
//Start
bool j2GuiSprites::Start() {

	was_hovered = false;
	was_clicked = false;
	clicked = false;
	hovering = false;
	dragging = false;

	return true;
}
//PreUpdate		
bool j2GuiSprites::PreUpdate() {

	return true;
}
//Update		
bool j2GuiSprites::Update() {
	LastMousePos = MousePos;
	App->input->GetMousePosition(MousePos.x, MousePos.y);
	/*MousePos.x /= scale;
	MousePos.y /= scale;*/

	UpdatePos();

	return true;
}
//PostUpdate	
bool j2GuiSprites::PostUpdate() {
	
	DisplaySprite();
	return true;
}


bool j2GuiSprites::InteractionUpdate()
{
	if (hovering != was_hovered)
	{
		was_hovered = hovering;
	}

	if (clicked != was_clicked)
	{
		was_clicked = clicked;
	}

	if (!(MousePos.x < InterRect.x /scale|| MousePos.x > (InterRect.x + InterRect.w) /scale || MousePos.y < InterRect.y / scale || MousePos.y > (InterRect.y + InterRect.h)/scale))
	{
		hovering = true;
	}
	else
	{
		hovering = false;
		clicked = false;
	}

	if (hovering)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			clicked = true;
			if (draggable)
			{
				dragging = true;
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			clicked = false;
		}

		//Once we know everything that is happening (and we are interacting with the button 
		//(hovering is necessary for any other action on the button)) we can pass that info to the modules
		
		
		App->UiElementCallback(this);
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && dragging == true)
	{
		dragging = false;
	}

	return true;
}

void j2GuiSprites::UpdatePos()
{
	if (Parent != nullptr) {
		if (dragging == true)
		{
			position.x = MousePos.x * scale - (LastMousePos.x * scale - Parent->InterRect.x);
			position.y = MousePos.y * scale - (LastMousePos.y * scale - Parent->InterRect.y);
		}
		GlobalPosition.x = Parent->GlobalPosition.x + position.x;
		GlobalPosition.y = Parent->GlobalPosition.y + position.y;
	}
	else
	{
		if (dragging == true)
		{
			position.x = MousePos.x * scale - (LastMousePos.x *scale - InterRect.x);
			position.y = MousePos.y *scale- (LastMousePos.y *scale - InterRect.y);
		}

		GlobalPosition.x = position.x;
		GlobalPosition.y = position.y;
	}

	InterRect.x = GlobalPosition.x;
	InterRect.y = GlobalPosition.y;
}



void j2GuiSprites::DisplaySprite() {

	//FIX must update functions and modules
	App->render->Blit(tex, GlobalPosition.x, GlobalPosition.y, &rect, SDL_FLIP_NONE,1.0f,0.0,0,0,!isStatic);

}