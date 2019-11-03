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
	void ReStartGame();
	void MinusLife();
	void AddScore(int scoreas);
	void UpdateScore();
	void ReSetScore();




public:

	int lifes = 3;

	char Score_text[15];
	int actualScore = 0;
	int prevScore = 0;
	int HhighestScore = 0;
	int actualRound = 0;

	int ScoreFont = -1;

};