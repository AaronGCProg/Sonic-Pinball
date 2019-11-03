#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Module.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"



ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ModuleSceneIntro* pointer = App->scene_intro;

	App->scene_intro->playerBall = App->physics->CreateCircle(240, 382, 6, 0.2f, false, COLLIDER_BALL, REGULAR_MAP, BALL);
	App->scene_intro->playerBall->listener = pointer;

	fontScore = App->renderer->LoadFont("pinball/score.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890", 1);

	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->renderer->UnLoadFont(fontScore);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//Ui quad
	App->renderer->DrawQuad({ 0, 416, 256, 50 }, 0, 0, 0, 255);



	sprintf_s(actualScore_text, 10, "%7d", actualScore);
	App->renderer->BlitText((SCREEN_WIDTH / 2) - 15, 40, 0, actualScore_text);

	return UPDATE_CONTINUE;
}

void ModulePlayer::MinusLife()
{
	if (lifes >= 0)
	{
		lifes -= 1;
	}

}

void ModulePlayer::AddScore(int score)
{
	actualScore += score;

}

void ModulePlayer::ReStartGame()
{
	if (lifes > 0)
	{
		App->scene_intro->playerBall->body->SetTransform({ PIXEL_TO_METERS(240),PIXEL_TO_METERS(382) }, 0.0f);
		App->scene_intro->playerBall->body->SetAngularVelocity(0.0f);
		App->scene_intro->playerBall->body->SetLinearVelocity({ 0,0 });

		b2Filter filter;
		filter.categoryBits = BALL;
		filter.maskBits = REGULAR_MAP;
		App->scene_intro->playerBall->body->GetFixtureList()->SetFilterData(filter);

	}

}



