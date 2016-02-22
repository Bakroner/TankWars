#include <iostream>
#include <string>
using namespace std;

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

class TankBullet {

public:

	bool active;
	SDL_Texture *texture;
	SDL_Rect posRect;
	float xDir, yDir;
	float speed;
	float pos_X, pos_Y;
	float tankangle;
	TankBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY);
	void Update(float deltaTime);
	void Draw(SDL_Renderer *renderer);
	void Reset();

};
