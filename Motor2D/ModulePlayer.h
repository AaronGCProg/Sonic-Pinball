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


public:

	int lifes = 3;

	char actualScore_text[12];
	int actualScore = 0;
	int fontScore = -1;

};