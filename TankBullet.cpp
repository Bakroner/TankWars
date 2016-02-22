#include "TankBullet.h"

TankBullet::TankBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY)
{
	active = false;

	speed = 800.0;

	texture = IMG_LoadTexture(renderer, filePath.c_str());

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	posRect.x = x - (posRect.w / 2);
	posRect.y = y;

	pos_X = x;
	pos_Y = y;

	//xDir = dirX;
	//yDir = dirY;
}

void TankBullet::Reset()
{
	posRect.x = -1000;
	pos_X = posRect.x;
	active = false;
}

void TankBullet::Draw(SDL_Renderer * renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void TankBullet::Update(float deltaTime)
{
	//pos_X += (speed * xDir) * deltaTime;
	//pos_Y += (speed * yDir) * deltaTime;

	float radians = (tankangle * 3.14)/ 180;

	float move_x = speed * cos(radians);
	float move_y = speed * sin(radians);

	pos_X += (move_x) * deltaTime;
	pos_Y += (move_y) * deltaTime;

	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if ((posRect.y < (0 - posRect.h)) || (posRect.y > 768) || (posRect.x < (0 - posRect.w)) || (posRect.x > 1024)) {
		Reset();
	}
}
