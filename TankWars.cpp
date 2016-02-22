
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include "Tank.h"
#include "Turret.h"

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
	Tank tank1 = Tank(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 50.0f, 50.0f);

	Turret turret1 = Turret(renderer, images_dir.c_str(), audio_dir.c_str(), 800.0f, 500.0f);

	SDL_Texture *ground = IMG_LoadTexture(renderer, (images_dir + "Ground.png").c_str());

	SDL_Rect groundRect;

	groundRect.x = 0;
	groundRect.y = 0;
	groundRect.w = 2048;
	groundRect.h = 1536;

	float X_pos = 0.0f;
	float Y_pos = 0.0f;

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
			}else{
				X_pos = groundRect.x;
			}
		}

		if((tank1.posRect.x <= 0) && (tank1.Xvalue < 8000)){
			X_pos += (tank1.speed) * deltaTime;

			if((groundRect.x < 0)){
				groundRect.x = (int) (X_pos + 0.5f);
			}else{
				X_pos = groundRect.x;
			}
		}

		if((tank1.posRect.y >= 768 - tank1.posRect.h) && (tank1.Yvalue > 8000)){
					Y_pos -= (tank1.speed) * deltaTime;

					if((groundRect.y > -768)){
						groundRect.y = (int) (Y_pos + 0.5f);
					}else{
						Y_pos = groundRect.y;
					}
				}

		if((tank1.posRect.y <= 0) && (tank1.Yvalue < 8000)){
					Y_pos += (tank1.speed) * deltaTime;

					if((groundRect.y < 0)){
						groundRect.y = (int) (Y_pos + 0.5f);
					}else{
						Y_pos = groundRect.y;
					}
				}


		turret1.Update(deltaTime, tank1.posRect);

		// DRAW ***********************
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, ground,NULL,&groundRect);

		tank1.Draw(renderer);

		turret1.Draw(renderer);
		SDL_RenderPresent(renderer);

	}
	SDL_DestroyWindow(window);

	SDL_Quit();


	return 0;
}
