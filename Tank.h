#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

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

#include "TankBullet.h"

class Tank{
public:

	SDL_Texture *back, *mid, *front;

	SDL_Rect backR, midR, frontR;

	float playerHealth, maxHealth;

	void eTankHit();

	void eBulletHit();

	bool active;

	vector<TankBullet> bulletList;

	string playerPath;

	int playerNum;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir, xDirOld;
	float yDir, yDirOld;
	float speed;
	float pos_X, pos_Y;
	float x, y, tankangle;

	SDL_Point center;

	float oldAngle;

	Mix_Chunk *fire;

	Sint16 Xvalue, Yvalue;

	Tank(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y);

	void OnControllerAxis(Sint16 X, Sint16 Y);
	void OnControllerButton(const SDL_ControllerButtonEvent event);
	void Update(float deltaTime);
	void Draw(SDL_Renderer *renderer);
	void Reset();

private:

	void CreateBullet();

};
