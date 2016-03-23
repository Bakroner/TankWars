#include "Tank.h"
const int JOYSTICK_DEAD_ZONE = 8000;

Tank::Tank(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	back = IMG_LoadTexture(renderer, (filePath + "health_1.png").c_str());
	mid = IMG_LoadTexture(renderer, (filePath + "health_2.png").c_str());
	front = IMG_LoadTexture(renderer, (filePath + "health_3.png").c_str());

	backR.x = midR.x = frontR.x = 10;
	backR.y = midR.y = frontR.y = 50;
	backR.w = midR.w = frontR.w = 239;
	backR.h = midR.h = frontR.h = 32;

	playerHealth = 100.0f;
	maxHealth = 100.0f;

	active = true;
	playerNum = pNum;
	speed = 200.0f;
	fire = Mix_LoadWAV((audioPath + "Fire.wav").c_str());
	if(playerNum == 0){
		playerPath = filePath + "TankProto.png";
	}else{
		playerPath = filePath + "BlueTank.png";
	}
	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	xDirOld = 1;
	yDirOld = 0;

	center.x = posRect.w/2;
	center.y = posRect.h/2;

	string bulletPath;

	if(playerNum == 0){
		bulletPath = filePath + "Red Bullet.png";
	}else{
		bulletPath = filePath + "Turret Bullet.png";
	}

	for (int i = 0; i < 10; i++)
	{
		TankBullet tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);

		bulletList.push_back(tmpBullet);
	}
}

void Tank::Update(float deltaTime)
{
	if (Xvalue != 0 || Yvalue != 0) {
		//x = posRect.x = xDir;
		//y = posRect.y = yDir;
		tankangle = atan2(Yvalue, Xvalue) * 180 / 3.14;
		oldAngle = tankangle;
		//xDirOld = xDir;
		//yDirOld = yDir;

		float radians = (tankangle * 3.14)/ 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		pos_X += (move_x) * deltaTime;
		pos_Y += (move_y) * deltaTime;

		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

	}else{
		tankangle = oldAngle;
	}
	//pos_X += (speed * xDir) * deltaTime;
	//pos_Y += (speed * yDir) * deltaTime;

	if (posRect.x < 0){
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if (posRect.x > 1024 - posRect.w){
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if (posRect.y < 0){
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if (posRect.y > 768 - posRect.h){
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}


	for (int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active){
			bulletList[i].Update(deltaTime);
		}
	}
}

void Tank::eTankHit(){

	playerHealth -= .025f;

	midR.w = playerHealth/maxHealth * 239;
}

void Tank::eBulletHit(){

	playerHealth -= 5;

	midR.w = playerHealth/maxHealth * 239;
}

void Tank::Draw(SDL_Renderer *renderer)
{

	for (int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active){
			bulletList[i].Draw(renderer);
		}
	}

	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, tankangle, &center, SDL_FLIP_NONE);

	SDL_RenderCopy(renderer, back, NULL, &backR);
	SDL_RenderCopy(renderer, mid, NULL, &midR);
	SDL_RenderCopy(renderer, front, NULL, &frontR);

}

void Tank::OnControllerAxis(Sint16 X, Sint16 Y)
{
	Xvalue = X;
	Yvalue = Y;

	if(!(Xvalue < -JOYSTICK_DEAD_ZONE) && !(Xvalue > JOYSTICK_DEAD_ZONE))
	{
		Xvalue = 0.0f;
	}

	if(!(Yvalue < -JOYSTICK_DEAD_ZONE) && !(Yvalue > JOYSTICK_DEAD_ZONE))
	{
		Yvalue = 0.0f;
	}
}

void Tank::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if (event.which == 0 && playerNum == 0)
	{
		if (event.button == 0)
		{
			//cout << "Player 1 - Button A" << endl;

			CreateBullet();
		}
	}
	if (event.which == 1 && playerNum == 1)
	{
		if (event.button == 0)
		{
			//cout << "Player 2 - Button A" << endl;

			CreateBullet();
		}
	}
}

void Tank::CreateBullet() {

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false) {

			Mix_PlayChannel(-1, fire, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
			bulletList[i].posRect.y = (posRect.y + (posRect.h / 2));

			bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w / 2);
			bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h / 2);

			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			if (Xvalue != 0 || Yvalue != 0) {
				bulletList[i].tankangle = tankangle;
			}
			else {
				bulletList[i].tankangle = oldAngle;
			}

			break;
		}
	}
}

