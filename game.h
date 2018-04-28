#pragma once
#include "gui.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Change base attributes here.
const int PLAYER_ONE_VEL_X = 0;
const int PLAYER_ONE_VEL_Y = 10;
const int PLAYER_ONE_POS_X = 10;
const int PLAYER_ONE_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5;
const unsigned PLAYER_ONE_WIDTH = SCREEN_WIDTH/30;
const unsigned PLAYER_ONE_HEIGHT = SCREEN_HEIGHT/5;

const int PLAYER_TWO_VEL_X = 0;
const int PLAYER_TWO_VEL_Y = 10;
const int PLAYER_TWO_POS_X = SCREEN_WIDTH - 10 - SCREEN_WIDTH/30;
const int PLAYER_TWO_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5;
const unsigned PLAYER_TWO_WIDTH = SCREEN_WIDTH/30;
const unsigned PLAYER_TWO_HEIGHT = SCREEN_HEIGHT/5;

const int BALL_VEL_X = 5;
const int BALL_VEL_Y = 10;
const unsigned BALL_WIDTH = 20;
const unsigned BALL_HEIGHT = 20; 
const int BALL_POS_X = SCREEN_WIDTH/2 - BALL_WIDTH/2;
const int BALL_POS_Y = SCREEN_HEIGHT/2 - BALL_HEIGHT; 

//*******************************************************************************************//
class FPS_Timer
{
    public:
		//Initializes variables
		FPS_Timer();

		//The various clock actions
		void start();
		void capFPS();

		//Gets the timer's time
		Uint32 getTicks();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;
};

FPS_Timer::FPS_Timer()
{
    //Initialize the variables
    mStartTicks = 0;
}

void FPS_Timer::start()
{
    //Get the current clock time
    mStartTicks = SDL_GetTicks();
}

void FPS_Timer :: capFPS(){
	int frameTicks = getTicks();
	if( frameTicks < SCREEN_TICK_PER_FRAME ){
		SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
	}
}

Uint32 FPS_Timer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    time = SDL_GetTicks() - mStartTicks;

    return time;
}

//*******************************************************************************************//


//*******************************************************************************************//
enum axes
{
	X,
	Y
};

enum corners{
	TL, //top left corner
	TR, //top right corner
	BL, //bottom left
	BR //bottom right
};


class Particle
{
	public:
		void move();
		void move(int);
		void changeVel(bool, int);
		void reverse(bool);
		void renderToScreen();
		
		int getCorner(bool, int);
		int whichPlayer();

		int vx; //velocity along x-axis
		int vy; //velocity along y-axis
		int x;
		int y;
		unsigned w;
		unsigned h;
};

/////////////////////////////////////////
Particle PLAYERONE, PLAYERTWO, BALL;
/////////////////////////////////////////

void Particle :: move(){
	if(whichPlayer() == -1){
		x += vx;
		y += vy;
	}


	else if(whichPlayer() == 1){
		if(keyStates[SDL_SCANCODE_W]){
			changeVel(Y, -PLAYER_ONE_VEL_Y);
			x += vx;
			y += vy;
			if(y < 0) y = 0;
		}
		if(keyStates[SDL_SCANCODE_S] && PLAYERONE.y < SCREEN_HEIGHT - PLAYERONE.h){
			changeVel(Y, PLAYER_ONE_VEL_Y);
			x += vx;
			y += vy;
			if (y > SCREEN_HEIGHT - PLAYERTWO.h) y = SCREEN_HEIGHT - PLAYERTWO.h;
		}
		changeVel(Y, 0);
	}


	else if(whichPlayer() == 2){
		if(keyStates[SDL_SCANCODE_UP]){
			changeVel(Y, -PLAYER_TWO_VEL_Y);
			x += vx;
			y += vy;
			if(y < 0) y = 0;
		}
		if(keyStates[SDL_SCANCODE_DOWN]){
			changeVel(Y, PLAYER_TWO_VEL_Y);
			x += vx;
			y += vy;
			if (y > SCREEN_HEIGHT - h) y = SCREEN_HEIGHT - h;
		}
		changeVel(Y, 0);
	}
}


void Particle :: changeVel(bool choice, int v){
	if (choice == X){
		vx = v;
	}
	else{
		vy = v;
	}
}

void Particle :: reverse(bool choice){
	if(!choice){
		vx *= -1;
	}
	else{
		vy *= -1;
	}
}

int Particle :: getCorner(bool choice1, int choice2){
	if(choice1 == X){
		if(choice2 == TL){
			return x;
		}
		if(choice2 == TR){
			return x+w;
		}
		if(choice2 == BL){
			return x;
		}
		if(choice2 == BR){
			return x+w;
		}
	}
	else{
		if(choice2 == TL){
			return y;
		}
		if(choice2 == TR){
			return y;
		}
		if(choice2 == BL){
			return y+h;
		}
		if(choice2 == BR){
			return y+h;
		}
	}
}

int Particle :: whichPlayer(){
	if (x == PLAYER_ONE_POS_X && h == PLAYER_ONE_HEIGHT) return 1;
	if (x == PLAYER_TWO_POS_X && h == PLAYER_TWO_HEIGHT) return 2;
	return -1;
}

void Particle :: renderToScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_Rect particleRect = {x, y, w, h};
	SDL_RenderFillRect( gRenderer,&particleRect);
}

//*******************************************************************************************//

//GAME FUNCTIONS

void newGame();
void collision(Particle&);
void collision(Particle&, Particle);
int getCollisionPoint(bool, int, int, int, int, int);
void reset();
bool outOfBounds(Particle);

void newGame(){
	PLAYERONE = {PLAYER_ONE_VEL_X, PLAYER_ONE_VEL_Y, PLAYER_ONE_POS_X, PLAYER_ONE_POS_Y, PLAYER_ONE_WIDTH, PLAYER_ONE_HEIGHT};
	PLAYERTWO = {PLAYER_TWO_VEL_X, PLAYER_TWO_VEL_Y, PLAYER_TWO_POS_X, PLAYER_TWO_POS_Y, PLAYER_TWO_WIDTH, PLAYER_TWO_HEIGHT};
	BALL = {BALL_VEL_X, BALL_VEL_Y, BALL_POS_X, BALL_POS_Y, BALL_WIDTH, BALL_HEIGHT};
}

void collision(Particle &a){
		if (a.y <= 0 || a.y >= SCREEN_HEIGHT - a.h){
			a.reverse(Y);
		}
}

int getCollisionPoint(bool choice, int x1, int y1, int vx, int vy, int corner){
	float m = (float)vy/vx;
	if(choice == X){
		return x1 + (corner-y1)/m;
	}
	else{
		return y1 + m*(corner - x1);
	}
}

void collision(Particle &ball, Particle player){
	
	if(ball.getCorner(X, TL) > player.getCorner(X, TL) && ball.getCorner(X, TL) < player.getCorner(X, BR) && ball.getCorner(Y, TL) > player.getCorner(Y, TL) && ball.getCorner(Y, TL) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy < player.getCorner(Y, BR)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TL), ball.getCorner(Y, TL), ball.vx, ball.vy, player.getCorner(X, BR));
			ball.x = player.getCorner(X, BR);
		}
		else if(ball.y - ball.vy > player.getCorner(Y, BR)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TL), ball.getCorner(Y, TL), ball.vx, ball.vy, player.getCorner(Y, BR));
			ball.y = player.getCorner(Y, BR);
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, BR);
			ball.y = player.getCorner(Y, BR);
			//add special here
		}
		ball.reverse(X);
		//return true;
	}
	if(ball.getCorner(X, TR) > player.getCorner(X, TL) && ball.getCorner(X, TR) < player.getCorner(X, BR) && ball.getCorner(Y, TR) > player.getCorner(Y, TL) && ball.getCorner(Y, TR) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy < player.getCorner(Y, BL)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(X, BL));
			ball.x = player.getCorner(X, BL) - ball.w;
		}
		else if(ball.y - ball.vy > player.getCorner(Y, BL)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(Y, BL)) - ball.w;
			ball.y = player.getCorner(Y, BL);
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, BL)-ball.w;
			ball.y = player.getCorner(Y, BL);
			//add special here
		}
		ball.reverse(X);
		//return true;
	}
	if(ball.getCorner(X, BL) > player.getCorner(X, TL) && ball.getCorner(X, BL) < player.getCorner(X, BR) && ball.getCorner(Y, BL) > player.getCorner(Y, TL) && ball.getCorner(Y, BL) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy > player.getCorner(Y, TR)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(X, TR)) - ball.h;
			ball.x = player.getCorner(X, TR);
		}
		else if(ball.y - ball.vy < player.getCorner(Y, TR)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(Y, TR));
			ball.y = player.getCorner(Y, TR) - ball.h;
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, TR);
			ball.y = player.getCorner(Y, TR) - ball.h;
			//add special here
		}
		ball.reverse(X);
		//return true;
	}
	if(ball.getCorner(X, BR) > player.getCorner(X, TL) && ball.getCorner(X, BR) < player.getCorner(X, BR) && ball.getCorner(Y, BR) > player.getCorner(Y, TL) && ball.getCorner(Y, BR) < player.getCorner(Y, BR)){
		if(ball.y - ball.vy > player.getCorner(Y, TL)){
			ball.y = getCollisionPoint(Y, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(X, TL)) - ball.h;
			ball.x = player.getCorner(X, TL) - ball.w;
		}
		else if(ball.y - ball.vy < player.getCorner(Y, TL)){
			ball.x = getCollisionPoint(X, ball.getCorner(X, TR), ball.getCorner(Y, TR), ball.vx, ball.vy, player.getCorner(Y, TL)) - ball.w;
			ball.y = player.getCorner(Y, TL) - ball.h;
			ball.changeVel(Y, -(ball.vy - player.vy));
		}
		else{
			ball.x = player.getCorner(X, TL) - ball.w;
			ball.y = player.getCorner(Y, TL) - ball.h;
			//add special here
		}
		ball.reverse(X);
		//return true;
	}
	//return false;
}

void reset(){

		BALL.x = BALL_POS_X;
		BALL.y = BALL_POS_Y;
		BALL.vx = BALL_VEL_X;
		BALL.vy = BALL_VEL_Y;

		PLAYERONE.x = PLAYER_ONE_POS_X;
		PLAYERONE.y = PLAYER_ONE_POS_Y;
		PLAYERONE.vx = PLAYER_ONE_VEL_X;
		PLAYERONE.vy = PLAYER_ONE_VEL_Y;

		PLAYERTWO.x = PLAYER_TWO_POS_X;
		PLAYERTWO.y = PLAYER_TWO_POS_Y;
		PLAYERTWO.vx = PLAYER_TWO_VEL_X;
		PLAYERTWO.vy = PLAYER_TWO_VEL_Y;

}

bool outOfBounds(Particle ball){
	return (ball.x + ball.w <= 0 || ball.x >= SCREEN_WIDTH);
}