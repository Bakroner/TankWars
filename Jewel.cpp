#include "jewel.h"

Jewel::Jewel(SDL_Renderer *renderer, string filePath, int color, float x, float y)
{
active = true;

if(color == 0){

string basePath = filePath +"purple.png";

jewel = IMG_LoadTexture(renderer, basePath.c_str());

}else if(color == 1){

	string basePath = filePath + "red.png";

	jewel = IMG_LoadTexture(renderer, basePath.c_str());

}else if(color == 2){

	string basePath = filePath + "blue.png";

	jewel = IMG_LoadTexture(renderer, basePath.c_str());

}else if(color == 3){

	string basePath = filePath + "folderPickup.png";

	jewel = IMG_LoadTexture(renderer, basePath.c_str());
}

jewelRect.x = x;
jewelRect.y = y;

int w,h;
SDL_QueryTexture(jewel, NULL, NULL, &w, &h);
jewelRect.w = w;
jewelRect.h = h;

posJ_X = jewelRect.x;
posJ_Y = jewelRect.y;

}

void Jewel::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, jewel, NULL, &jewelRect);
}

void Jewel::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed) * deltaTime;

	jewelRect.x = (int) (posJ_X + .5f);
}

void Jewel::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed) * deltaTime;

	jewelRect.y = (int) (posJ_Y + .5f);
}
