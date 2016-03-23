
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>
#include "Tank.h"
#include "Turret.h"
#include "EnemyTank.h"
#include "jewel.h"

using namespace std;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/src/";
string audio_dir = currentWorkingDirectory + "/src/";
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <direct.h>
#define getcwd _getcwd

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "\\src\\";
string audio_dir = currentWorkingDirectory + "\\src\\";
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include <unistd.h>

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/src/";
string audio_dir = currentWorkingDirectory + "/src/";
#endif

TTF_Font *font;

SDL_Color colorP1 = {255, 255, 255, 255};

SDL_Surface *playerSurface, *turretSurface;

SDL_Texture *playerTexture, *turretTexture;

SDL_Rect playerPos, turretPos;

int playerHealth = 100;

string tempText = "";

void PlayerText(SDL_Renderer *renderer){

	string Result;

	ostringstream convert;

	convert << playerHealth;

	Result = convert.str();

	//tempText = "Player Health: " + Result;
	tempText = "Player Health" ;

	playerSurface = TTF_RenderText_Solid(font, tempText.c_str(), colorP1);

	playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

	SDL_QueryTexture(playerTexture, NULL, NULL, &playerPos.w, &playerPos.h);

	SDL_FreeSurface(playerSurface);
}

void TurretText(SDL_Renderer *renderer, int turretNum)
{
	string Result;

	ostringstream convert;

	convert << turretNum;

	Result = convert.str();

	tempText = "Turret " + Result + " was the last hit";

	if(turretNum == 0){

		tempText = "No Turret has been hit";
	}
	turretSurface = TTF_RenderText_Solid(font, tempText.c_str(), colorP1);

	turretTexture = SDL_CreateTextureFromSurface(renderer, turretSurface);

	SDL_QueryTexture(turretTexture, NULL, NULL, &turretPos.w, &turretPos.h);

	SDL_FreeSurface(turretSurface);

}

int main(int argc, char *argv[]){

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	window = SDL_CreateWindow("Tank Wars",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool quit = false;
	SDL_Event e;

	SDL_GameControllerEventState(SDL_ENABLE);
	SDL_GameController*gGameController0 = NULL;
	gGameController0 = SDL_GameControllerOpen(0);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "BackgroundTEST.wav").c_str());

	if(!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	//***************************** CREATE PLAYERS ***************************************
	Tank tank1 = Tank(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 50.0f, 100.0f);

	//top left
	Turret turret1 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 800.0f, 500.0f);
	//top right
	Turret turret2 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1600.0f, 250.0f);
	//bottom right
	Turret turret3 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 400.0f, 1000.0f);
	//bottom left
	Turret turret4 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 1600.0f, 1250.0f);

	EnemyTank eTank1 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 400.0f, 200.0f);
	EnemyTank eTank2 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 1600.0f, 200.0f);
	EnemyTank eTank3 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 300.0f, 900.0f);
	EnemyTank eTank4 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 1500.0f, 1150.0f);

	SDL_Texture *ground = IMG_LoadTexture(renderer, (images_dir + "Ground.png").c_str());

	SDL_Rect groundRect;

	groundRect.x = 0;
	groundRect.y = 0;
	groundRect.w = 2048;
	groundRect.h = 1536;

	float X_pos = 0.0f;
	float Y_pos = 0.0f;

	// font engine
	TTF_Init();

	font = TTF_OpenFont((images_dir + "planetnital.ttf").c_str(), 20);

	playerPos.x = 10;
	playerPos.y = 10;

	turretPos.x = 600;
	turretPos.y = 10;

	PlayerText(renderer);

	TurretText(renderer, 0);

	//Jewel HUD ********************* NEW ***********************
	SDL_Texture *Jewelsbkgd = IMG_LoadTexture(renderer, (images_dir + "jewelsBKGD.png").c_str());
	SDL_Rect JewelsbkgdRect;
	JewelsbkgdRect.x = 370;
	JewelsbkgdRect.y = 10;
	JewelsbkgdRect.w = 284;
	JewelsbkgdRect.h = 91;

	SDL_Texture *purple = IMG_LoadTexture(renderer, (images_dir + "jewelsPurple.png").c_str());
	SDL_Rect purplePos;
	purplePos.x = 370;
	purplePos.y = 10;
	purplePos.w = 284;
	purplePos.h = 91;

	SDL_Texture *red = IMG_LoadTexture(renderer, (images_dir + "jewelsRed.png").c_str());
	SDL_Rect redPos;
	redPos.x = 370;
	redPos.y = 10;
	redPos.w = 284;
	redPos.h = 91;

	SDL_Texture *blue = IMG_LoadTexture(renderer, (images_dir + "jewelsBlue.png").c_str());
	SDL_Rect bluePos;
	bluePos.x = 370;
	bluePos.y = 10;
	bluePos.w = 284;
	bluePos.h = 91;

	bool havePurple = false;
	bool haveRed = false;
	bool haveBlue = false;

	Jewel purpleJewel = Jewel(renderer, images_dir.c_str(), 0, 200.0f, 200.0f);
	Jewel redJewel = Jewel(renderer, images_dir.c_str(), 1, 400.0f, 200.0f);
	Jewel blueJewel = Jewel(renderer, images_dir.c_str(), 2, 600.0f, 200.0f);

	// Folder stuff
	Jewel folder1 = Jewel(renderer, images_dir.c_str(), 3, 600.0f, 350.0f);
	Jewel folder2 = Jewel(renderer, images_dir.c_str(), 3, 1200.0f, 600.0f);
	Jewel folder3 = Jewel(renderer, images_dir.c_str(), 3, 550.0f, 1000.0f);
	Jewel folder4 = Jewel(renderer, images_dir.c_str(), 3, 1400.0f, 1200.0f);

	SDL_Texture *folderB = IMG_LoadTexture(renderer, (images_dir + "folderBKGD.png").c_str());
	SDL_Texture *folderM = IMG_LoadTexture(renderer, (images_dir + "movingBar.png").c_str());
	SDL_Texture *folderF = IMG_LoadTexture(renderer, (images_dir + "folderFront.png").c_str());

	SDL_Rect folderRect;
	folderRect.x = 310;
	folderRect.y = 110;
	folderRect.w = 368;
	folderRect.h = 65;

	SDL_Rect movingRect;
	movingRect.x = 401;
	movingRect.y = 125;
	movingRect.w = 272;
	movingRect.h = 27;

	//Folder Info
	float currentFolders = 0.0f;
	float maxFolders = 100.0f;

	while(!quit)
	{

		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				quit = true;
			}

			switch (e.type){

			case SDL_CONTROLLERBUTTONDOWN:
				if (e.cdevice.which == 0){

					if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A){
						tank1.OnControllerButton(e.cbutton);
						break;
					}
				}
				break;

			case SDL_CONTROLLERAXISMOTION:
				//tank1.OnControllerAxis(e.caxis);
				break;
			}
		}

		const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
		const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

		tank1.OnControllerAxis(Xvalue, Yvalue);

		// update player 1 tank
		tank1.Update(deltaTime);

		if((tank1.posRect.x >= 1024 - tank1.posRect.w) && (tank1.Xvalue > 8000)){
			X_pos -= (tank1.speed) * deltaTime;

			if((groundRect.x >- 1024)){
				groundRect.x = (int) (X_pos + 0.5f);
				//move turret
				turret1.TankMoveX(-tank1.speed, deltaTime);
				turret2.TankMoveX(-tank1.speed, deltaTime);
				turret3.TankMoveX(-tank1.speed, deltaTime);
				turret4.TankMoveX(-tank1.speed, deltaTime);

				eTank1.eTankMoveX(-tank1.speed, deltaTime);
				eTank2.eTankMoveX(-tank1.speed, deltaTime);
				eTank3.eTankMoveX(-tank1.speed, deltaTime);
				eTank4.eTankMoveX(-tank1.speed, deltaTime);

				purpleJewel.TankMoveX(-tank1.speed, deltaTime);
				redJewel.TankMoveX(-tank1.speed, deltaTime);
				blueJewel.TankMoveX(-tank1.speed, deltaTime);

				folder1.TankMoveX(-tank1.speed, deltaTime);
				folder2.TankMoveX(-tank1.speed, deltaTime);
				folder3.TankMoveX(-tank1.speed, deltaTime);
				folder4.TankMoveX(-tank1.speed, deltaTime);

			}else{
				X_pos = groundRect.x;
			}
		}

		if((tank1.posRect.x <= 0) && (tank1.Xvalue < 8000)){
			X_pos += (tank1.speed) * deltaTime;

			if((groundRect.x < 0)){
				groundRect.x = (int) (X_pos + 0.5f);
				turret1.TankMoveX(tank1.speed, deltaTime);
				turret2.TankMoveX(tank1.speed, deltaTime);
				turret3.TankMoveX(tank1.speed, deltaTime);
				turret4.TankMoveX(tank1.speed, deltaTime);

				eTank1.eTankMoveX(tank1.speed, deltaTime);
				eTank2.eTankMoveX(tank1.speed, deltaTime);
				eTank3.eTankMoveX(tank1.speed, deltaTime);
				eTank4.eTankMoveX(tank1.speed, deltaTime);

				purpleJewel.TankMoveX(tank1.speed, deltaTime);
				redJewel.TankMoveX(tank1.speed, deltaTime);
				blueJewel.TankMoveX(tank1.speed, deltaTime);

				folder1.TankMoveX(tank1.speed, deltaTime);
				folder2.TankMoveX(tank1.speed, deltaTime);
				folder3.TankMoveX(tank1.speed, deltaTime);
				folder4.TankMoveX(tank1.speed, deltaTime);


			}else{
				X_pos = groundRect.x;
			}
		}

		if((tank1.posRect.y >= 768 - tank1.posRect.h) && (tank1.Yvalue > 8000)){
			Y_pos -= (tank1.speed) * deltaTime;

			if((groundRect.y > -768)){
				groundRect.y = (int) (Y_pos + 0.5f);
				turret1.TankMoveY(-tank1.speed, deltaTime);
				turret2.TankMoveY(-tank1.speed, deltaTime);
				turret3.TankMoveY(-tank1.speed, deltaTime);
				turret4.TankMoveY(-tank1.speed, deltaTime);

				eTank1.eTankMoveY(-tank1.speed, deltaTime);
				eTank2.eTankMoveY(-tank1.speed, deltaTime);
				eTank3.eTankMoveY(-tank1.speed, deltaTime);
				eTank4.eTankMoveY(-tank1.speed, deltaTime);

				purpleJewel.TankMoveY(-tank1.speed, deltaTime);
				redJewel.TankMoveY(-tank1.speed, deltaTime);
				blueJewel.TankMoveY(-tank1.speed, deltaTime);

				folder1.TankMoveY(-tank1.speed, deltaTime);
				folder2.TankMoveY(-tank1.speed, deltaTime);
				folder3.TankMoveY(-tank1.speed, deltaTime);
				folder4.TankMoveY(-tank1.speed, deltaTime);

			}else{
				Y_pos = groundRect.y;
			}
		}

		if((tank1.posRect.y <= 0) && (tank1.Yvalue < 8000)){
			Y_pos += (tank1.speed) * deltaTime;

			if((groundRect.y < 0)){
				groundRect.y = (int) (Y_pos + 0.5f);
				turret1.TankMoveY(tank1.speed, deltaTime);
				turret2.TankMoveY(tank1.speed, deltaTime);
				turret3.TankMoveY(tank1.speed, deltaTime);
				turret4.TankMoveY(tank1.speed, deltaTime);

				eTank1.eTankMoveY(tank1.speed, deltaTime);
				eTank2.eTankMoveY(tank1.speed, deltaTime);
				eTank3.eTankMoveY(tank1.speed, deltaTime);
				eTank4.eTankMoveY(tank1.speed, deltaTime);

				purpleJewel.TankMoveY(tank1.speed, deltaTime);
				redJewel.TankMoveY(tank1.speed, deltaTime);
				blueJewel.TankMoveY(tank1.speed, deltaTime);

				folder1.TankMoveY(tank1.speed, deltaTime);
				folder2.TankMoveY(tank1.speed, deltaTime);
				folder3.TankMoveY(tank1.speed, deltaTime);
				folder4.TankMoveY(tank1.speed, deltaTime);

			}else{
				Y_pos = groundRect.y;
			}
		}

		turret1.Update(deltaTime, tank1.posRect);
		turret2.Update(deltaTime, tank1.posRect);
		turret3.Update(deltaTime, tank1.posRect);
		turret4.Update(deltaTime, tank1.posRect);

		eTank1.Update(deltaTime, tank1.posRect);
		eTank2.Update(deltaTime, tank1.posRect);
		eTank3.Update(deltaTime, tank1.posRect);
		eTank4.Update(deltaTime, tank1.posRect);

		//turret to player collision
		for (int i = 0; i < turret1.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret1.bulletList[i].posRect)){
				turret1.bulletList[i].Reset();
				//playerHealth--;
				//PlayerText(renderer);
				tank1.eBulletHit();
				break;
			}
		}

		for (int i = 0; i < turret2.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret2.bulletList[i].posRect)){
				turret2.bulletList[i].Reset();
				//playerHealth--;
				//PlayerText(renderer);
				tank1.eBulletHit();
				break;
			}
		}

		for (int i = 0; i < turret3.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret3.bulletList[i].posRect)){
				turret3.bulletList[i].Reset();
				//playerHealth--;
				//PlayerText(renderer);
				tank1.eBulletHit();
				break;
			}
		}

		for (int i = 0; i < turret4.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&tank1.posRect, &turret4.bulletList[i].posRect)){
				turret4.bulletList[i].Reset();
				//playerHealth--;
				//PlayerText(renderer);
				tank1.eBulletHit();
				break;
			}
		}

		//player to turret collision
		for (int i = 0; i < tank1.bulletList.size(); i++)
		{
			if(SDL_HasIntersection(&turret1.baseRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				TurretText(renderer, 1);
				break;
			}

			if(SDL_HasIntersection(&turret2.baseRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				TurretText(renderer, 2);
				break;
			}

			if(SDL_HasIntersection(&turret3.baseRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				TurretText(renderer, 3);
				break;
			}

			if(SDL_HasIntersection(&turret4.baseRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				TurretText(renderer, 4);
				break;
			}

			//eTank 1
			if(SDL_HasIntersection(&eTank1.eTankRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				if(eTank1.active == true){
					eTank1.RemoveHealth();
				}
				break;
			}

			//eTank 2
			if(SDL_HasIntersection(&eTank2.eTankRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				if(eTank2.active == true){
					eTank2.RemoveHealth();
				}
				break;
			}

			//eTank 3
			if(SDL_HasIntersection(&eTank3.eTankRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				if(eTank3.active == true){
					eTank3.RemoveHealth();
				}
				break;
			}

			//eTank 4
			if(SDL_HasIntersection(&eTank4.eTankRect, &tank1.bulletList[i].posRect)){
				tank1.bulletList[i].Reset();
				if(eTank4.active == true){
					eTank4.RemoveHealth();
				}
				break;
			}

		}

		if(SDL_HasIntersection(&tank1.posRect, &eTank1.eTankRect)){
			tank1.eTankHit();
		}

		if(SDL_HasIntersection(&tank1.posRect, &eTank2.eTankRect)){
			tank1.eTankHit();
		}

		if(SDL_HasIntersection(&tank1.posRect, &eTank3.eTankRect)){
			tank1.eTankHit();
		}

		if(SDL_HasIntersection(&tank1.posRect, &eTank4.eTankRect)){
			tank1.eTankHit();
		}



		if(SDL_HasIntersection(&tank1.posRect, &purpleJewel.jewelRect)){
			havePurple = true;
			purpleJewel.active = false;
			purpleJewel.jewelRect.x = -5000;
			purpleJewel.jewelRect.y = -5000;
		}

		if(SDL_HasIntersection(&tank1.posRect, &redJewel.jewelRect)){
			haveRed = true;
			redJewel.active = false;
			redJewel.jewelRect.x = -5000;
			redJewel.jewelRect.y = -5000;
		}

		if(SDL_HasIntersection(&tank1.posRect, &blueJewel.jewelRect)){
			haveBlue = true;
			blueJewel.active = false;
			blueJewel.jewelRect.x = -5000;
			blueJewel.jewelRect.y = -5000;
		}

		if(SDL_HasIntersection(&tank1.posRect, &folder1.jewelRect) && folder1.active == true){
			folder1.active = false;
			folder1.jewelRect.x = -5000;
			folder1.jewelRect.y = -5000;
			currentFolders += 100/4;
		}

		if(SDL_HasIntersection(&tank1.posRect, &folder2.jewelRect) && folder2.active == true){
			folder2.active = false;
			folder2.jewelRect.x = -5000;
			folder2.jewelRect.y = -5000;
			currentFolders += 100/4;
		}

		if(SDL_HasIntersection(&tank1.posRect, &folder3.jewelRect) && folder3.active == true){
			folder3.active = false;
			folder3.jewelRect.x = -5000;
			folder3.jewelRect.y = -5000;
			currentFolders += 100/4;
		}

		if(SDL_HasIntersection(&tank1.posRect, &folder4.jewelRect) && folder4.active == true){
			folder4.active = false;
			folder4.jewelRect.x = -5000;
			folder4.jewelRect.y = -5000;
			currentFolders += 100/4;
		}

		movingRect.w = currentFolders/maxFolders * 272;

		// DRAW ***********************
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, ground,NULL,&groundRect);

		SDL_RenderCopy(renderer, Jewelsbkgd,NULL,&JewelsbkgdRect);

		if(havePurple)
			SDL_RenderCopy(renderer, purple,NULL,&purplePos);

		if(haveRed)
			SDL_RenderCopy(renderer, red,NULL,&redPos);

		if(haveBlue)
			SDL_RenderCopy(renderer, blue,NULL,&bluePos);

		SDL_RenderCopy(renderer, folderB, NULL, &folderRect);

		SDL_RenderCopy(renderer, folderM, NULL, &movingRect);

		SDL_RenderCopy(renderer, folderF, NULL, &folderRect);

		if(purpleJewel.active)
			purpleJewel.Draw(renderer);

		if(redJewel.active)
			redJewel.Draw(renderer);

		if(blueJewel.active)
			blueJewel.Draw(renderer);

		if(folder1.active)
			folder1.Draw(renderer);

		if(folder2.active)
			folder2.Draw(renderer);

		if(folder3.active)
			folder3.Draw(renderer);

		if(folder4.active)
			folder4.Draw(renderer);

		tank1.Draw(renderer);

		turret1.Draw(renderer);
		turret2.Draw(renderer);
		turret3.Draw(renderer);
		turret4.Draw(renderer);

		eTank1.Draw(renderer);
		eTank2.Draw(renderer);
		eTank3.Draw(renderer);
		eTank4.Draw(renderer);

		SDL_RenderCopy(renderer, playerTexture, NULL, &playerPos);
		SDL_RenderCopy(renderer, turretTexture, NULL, &turretPos);
		SDL_RenderPresent(renderer);

	}
	SDL_DestroyWindow(window);

	SDL_Quit();


	return 0;
}
