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
	bool CleanUp();


	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void mapBlit();
	void Disapearing();
	void DebugInputs();
	void PlayerInputs();
	void ComboEggs();
	

public:

	// Official 
	PhysBody* playerBall;
	p2List<PhysBody*> map_col;
	p2List<PhysBody*> bumpers;

	// Testing
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> flippers;

	PhysBody* BleftFlipper;
	PhysBody* BrightFlipper;

	b2RevoluteJoint* JleftFlipper;
	b2RevoluteJoint* JrightFlipper;

	b2PrismaticJoint* ballShooter;


	//Sensors
	//Official
	PhysBody* sensor;
	bool sensed;
	//Testing
	PhysBody* launcherToRail;
	PhysBody* railToRegular;

	SDL_Texture* map = nullptr;
	SDL_Texture* graphics = nullptr;
	SDL_Texture* circle;

	// Props
	Animation coin;

	// Scene Decoration
	Animation mapMonitor;

	// Animals Decoration
	Animation brownBambi;
	Animation blueBird;
	Animation pinkBunny;
	Animation blackSquirrel;
	Animation blueCupoSit;
	Animation blueCupoSleeping;
	Animation eggCupo;
	Animation eggCupo2;
	Animation monsterFrog;
	Animation animalSquirrel;
	Animation monsterBat;
	Animation animalBird;
	Animation monsterCrap;
	Animation animalElephant;
	Animation launcher;




	uint startingRoundFX = 0u;
	uint bouncingBumperFX = 0u;
	uint bouncingWallFX = 0u;
	uint flippersFX = 0u;
	uint boostFX = 0u;
	uint loseballFX = 0u;
	uint winRoundFX = 0u;
	uint loseRoundFX = 0u;
	uint ballDisapearFX = 0u;
	uint lifeWonFX = 0u;
	uint ballShooterFX = 0u;

	bool egg1 = true;
	bool egg2 = true;
	bool disapear = false;
	bool launcheable = false;
	uint disapearTimer = 0u;
	b2Vec2 disapearCoords = { 0,0 };


	p2Point<int> ray;
	bool ray_on;



private:

	bool mapDebug = false;
	bool reStart = false;



};
