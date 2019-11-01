#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL\include\SDL.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);

	// Load/Unload Font
	int LoadFont(const char* texture_path, const char* characters, uint rows = 1);
	void UnLoadFont(int font_id);

	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);

public:
	SDL_Renderer* renderer;
	SDL_Rect camera;

private:
	Font fonts[MAX_FONTS];
};