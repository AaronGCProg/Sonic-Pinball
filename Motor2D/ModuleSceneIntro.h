#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void mapBlit();
	

public:

	// Official 
	p2List<PhysBody*> playerBall;
	p2List<PhysBody*> map_col;

	// Testing
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> flippers;


	PhysBody* sensor;
	bool sensed;

	SDL_Texture* map = nullptr;
	SDL_Texture* graphics = nullptr;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	Animation mapMonitor;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
