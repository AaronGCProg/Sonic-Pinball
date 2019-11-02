#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;

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
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Example Default Resources
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	// Game Resources
	map = App->textures->Load("pinball/map_spritesheet.png");
	graphics = App->textures->Load("pinball/general_spritesheet.png");
	fontScore = App->renderer->LoadFont("pinball/score.png", "0123456789%,", 1);


	// Map Collisions 
	// Pivot 0, 0
	int map_spritesheet[94] = {
		23, 49,
		52, 22,
		86, 14,
		125, 14,
		147, 17,
		178, 22,
		204, 30,
		227, 42,
		225, 33,
		216, 19,
		219, 10,
		225, 8,
		235, 10,
		245, 26,
		248, 41,
		249, 47,
		229, 153,
		212, 187,
		219, 193,
		202, 207,
		200, 212,
		197, 216,
		200, 274,
		210, 276,
		225, 272,
		225, 331,
		153, 365,
		153, 373,
		144, 377,
		144, 400,
		144, 443,
		90, 442,
		90, 377,
		81, 372,
		81, 366,
		44, 347,
		35, 343,
		35, 298,
		37, 271,
		13, 239,
		13, 229,
		24, 225,
		35, 236,
		34, 194,
		25, 176,
		13, 130,
		10, 85
	};
	map_col.add(App->physics->CreateChain(0, 0, map_spritesheet, 94, true, COLLIDER_WALL, REGULAR_MAP, BALL));

	int map_starter_ballShooter[14] = {
		232, 389,
		230, 358,
		228, 274,
		232, 62,
		248, 62,
		250, 350,
		248, 390
	};
	map_col.add(App->physics->CreateChain(0, 0, map_starter_ballShooter, 14, true, COLLIDER_WALL, REGULAR_MAP, BALL));
	App->physics->CreateRectangleSensor(242, 140, 5, 5, COLLIDER_BALLTORAIL, REGULAR_MAP, BALL);

	int map_right_rail[94] = {
	230, 150,
	230, 63,
	229, 48,
	224, 41,
	218, 36,
	209, 30,
	202, 27,
	194, 27,
	187, 30,
	179, 32,
	174, 36,
	170, 45,
	165, 57,
	165, 63,
	164, 69,
	164, 75,
	166, 84,
	170, 89,
	174, 92,
	243, 162,
	240, 269,
	191, 316,
	185, 313,
	182, 301,
	186, 297,
	193, 294,
	222, 260,
	224, 163,
	152, 93,
	149, 84,
	146, 75,
	145, 65,
	145, 57,
	151, 39,
	156, 29,
	163, 21,
	171, 16,
	178, 12,
	184, 12,
	215, 12,
	230, 21,
	241, 35,
	244, 41,
	247, 50,
	247, 61,
	247, 73,
	248, 150
	};
	map_col.add(App->physics->CreateChain(0, 0, map_right_rail, 94, true, COLLIDER_WALL, RAIL, RAIL_BALL));
	App->physics->CreateRectangleSensor(190, 309, 5, 5, COLLIDER_RAILTOBALL, RAIL, RAIL_BALL);


	int map_right_rail_rightEnter[16] = {
	175, 151,
	204, 121,
	216, 97,
	224, 84,
	229, 65,
	250, 73,
	224, 117,
	196, 163
	};
	//map_col.add(App->physics->CreateChain(0, 0, map_right_rail_rightEnter, 94, true, COLLIDER_WALL, STARTER_RAIL, RAIL_BALL));

	int map_right_rail_leftEnter[78] = {
	105, 156,
	84, 126,
	80, 113,
	78, 105,
	77, 95,
	79, 83,
	85, 76,
	93, 68,
	102, 62,
	116, 57,
	137, 59,
	156, 71,
	165, 82,
	177, 91,
	183, 95,
	195, 98,
	202, 98,
	217, 92,
	224, 86,
	230, 72,
	248, 72,
	244, 82,
	241, 91,
	230, 104,
	224, 112,
	205, 118,
	184, 115,
	168, 107,
	155, 98,
	146, 89,
	141, 81,
	135, 78,
	125, 77,
	115, 77,
	109, 79,
	98, 87,
	98, 95,
	99, 105,
	105, 116
	};


	int aloneBumper[16] = {
		126, 75,
		121, 53,
		113, 40,
		107, 36,
		115, 34,
		135, 36,
		129, 41,
		129, 95
	};
	int genericColMap1[36] = {
		40, 60,
		58, 124,
		58, 108,
		64, 101,
		71, 101,
		77, 105,
		78, 82,
		63, 74,
		56, 68,
		55, 57,
		66, 54,
		79, 57,
		88, 64,
		92, 69,
		100, 62,
		90, 48,
		76, 39,
		56, 43
	};
	int genericColMap2[18] = {
		91, 166,
		99, 160,
		109, 163,
		111, 175,
		116, 176,
		115, 163,
		107, 157,
		98, 156,
		88, 161
	};

	int initialBumpers1[6] = {
		58, 300,
		58, 328,
		76, 339
	};
	int initialBumpers2[6] = {
		175, 300,
		175, 328,
		156, 337
	};

	map_col.add(App->physics->CreateChain(0, 0, genericColMap1, 36, true, COLLIDER_WALL, REGULAR_MAP, BALL));
	map_col.add(App->physics->CreateChain(0, 0, genericColMap2, 18, true, COLLIDER_WALL, REGULAR_MAP, BALL));

	bumpers.add(App->physics->CreateChain(0, 0, initialBumpers1, 6, true, COLLIDER_BOUNCER, REGULAR_MAP, BALL));
	bumpers.getLast()->data->listener = this;
	bumpers.add(App->physics->CreateChain(0, 0, initialBumpers2, 6, true, COLLIDER_BOUNCER, REGULAR_MAP, BALL));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateChain(0, 0, aloneBumper, 16, true, COLLIDER_BOUNCER, REGULAR_MAP, BALL));
	bumpers.getLast()->data->listener = this;
	bumpers.add(App->physics->CreateCircle(160, 82, 12, 0.00f, true, COLLIDER_BOUNCER, REGULAR_MAP, BALL));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateCircle(195, 90, 12, 0.00f, true, COLLIDER_BOUNCER, REGULAR_MAP, BALL));
	bumpers.getLast()->data->listener = this;

	bumpers.add(App->physics->CreateCircle(158, 118, 12, 0.00f, true, COLLIDER_BOUNCER, REGULAR_MAP, BALL));
	bumpers.getLast()->data->listener = this;

	BleftFlipper = App->physics->CreateFlipper(86, 371, FL_LEFT, {24,6});
	BleftFlipper->listener = this;
	JleftFlipper = App->physics->flipperJoints.getLast()->data;

	BrightFlipper = App->physics->CreateFlipper(149, 371, FL_RIGHT, { 24,5 });
	BrightFlipper->listener = this;
	JrightFlipper = App->physics->flipperJoints.getLast()->data;

	ballShooter = App->physics->CreateBallShooter(232, 406, 20, 16, COLLIDER_WALL, REGULAR_MAP, BALL);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(map);
	App->textures->Unload(graphics);
	App->renderer->UnLoadFont(fontScore);

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		mapDebug = !mapDebug;

	if(!mapDebug)
	mapBlit();


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	//Left Trigger
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		JleftFlipper->SetMotorSpeed(30);

	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		JleftFlipper->SetMotorSpeed(-30);
	}


	//Right Trigger
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		JrightFlipper->SetMotorSpeed(-30);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		JrightFlipper->SetMotorSpeed(30);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		ballShooter->SetMotorSpeed(1.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		ballShooter->SetMotorSpeed(-40.0f);
	}

	
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		playerBall.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 6, 0.2f, false, COLLIDER_BALL, BALL, REGULAR_MAP));
		playerBall.getLast()->data->listener = this;
	}


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = playerBall.getFirst();

	while(c != NULL)
	{
		int x, y;
		SDL_Rect ball = { 273, 405, 14, 14 };
		c->data->GetPosition(x, y);
		App->renderer->Blit(map, x-1, y-1, &ball, 1.0f, c->data->GetRotation());
		
		c = c->next;	
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}


	sprintf_s(actualScore_text, 10, "%7d", actualScore);
	App->renderer->BlitText((SCREEN_WIDTH / 2) - 15, 40, 0, actualScore_text);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->colType == COLLIDER_BOUNCER) 
		bodyA->body->GetContactList()->contact->SetRestitution(2.2f);


	if (bodyB->colType == COLLIDER_BALLTORAIL)
	{
		bodyA->body->GetFixtureList()->SetFilterData({ RAIL, RAIL_BALL});
	}

	if (bodyB-> colType == COLLIDER_RAILTOBALL)
	{
		bodyA->body->GetFixtureList()->SetFilterData({ REGULAR_MAP, BALL });
		bodyA->body->SetLinearVelocity(b2Vec2(0, 0));
		bodyA->body->SetAngularVelocity(0);
	}
	
		
	actualScore += 10;
	App->audio->PlayFx(bonus_fx);
	
}

void ModuleSceneIntro::mapBlit() 
{

	SDL_Rect background = { 1, 1, 256, 416 };
	App->renderer->Blit(map, 0, 0, &background);

	SDL_Rect bouncer = { 445, 389, 26, 26 };
	App->renderer->Blit(map, 145, 70, &bouncer);
	App->renderer->Blit(map, 182, 78, &bouncer);
	App->renderer->Blit(map, 145, 105, &bouncer);

	App->renderer->Blit(map, 40, 30, &mapMonitor.GetCurrentFrame());

	App->renderer->Blit(graphics, 10, 365, &brownBambi.GetCurrentFrame());
	App->renderer->Blit(graphics, 50, 375, &blueBird.GetCurrentFrame());
	App->renderer->Blit(graphics, 30, 385, &blackSquirrel.GetCurrentFrame());

	App->renderer->Blit(graphics, 160, 385, &pinkBunny.GetCurrentFrame(), 1.0f, 0.0, 0, 0, SDL_FLIP_HORIZONTAL);
	App->renderer->Blit(graphics, 195, 370, &blueCupoSleeping.GetCurrentFrame(), 1.0f, 0.0, 0, 0, SDL_FLIP_HORIZONTAL);
	App->renderer->Blit(graphics, 180, 380, &blueCupoSit.GetCurrentFrame());

	SDL_Rect backgroundPlus = { 258, 1, 256, 350 };
	App->renderer->Blit(map, 0, 0, &backgroundPlus);

	SDL_Rect rail = { 166, 467, 105, 89 };
	App->renderer->Blit(map, 142, 62, &rail);

	SDL_Rect goPush = { 381, 365, 32, 64 };
	App->renderer->Blit(map, 230, 344, &goPush);

	SDL_Rect flipper = { 436, 368, 31, 12 };

	iPoint LFpos = { METERS_TO_PIXELS(BleftFlipper->body->GetPosition().x),METERS_TO_PIXELS(BleftFlipper->body->GetPosition().y) };
	iPoint LFOff = { BleftFlipper->width, BleftFlipper->height };
	App->renderer->Blit(map, LFpos.x - LFOff.x/2 - LFOff.y, LFpos.y - LFOff.y, &flipper,1.0f, RADTODEG * BleftFlipper->body->GetAngle());

	iPoint rightFlipPos = { METERS_TO_PIXELS(BrightFlipper->body->GetPosition().x),METERS_TO_PIXELS(BrightFlipper->body->GetPosition().y) };
	iPoint RFOff = { BrightFlipper->width, BrightFlipper->height };
	App->renderer->Blit(map, rightFlipPos.x - RFOff.x / 2, rightFlipPos.y - RFOff.y, &flipper, 1.0f, RADTODEG * BrightFlipper->body->GetAngle(), INT_MAX, INT_MAX, SDL_FLIP_HORIZONTAL);


	SDL_Rect initialBouncer = { 327, 387, 23, 41 };
	App->renderer->Blit(map, 55, 298, &initialBouncer);

	SDL_Rect initialBouncer2 = { 351, 387, 23, 41 };
	App->renderer->Blit(map, 153, 298, &initialBouncer2);


}
