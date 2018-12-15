#include "ElementGUI.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"

ElementGUI::ElementGUI(const char* name, ElementType Element, ElementAction action, iPoint position, SDL_Rect rect, bool isStatic, bool interactable, bool draggable, bool invisible, SDL_Texture*tex) : name(name), type(Element), action(action), position(position), rect(rect), tex(tex), isStatic(isStatic), draggable(draggable), invisible(invisible), interactable(interactable) {
	scale = App->gui->scale;
}


ElementGUI::~ElementGUI()
{
}

void ElementGUI::debugDraw()
{
	App->render->DrawQuad(InterRect, 255, 0, 0, 255, false, false);
}


bool ElementGUI::CleanUp()
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
	}

	return true;
}
