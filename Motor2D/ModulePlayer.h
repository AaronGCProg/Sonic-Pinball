#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	//Game loop--------------------------
	bool ReStartGame();
	void RoundEnd();
	void BallRePos();

	//Score---------------------------
	void AdjustLife(int life);
	void AddScore(int scoreas);
	void UpdateScore();
	void ReSetScore();

	//Draw--------------------------------
	void DrawEndRoundScreen();
	void DrawUI();

	//Additional Textures----------------
	SDL_Texture* ui = nullptr;
	SDL_Texture* ui_sup = nullptr;



private:
	//Game Loop---------------------------------
	int lifes = 3;
	uint roundTimer = 0u;

	//Score----------------------------
	int actualScore = 0;
	int prevScore = 0;
	int HhighestScore = 0;
	int actualRound = 0;
	int starterHighScore = 0;
	int ScoreFont = -1;

	//UI Draw---------------------------
	char Score_text[15];

	//Internal Control---------------------------
	bool roundWin = false;
	bool roundLose = false;



};