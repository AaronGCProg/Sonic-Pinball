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


	mapMonitor.PushBack({ 0, 509, 48, 55 });
	mapMonitor.PushBack({ 52, 509, 48, 55 });
	mapMonitor.PushBack({ 107, 509, 48, 55 });
	mapMonitor.PushBack({ 52, 509, 48, 55 });
	mapMonitor.speed = 0.15f;

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

	// sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);


	// Map Collisions (Se debe pasar a función)

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

	map_col.add(App->physics->CreateChain(0, 0, map_spritesheet, 94, true, COLLIDER_WALL));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(map);
	App->textures->Unload(graphics);

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	mapBlit();


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		playerBall.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 8));
		playerBall.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
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
		//if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
		App->renderer->Blit(map, x, y, &ball, 1.0f, c->data->GetRotation());
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

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;


	if(bodyB->colType != COLLIDER_WALL)
		App->audio->PlayFx(bonus_fx);


	

	


	/*if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

void ModuleSceneIntro::mapBlit() 
{

	SDL_Rect background = { 1, 1, 256, 416 };
	App->renderer->Blit(map, 0, 0, &background, 0);

	SDL_Rect bouncer = { 445, 389, 26, 26 };
	App->renderer->Blit(map, 145, 70, &bouncer, 0);
	App->renderer->Blit(map, 182, 78, &bouncer, 0);
	App->renderer->Blit(map, 145, 105, &bouncer, 0);

	App->renderer->Blit(map, 40, 30, &mapMonitor.GetCurrentFrame(), 0);



	SDL_Rect backgroundPlus = { 258, 1, 256, 350 };
	App->renderer->Blit(map, 0, 0, &backgroundPlus, 0);

	SDL_Rect rail = { 186, 467, 105, 89 };
	App->renderer->Blit(map, 186, 60, &rail, 0);

	SDL_Rect goPush = { 381, 365, 32, 64 };
	App->renderer->Blit(map, 230, 344, &goPush, 0);

	SDL_Rect initialBouncer = { 327, 387, 23, 41 };
	App->renderer->Blit(map, 55, 298, &initialBouncer, 0);

	SDL_Rect initialBouncer2 = { 351, 387, 23, 41 };
	App->renderer->Blit(map, 153, 298, &initialBouncer2, 0);


}
