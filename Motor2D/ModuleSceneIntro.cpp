#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "MapChains.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
	ray_on = false;

	//Animation Load ------------------------------------------------------
	coin.PushBack({ 372, 158, 16, 16 });
	coin.PushBack({ 392, 158, 11, 16 });
	coin.PushBack({ 407, 158, 4, 16 });
	coin.PushBack({ 415, 158, 11, 16 });
	coin.speed = 0.15f;

	mapMonitor.PushBack({ 0, 509, 48, 55 });
	mapMonitor.PushBack({ 52, 509, 48, 55 });
	mapMonitor.PushBack({ 107, 509, 48, 55 });
	mapMonitor.PushBack({ 52, 509, 48, 55 });
	mapMonitor.speed = 0.15f;

	eggCupo.PushBack({ 496, 164, 7, 7 });
	eggCupo.PushBack({ 546, 150, 21, 31 });
	eggCupo.PushBack({ 570, 148, 22, 32 });
	eggCupo.PushBack({ 595, 147, 22, 33 });
	eggCupo.PushBack({ 620, 147, 20, 33 });
	eggCupo.PushBack({ 595, 147, 22, 33 });
	eggCupo.PushBack({ 620, 147, 20, 33 });
	eggCupo.PushBack({ 643, 151, 19, 28 });
	eggCupo.PushBack({ 665, 149, 18, 29 });
	eggCupo.PushBack({ 686, 146, 20, 33 });
	eggCupo.speed = 0.015f;

	brownBambi.PushBack({ 551, 439, 22, 30 });
	brownBambi.PushBack({ 576, 439, 20, 30 });
	brownBambi.PushBack({ 599, 441, 22, 28 });
	brownBambi.PushBack({ 624, 440, 18, 29 });
	brownBambi.PushBack({ 645, 444, 23, 25 });
	brownBambi.speed = 0.15f;

	pinkBunny.PushBack({ 679, 383, 13, 24 });
	pinkBunny.PushBack({ 695, 384, 14, 23 });
	pinkBunny.PushBack({ 712, 385, 13, 22 });
	pinkBunny.PushBack({ 728, 384, 13, 23 });
	pinkBunny.speed = 0.15f;

	blackSquirrel.PushBack({ 630, 387, 12, 20 });
	blackSquirrel.PushBack({ 644, 388, 13, 19 });
	blackSquirrel.PushBack({ 660, 389, 13, 18 });
	blackSquirrel.speed = 0.15f;

	blueBird.PushBack({ 676, 443, 22, 26 });
	blueBird.PushBack({ 701, 443, 19, 26 });
	blueBird.PushBack({ 723, 443, 19, 26 });
	blueBird.speed = 0.15f;

	blueCupoSit.PushBack({ 458, 104, 22, 33 });
	blueCupoSit.PushBack({ 482, 103, 21, 34 });
	blueCupoSit.PushBack({ 506, 102, 21, 35 });
	blueCupoSit.PushBack({ 530, 105, 20, 30 });
	blueCupoSit.PushBack({ 553, 102, 20, 32 });
	blueCupoSit.speed = 0.15f;

	blueCupoSleeping.PushBack({ 576, 111, 30, 26 });
	blueCupoSleeping.PushBack({ 609, 109, 28, 28 });
	blueCupoSleeping.PushBack({ 640, 109, 28, 28 });
	blueCupoSleeping.PushBack({ 671, 108, 26, 29 });
	blueCupoSleeping.PushBack({ 700, 108, 26, 29 });
	blueCupoSleeping.speed = 0.01f;

	monsterFrog.PushBack({ 17, 380, 14, 12 });
	monsterFrog.PushBack({ 34, 379, 13, 13 });
	monsterFrog.PushBack({ 50, 374, 16, 18 });
	monsterFrog.PushBack({ 69, 375, 19, 14 });
	monsterFrog.speed = 0.10f;

	monsterCrap.PushBack({ 95, 375, 21, 17 });
	monsterCrap.PushBack({ 120, 374, 21, 18 });
	monsterCrap.PushBack({ 145, 375, 21, 17 });
	monsterCrap.speed = 0.10f;

	monsterBat.PushBack({ 263, 374, 26, 20 });
	monsterBat.PushBack({ 292, 377, 24, 17 });
	monsterBat.PushBack({ 319, 377, 20, 17 });
	monsterBat.speed = 0.10f;

	animalBird.PushBack({ 259, 405, 15, 11 });
	animalBird.PushBack({ 277, 405, 15, 10 });
	animalBird.PushBack({ 295, 405, 15, 10 });
	animalBird.speed = 0.10f;

	animalSquirrel.PushBack({ 7, 402, 12, 15 });
	animalSquirrel.PushBack({ 23, 403, 12, 14 });
	animalSquirrel.PushBack({ 39, 402, 11, 15 });
	animalSquirrel.speed = 0.10f;

	animalElephant.PushBack({ 106, 403, 15, 14 });
	animalElephant.PushBack({ 124, 403, 13, 14 });
	animalElephant.PushBack({ 141, 403, 11, 14 });
	animalElephant.speed = 0.10f;

	launcher.PushBack({ 383, 431, 16, 7 });
	launcher.PushBack({ 900, 900, 16, 7 });
	launcher.speed = 0.1f;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Game Graphic Resources
	map = App->textures->Load("pinball/map_spritesheet.png");
	graphics = App->textures->Load("pinball/general_spritesheet.png");

	// Game Audio Resources
	bouncingBumperFX = App->audio->LoadFx("pinball/audio/bumperEffect.wav");
	bouncingWallFX = App->audio->LoadFx("pinball/audio/bouncingWall2.wav");
	startingRoundFX = App->audio->LoadFx("pinball/audio/startingRound.wav");
	flippersFX = App->audio->LoadFx("pinball/audio/flipperEffect.wav");
	boostFX = App->audio->LoadFx("pinball/audio/speedupBall.wav");
	loseballFX = App->audio->LoadFx("pinball/audio/losingBall.wav");
	winRoundFX = App->audio->LoadFx("pinball/audio/highScoreEffect2.wav");
	loseRoundFX = App->audio->LoadFx("pinball/audio/loseEffect.wav");
	ballDisapearFX = App->audio->LoadFx("pinball/audio/doorEffect3.wav");
	lifeWonFX = App->audio->LoadFx("pinball/audio/highScoreEffect1.wav");
	ballShooterFX = App->audio->LoadFx("pinball/audio/launchingBall3.wav");

	// Game Music
	App->audio->PlayMusic("pinball/audio/ost/angel_island_loop.ogg");

	// Map Collisions & Sensors----------------------------------

	//General Map
	map_col.add(App->physics->CreateChain(0, 0, blue_triangle, 18, true, COLLIDER_WALL, BALL, REGULAR_MAP));
	map_col.add(App->physics->CreateChain(0, 0, general_map_right, 74, true, COLLIDER_WALL, BALL, REGULAR_MAP));
	map_col.add(App->physics->CreateChain(0, 0, map_left_enter, 14, true, COLLIDER_WALL, BALL, REGULAR_MAP));
	map_col.add(App->physics->CreateChain(0, 0, general_map_left, 78, true, COLLIDER_WALL, BALL, REGULAR_MAP));


	//Ball Shooter
	map_col.add(App->physics->CreateChain(0, 0, map_starter_ballShooter, 14, true, COLLIDER_WALL, BALL, REGULAR_MAP));
	App->physics->CreateRectangleSensor(242, 140, 5, 5, COLLIDER_BALLTORAIL, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(242, 385, 5, 5, COLLIDER_LAUNCHER, BALL, REGULAR_MAP);

	//Right Rail
	map_col.add(App->physics->CreateChain(0, 0, map_right_rail, 94, true, COLLIDER_WALL, RAIL_BALL, RAIL));
	App->physics->CreateRectangleSensor(190, 309, 5, 5, COLLIDER_RAILTOBALL, RAIL_BALL, RAIL);

	//Right Rail Right Entrance
	map_col.add(App->physics->CreateChain(0, 0, map_right_rail_rightEnter, 30, true, COLLIDER_WALL, RAIL_BALL_ENTRANCE, RAIL_ENTRANCE));
	App->physics->CreateRectangleSensor(195, 145, 5, 5, COLLIDER_BALLTOENTRANCE, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(240, 74, 5, 5, COLLIDER_ENTRANCETORAIL, RAIL_BALL_ENTRANCE, RAIL_ENTRANCE);

	//Right Rail Left Entrance
	map_col.add(App->physics->CreateChain(0, 0, map_right_rail_leftEnter, 44, true, COLLIDER_WALL, RAIL_BALL_ENTRANCE, RAIL_ENTRANCE));
	App->physics->CreateRectangleSensor(92, 121, 5, 5, COLLIDER_BALLTOENTRANCE, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(160, 94, 5, 5, COLLIDER_ENTRANCETORAIL, RAIL_BALL_ENTRANCE, RAIL_ENTRANCE);

	//Left Rail
	map_col.add(App->physics->CreateChain(0, 0, map_rail_left, 64, true, COLLIDER_WALL, RAIL_BALL, RAIL));
	App->physics->CreateRectangleSensor(43, 120, 5, 5, COLLIDER_BALLTOENTRANCETORAIL, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(42, 312, 5, 5, COLLIDER_RAILTOBALL, RAIL_BALL, RAIL);


	//Entrance Walls
	map_col.add(App->physics->CreateChain(0, 0, wall1, 16, true, COLLIDER_WALL, BALL, REGULAR_MAP));


	//Map Shapes
	map_col.add(App->physics->CreateChain(0, 0, genericColMap1, 90, true, COLLIDER_WALL, BALL, REGULAR_MAP));
	map_col.add(App->physics->CreateChain(0, 0, genericColMap2, 18, true, COLLIDER_WALL, BALL, REGULAR_MAP));

	//Death Sensor
	App->physics->CreateRectangleSensor(250, 460, 500, 50, COLLIDER_DEATH, BALL, REGULAR_MAP);

	//Egg Sensors
	App->physics->CreateRectangleSensor(103, 172, 15, 15, COLLIDER_EGG_1, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(17, 227, 15, 15, COLLIDER_EGG_NONE, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(68, 110, 15, 15, COLLIDER_EGG_2, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(59, 59, 15, 15, COLLIDER_EGG_NONE, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(221, 14, 15, 15, COLLIDER_EGG_NONE, BALL, REGULAR_MAP);

	//Lolipop Sensors
	App->physics->CreateRectangleSensor(143, 29, 10, 10, COLLIDER_LOLIPOP, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(172, 37, 15, 15, COLLIDER_LOLIPOP, BALL, REGULAR_MAP);
	App->physics->CreateRectangleSensor(200, 45, 15, 15, COLLIDER_LOLIPOP, BALL, REGULAR_MAP);


	//Physical Objects-----------------------------------

	//Bumpers
	bumpers.add(App->physics->CreateChain(0, 0, initialBumpers1, 6, true, COLLIDER_BOUNCER, BALL, REGULAR_MAP));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateChain(0, 0, initialBumpers2, 6, true, COLLIDER_BOUNCER, BALL, REGULAR_MAP));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateChain(0, 0, aloneBumper, 16, true, COLLIDER_BOUNCER, BALL, REGULAR_MAP));
	bumpers.getLast()->data->listener = this;
	bumpers.add(App->physics->CreateCircle(160, 82, 12, 0.00f, true, COLLIDER_BOUNCER, BALL, REGULAR_MAP));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateCircle(195, 90, 12, 0.00f, true, COLLIDER_BOUNCER, BALL, REGULAR_MAP));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateCircle(158, 118, 12, 0.00f, true, COLLIDER_BOUNCER, BALL, REGULAR_MAP));
	bumpers.getLast()->data->listener = this;


	//Flippers
	BleftFlipper = App->physics->CreateFlipper(86, 371, FL_LEFT, {24,6},COLLIDER_GENERAL,BALL,REGULAR_MAP);
	BleftFlipper->listener = this;
	JleftFlipper = App->physics->flipperJoints.getLast()->data;

	BrightFlipper = App->physics->CreateFlipper(149, 371, FL_RIGHT, { 24,5 }, COLLIDER_GENERAL, BALL, REGULAR_MAP);
	BrightFlipper->listener = this;
	JrightFlipper = App->physics->flipperJoints.getLast()->data;

	//Ball Shooter
	ballShooter = App->physics->CreateBallShooter(232, 406, 20, 16, COLLIDER_BALL_SHOOTER, BALL, REGULAR_MAP);


	return ret;
}

// Unloads assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(map);
	App->textures->Unload(graphics);


	Mix_FadeOutMusic(200);

	return true;
}
//Controls the internal gameplay
update_status ModuleSceneIntro::PreUpdate()
{
	//Called when a ball is lost, needs to be called before worldStep
	if (reStart)
	{
		App->player->AdjustLife(-1);
		App->player->ReStartGame();
		reStart = false;
	}

	//Check if there's a ball in the launch area , needs to be called before worldStep
	if(launcheable && METERS_TO_PIXELS(playerBall->body->GetPosition().x) < SCREEN_HEIGHT/2)
		launcheable = false;

	//Controls when the ball disapears
	if (disapear)
		Disapearing();

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	DebugInputs();

	PlayerInputs(); 

	//Blits map
	if (!mapDebug)
		mapBlit();

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::DebugInputs()
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->physics->debug = !App->physics->debug;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		mapDebug = !mapDebug;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}
}

void ModuleSceneIntro::PlayerInputs()
{
	//Left Trigger

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(flippersFX);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		JleftFlipper->SetMotorSpeed(25);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		JleftFlipper->SetMotorSpeed(-30);
	}


	//Right Trigger

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(flippersFX);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		JrightFlipper->SetMotorSpeed(-25);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		JrightFlipper->SetMotorSpeed(30.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && launcheable)
	{
		ballShooter->SetMotorSpeed(-10000.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE)
	{
		ballShooter->SetMotorSpeed(1000.0f);
	}


}


//Control of the ball when it goes through a tunel
void ModuleSceneIntro::Disapearing()
{
	if (disapearCoords.x == 0)
	{
		disapearCoords = playerBall->body->GetPosition();
		App->audio->PlayFx(ballDisapearFX);
	}
	disapearTimer++;


	App->scene_intro->playerBall->body->SetTransform({ PIXEL_TO_METERS(1000),PIXEL_TO_METERS(1000) }, 0.0f);

	if (disapearTimer > 30)
	{
		disapearTimer = 0;
		playerBall->body->SetTransform({ disapearCoords.x + PIXEL_TO_METERS(5), disapearCoords.y + PIXEL_TO_METERS(5) }, 0.0f);
		App->scene_intro->playerBall->body->SetAngularVelocity(0.0f);
		App->scene_intro->playerBall->body->SetLinearVelocity({ 0,0 });
		App->audio->PlayFx(ballDisapearFX);
		disapear = false;
		disapearCoords = { 0,0 };
	}


}


//Controls the map collision
void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	switch (bodyB->colType)
	{
	case COLLIDER_BOUNCER: //when the ball hits a bouncer
	{
		bodyA->body->GetContactList()->contact->SetRestitution(1.3f);
		App->audio->PlayFx(bouncingBumperFX);
		App->player->AddScore(35);
		break;
	}
	case COLLIDER_BALLTORAIL: //when the ball enters a rail
	{
		//We change it's mask and category in order to not collide with the rest of the map
		bodyA->body->GetFixtureList()->SetFilterData({ RAIL_BALL, RAIL });
		break;
	}
	case COLLIDER_RAILTOBALL: //when the ball leaves a rail
	{
		bodyA->body->GetFixtureList()->SetFilterData({ BALL, REGULAR_MAP });
		bodyA->body->SetLinearVelocity(b2Vec2(0, 0));
		bodyA->body->SetAngularVelocity(0);
		break;
	}

	case COLLIDER_BALLTOENTRANCE: //when the ball enters a rail from the map
	{
		bodyA->body->GetFixtureList()->SetFilterData({ RAIL_BALL_ENTRANCE, RAIL_ENTRANCE });
		App->audio->PlayFx(boostFX);

		//Calculate it's boost depending on it's speed in entering
		int speedX = 0;
		if (bodyA->body->GetPosition().x < bodyA->body->GetWorldCenter().x)
			speedX = -5;
		else
			speedX = 5;

		App->player->AddScore(25);
		bodyA->body->SetLinearVelocity(b2Vec2(speedX, -10));
		break;
	}

	case COLLIDER_BALLTOENTRANCETORAIL: //When the ball enters to the left rail of the nmap
	{
		bodyA->body->GetFixtureList()->SetFilterData({ RAIL_BALL, RAIL });

		int speedX = 0;
		if (bodyA->body->GetPosition().x < bodyA->body->GetWorldCenter().x)
			speedX = -5;
		else
			speedX = 5;

		App->audio->PlayFx(boostFX);

		App->player->AddScore(25);
		bodyA->body->SetLinearVelocity(b2Vec2(speedX, -10));
		break;
	}

	case COLLIDER_ENTRANCETORAIL: //When the ball enters the rail from an entrance in the map
	{
		bodyA->body->GetFixtureList()->SetFilterData({ RAIL_BALL, RAIL });
		break;
	}

	case COLLIDER_DEATH: //When the ball falls from the stage
	{
		reStart = true;
		App->audio->PlayFx(loseballFX);
		break;
	}

	case COLLIDER_EGG_1: //When the ball enters the first teleporter
	{
		if (egg1)
		{
			egg1 = false;
			ComboEggs();
			App->player->AddScore(50);
		}
		App->particles->AddParticle(App->particles->smoke, METERS_TO_PIXELS(bodyA->body->GetPosition().x), METERS_TO_PIXELS(bodyA->body->GetPosition().y));
		disapear = true;
		break;
	}

	case COLLIDER_EGG_2: //When the ball enters the second teleporter
	{
		if (egg2)
		{
			egg2 = false;
			ComboEggs();
			App->player->AddScore(50);
		}
		App->particles->AddParticle(App->particles->smoke, METERS_TO_PIXELS(bodyA->body->GetPosition().x), METERS_TO_PIXELS(bodyA->body->GetPosition().y));
		disapear = true;
		break;
	}

	case COLLIDER_LAUNCHER: //checks if the player is ready to launch
	{
		launcheable = true;
		break;
	}
	case COLLIDER_BALL_SHOOTER: //Controls if the ball is ready to launch or not
	{
		if(launcheable)
			App->audio->PlayFx(ballShooterFX);
		break;
	}
	case COLLIDER_EGG_NONE: //Teleport from the tunels that don't have an egg asigned
	{
		disapear = true;
		App->player->AddScore(25);
		break;
	}
	case COLLIDER_WALL:
	{
		App->audio->PlayFx(bouncingWallFX);
		break;
	}
	}

}

//Combo: when you enter 2 specific teleportes, you get an extra life
void ModuleSceneIntro::ComboEggs()
{
	if (!egg1 && !egg2)
	{
		App->player->AdjustLife(+1);
		App->audio->PlayFx(lifeWonFX);
		App->player->AddScore(100);
	}
}

void ModuleSceneIntro::mapBlit() 
{
	//Background-----------------------------

	SDL_Rect background = { 1, 1, 256, 416 };
	App->renderer->Blit(map, 0, 0, &background);

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);
	fVector normal(0.0f, 0.0f);


	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//Ball in the background ------------------------------------------------------

	if (playerBall != nullptr && (playerBall->body->GetFixtureList()->GetFilterData().categoryBits == BALL || playerBall->body->GetFixtureList()->GetFilterData().categoryBits == RAIL_BALL))
	{
		int x, y;
		SDL_Rect ball = { 273, 405, 14, 14 };
		playerBall->GetPosition(x, y);
		App->renderer->Blit(map, x - 1, y - 1, &ball, 1.0f, playerBall->GetRotation());
	}

	//Bouncers ------------------------------------------------------

	SDL_Rect bouncer = { 445, 389, 26, 26 };
	App->renderer->Blit(map, 145, 70, &bouncer);
	App->renderer->Blit(map, 182, 78, &bouncer);
	App->renderer->Blit(map, 145, 105, &bouncer);

	SDL_Rect initialBouncer = { 327, 387, 23, 41 };
	App->renderer->Blit(map, 55, 298, &initialBouncer);

	SDL_Rect initialBouncer2 = { 351, 387, 23, 41 };
	App->renderer->Blit(map, 153, 298, &initialBouncer2);

	//Map details ------------------------------------------------------
	App->renderer->Blit(map, 40, 30, &mapMonitor.GetCurrentFrame());

	//Animals ------------------------------------------------------
	App->renderer->Blit(graphics, 10, 365, &brownBambi.GetCurrentFrame());
	App->renderer->Blit(graphics, 50, 375, &blueBird.GetCurrentFrame());
	App->renderer->Blit(graphics, 30, 385, &blackSquirrel.GetCurrentFrame());

	App->renderer->Blit(graphics, 160, 385, &pinkBunny.GetCurrentFrame(), 1.0f, 0.0, 0, 0, SDL_FLIP_HORIZONTAL);
	App->renderer->Blit(graphics, 195, 370, &blueCupoSleeping.GetCurrentFrame(), 1.0f, 0.0, 0, 0, SDL_FLIP_HORIZONTAL);
	App->renderer->Blit(graphics, 180, 380, &blueCupoSit.GetCurrentFrame());


	//Map Rails ------------------------------------------------------
	SDL_Rect backgroundPlus = { 258, 1, 256, 350 };
	App->renderer->Blit(map, 0, 0, &backgroundPlus);

	SDL_Rect rail = { 166, 467, 105, 89 };
	App->renderer->Blit(map, 142, 62, &rail);


	//Eggs ------------------------------------------------------
	SDL_Rect egg = { 517, 152, 26, 31 };
	if(egg1)
		App->renderer->Blit(graphics, 80, 130, &egg);
	else
		App->renderer->Blit(graphics, 80, 130, &blueCupoSleeping.GetCurrentFrame());

	if(egg2)
	App->renderer->Blit(graphics, 70, 110, &egg);
	else
		App->renderer->Blit(graphics, 70, 110, &blueCupoSit.GetCurrentFrame());





	//Launcher ------------------------------------------------------
	SDL_Rect goPush = { 381, 365, 32, 64 };
	App->renderer->Blit(map, 230, 344, &goPush);

	if (launcheable)
	{
		//Red launcher if is ready to launch
		App->renderer->Blit(map, 232, 397, &launcher.GetCurrentFrame());
	}

	//Flippers ------------------------------------------------------
	SDL_Rect flipper = { 436, 368, 31, 12 };

	iPoint LFpos = { METERS_TO_PIXELS(BleftFlipper->body->GetPosition().x),METERS_TO_PIXELS(BleftFlipper->body->GetPosition().y) };
	iPoint LFOff = { BleftFlipper->width, BleftFlipper->height };
	App->renderer->Blit(map, LFpos.x - LFOff.x/2 - LFOff.y, LFpos.y - LFOff.y, &flipper,1.0f, RADTODEG * BleftFlipper->body->GetAngle());

	iPoint rightFlipPos = { METERS_TO_PIXELS(BrightFlipper->body->GetPosition().x),METERS_TO_PIXELS(BrightFlipper->body->GetPosition().y) };
	iPoint RFOff = { BrightFlipper->width, BrightFlipper->height };
	App->renderer->Blit(map, rightFlipPos.x - RFOff.x / 2, rightFlipPos.y - RFOff.y, &flipper, 1.0f, RADTODEG * BrightFlipper->body->GetAngle(), INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);



	//Ball in the foreground-------------------------------------------
	if (playerBall != nullptr && playerBall->body->GetFixtureList()->GetFilterData().categoryBits == RAIL_BALL_ENTRANCE)
	{
		int x, y;
		SDL_Rect ball = { 273, 405, 14, 14 };
		playerBall->GetPosition(x, y);
		App->renderer->Blit(map, x - 1, y - 1, &ball, 1.0f, playerBall->GetRotation());

	}

}
