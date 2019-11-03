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
	bool ReStartGame();
	void MinusLife();
	void AddScore(int scoreas);
	void UpdateScore();
	void ReSetScore();

	SDL_Texture* ui = nullptr;
	SDL_Texture* ui_sup = nullptr;



private:

	int lifes = 3;

	char Score_text[15];
	int actualScore = 0;
	int prevScore = 0;
	int HhighestScore = 0;
	int actualRound = 0;
	int starterHighScore = 0;

	bool roundWin = false;
	bool roundLose = false;

	uint roundTimer = 0u;

	int ScoreFont = -1;

};