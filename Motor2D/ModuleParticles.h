
#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;

struct Particle
{
	Animation anim;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;
	SDL_RendererFlip fliped = SDL_FLIP_NONE;



	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{

public:
	ModuleParticles(Application* app, bool start_enabled = true);
	~ModuleParticles();

	bool Awake();
	update_status Update();
	bool CleanUp();


	void AddParticle(const Particle& particle, int x, int y, SDL_RendererFlip fliped = SDL_FLIP_NONE, Uint32 delay = 0);

private:

	SDL_Texture* graphics = nullptr;
	uint last_particle = 0;
	Uint32 defaultParticleLife = 200;

public:

	Particle* active[MAX_ACTIVE_PARTICLES];
	Particle smoke;

};

#endif // __MODULEPARTICLES_H__
