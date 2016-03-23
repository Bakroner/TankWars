#include <string>
#include <iostream>

using namespace std;
#include <vector>

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif


class EnemyTank{
public:

	bool active;

	string eTankPath;

	SDL_Texture *eTank;

	SDL_Rect eTankRect;

	float x,y,tankangle;

	Mix_Chunk *explode;

	float speed;

	int health;

	SDL_Point center;

	EnemyTank(SDL_Renderer*renderer, string filePath, string audioPath, float x, float y);

	void Update(float deltaTime, SDL_Rect tankRect);

	void Draw(SDL_Renderer*renderer);

	void Reset();

	void RemoveHealth();

	void eTankMoveX(float tankSpeed, float deltaTime);

	void eTankMoveY(float tankSpeed, float deltaTime);

	float posT_X, posT_Y;






};
