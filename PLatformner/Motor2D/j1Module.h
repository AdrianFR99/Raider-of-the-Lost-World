// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;

struct Collider; //Remember this is to avoid problems as j1Module.h is created before the struct collider

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each frame(for logic)
	virtual bool UpdateTicks(float dt)
	{
		return true;
	}


	// Called each loop iteration(for graphics)
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}
	//Testing
	


	// Callbacks ---
	virtual void OnCollision(Collider* c1, Collider* c2) {}
	virtual void OnPreCollision(int d) {}

public:

	p2SString	name;
	bool		active;
	bool enabled = true;
};

#endif // __j1MODULE_H__