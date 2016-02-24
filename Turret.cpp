#include "Turret.h"

Turret::Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;

	fire = Mix_LoadWAV((audioPath + "Fire.wav").c_str());

	string basePath = filePath + "Turret Base.png";

	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	string barrelPath = filePath + "Turret Barrel TEST2.png";

	tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	baseRect.x = x;
	baseRect.y = y;

	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	barrelRect.x = x;
	barrelRect.y = y;

	SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;

	center.x = 35;
	center.y = 35;

	string bulletPath = filePath + "Turret Bullet.png";

	for (int i = 0; i < 10; i++)
	{
		TurretBullet tmpBullet(renderer, bulletPath, 500, 500);

		bulletList.push_back(tmpBullet);
	}

	srand(time(NULL));

	//update floats for movemnet

	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;

}

void Turret::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	baseRect.x = (int) (posB_X + 0.5f);
	barrelRect.x = (int) (posT_X + 0.5f);
}

void Turret::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	baseRect.y = (int) (posB_Y + 0.5f);
	barrelRect.y = (int) (posT_Y + 0.5f);
}

void Turret::Draw(SDL_Renderer *renderer)
{

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active) {
			bulletList[i].Draw(renderer);
		}
	}

	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, turretangle, &center, SDL_FLIP_NONE);
}

void Turret::Update(float deltaTime, SDL_Rect tankRect)
{
	x = (tankRect.x + (tankRect.w / 2)) - (baseRect.x + (baseRect.w / 2));
	y = (tankRect.y + (tankRect.h / 2)) - (baseRect.y + (baseRect.h / 2));
	turretangle = atan2(y, x) * 180 / 3.14;

	if (SDL_GetTicks() > fireTime) {

		if (baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768){
		CreateBullet(tankRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 3 + 1) * 1000;

	}

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active) {
			bulletList[i].Update(deltaTime);
		}
	}

}

void Turret::CreateBullet(SDL_Rect target) {

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false) {

			bulletList[i].Start(target, baseRect);

			Mix_PlayChannel(-1, fire, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = ((baseRect.x + (baseRect.w / 2)) - (bulletList[i].posRect.w / 2));
			bulletList[i].posRect.y = ((baseRect.y + (baseRect.h / 2)) - (bulletList[i].posRect.h / 2));

			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			break;
		}
	}

}
