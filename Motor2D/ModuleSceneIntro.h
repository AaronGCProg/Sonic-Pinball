#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "Globals.h"

class PhysBody;

//Struct to store the combos
struct lightsCombo
{
	lightsCombo(PhysBody* body, bool drawing) : physBody(body), draw (drawing)
	{};

	PhysBody* physBody;
	bool draw = false;

};

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
	bool ComboLolipops();
	void ReSetCombos();
	

public:

	// Physical Objects & Joints--------------------------- 
	//Ball
	PhysBody* playerBall;
	//Map Colliders
	p2List<PhysBody*> map_col;
	//Bumpers
	p2List<PhysBody*> bumpers;
	//Flippers
	PhysBody* BleftFlipper;
	PhysBody* BrightFlipper;
	//Flipper Joints
	b2RevoluteJoint* JleftFlipper;
	b2RevoluteJoint* JrightFlipper;
	//Ball Shooter
	b2PrismaticJoint* ballShooter;
	//Lolipops
	lightsCombo* lolipop1;
	lightsCombo* lolipop2;
	lightsCombo* lolipop3;


	//Textures-------------------------------------
	SDL_Texture* map = nullptr;
	SDL_Texture* graphics = nullptr;
	SDL_Texture* circle;

	//Animations--------------------------------
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



	//Sound effects--------------------------------
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
	uint comboEndFX = 0u;
	uint inComboFX = 0u;

	//Map Control----------------------------
	int scoreMultiplier = 1;


	//Ray-------------------------------------
	p2Point<int> ray;
	bool ray_on;



private:
	//Internal Control-------------------------------
	bool mapDebug = false;
	bool reStart = false;
	bool disapear = false;
	bool launcheable = false;
	bool egg1 = true;
	bool egg2 = true;
	uint disapearTimer = 0u;
	b2Vec2 disapearCoords = { 0,0 };


};
