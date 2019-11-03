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

	ScoreFont = App->renderer->LoadFont("pinball/score_text.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ", 1);
	ui = App->textures->Load("pinball/misc.png");
	ui_sup = App->textures->Load("pinball/misc2.png");


	actualRound++;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->renderer->UnLoadFont(ScoreFont);
	App->textures->Unload(ui);


	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//Ui quad
	App->renderer->DrawQuad({ 0, 416, 256, 50 }, 0, 0, 0, 255);

	App->renderer->BlitText(0, 425, 0, "PREVIOUS");
	sprintf_s(Score_text, 10, "%7d", prevScore);
	App->renderer->BlitText(8, 435, 0, Score_text);

	App->renderer->BlitText((SCREEN_WIDTH / 2)-30, 425, 0, "SCORE");
	sprintf_s(Score_text, 10, "%7d", actualScore);
	App->renderer->BlitText((SCREEN_WIDTH / 2) - 50, 435, 0, Score_text);

	App->renderer->BlitText(SCREEN_WIDTH - 60, 425, 0, "HIGHEST");
	sprintf_s(Score_text, 10, "%7d", HhighestScore);
	App->renderer->BlitText(SCREEN_WIDTH -60, 435, 0, Score_text);

	SDL_Rect ball = { 273, 405, 14, 14 };
	App->renderer->Blit(App->scene_intro->map, SCREEN_WIDTH - 60, 442, &ball, 1.0f);
	sprintf_s(Score_text, 10, "%7d", lifes);
	App->renderer->BlitText(SCREEN_WIDTH - 90, 448, 0, Score_text);

	App->renderer->BlitText(37, 448, 0, "ROUND");
	sprintf_s(Score_text, 10, "%7d", actualRound);
	App->renderer->BlitText(47, 448, 0, Score_text);

	App->renderer->Blit(graphics, 10, 365, &brownBambi.GetCurrentFrame());



	if (roundWin || roundLose)
	{
		roundTimer++;

		SDL_Rect win_banner = { 40, 238, 240, 56 };
		App->renderer->Blit(ui, 5 , SCREEN_HEIGHT / 4.5f, &win_banner, 1.0f);


		SDL_Rect banner_strip = { 3, 52, 33, 62 };
		App->renderer->Blit(ui, -15, SCREEN_HEIGHT / 4.5f-5, &banner_strip, 1.0f);

		if (roundWin)
		{
			SDL_Rect sonic_thumbs_up = { 500,405, 63, 55 };
			App->renderer->Blit(ui_sup, SCREEN_WIDTH / 2 - 35, SCREEN_HEIGHT / 2.75f, &sonic_thumbs_up, 1.0f);

			SDL_Rect winner = { 566,414, 104, 35 };
			App->renderer->Blit(ui_sup, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 3.25f, &winner, 1.0f);

		}

		if (roundTimer == 70)
		{
			roundWin = false;
			roundLose = false;
			roundTimer = 0;
			App->audio->PlayFx(App->scene_intro->startingRoundFX, 0, 10);
			ReStartGame();
		}

	}


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

void ModulePlayer::UpdateScore()
{
	prevScore = actualScore;

	if (HhighestScore < actualScore)
	{
		HhighestScore = actualScore;
	}
}

void ModulePlayer::ReSetScore()
{
	actualScore = 0;
}

bool ModulePlayer::ReStartGame()
{
	if (lifes <= 0)
	{
		if (starterHighScore < HhighestScore)
		{
			roundWin = true;
			starterHighScore = HhighestScore;
			App->audio->PlayFx(App->scene_intro->winRoundFX, 0, 10);

		}
		else
		{
			roundLose = true;
			App->audio->PlayFx(App->scene_intro->loseRoundFX, 0, 10);

		}
		actualRound++;
		lifes = 3;

		return true;
	}

	App->scene_intro->playerBall->body->SetTransform({ PIXEL_TO_METERS(240),PIXEL_TO_METERS(382) }, 0.0f);
	App->scene_intro->playerBall->body->SetAngularVelocity(0.0f);
	App->scene_intro->playerBall->body->SetLinearVelocity({ 0,0 });

	b2Filter filter;
	filter.categoryBits = BALL;
	filter.maskBits = REGULAR_MAP;
	App->scene_intro->playerBall->body->GetFixtureList()->SetFilterData(filter);

	UpdateScore();
	ReSetScore();

	return true;

}



