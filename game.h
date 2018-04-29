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

enum collision_type{
	TLBR, //top left corner of ball and bottom right corner of player
	TRBL, //top right corner of ball and bottom left corner of player
	BLTR, //bottom left corner of ball and top right corner of player
	BRTL //bottom right corner of ball and top left corner of player
};


class Particle
{
	public:
		void move();
		void changeVel(bool, int);
		void reverse(bool);
		void renderToScreen();
		void reboundFromWall();
		void reboundFrom(Particle);

		bool collisionWith(Particle);
		bool collisionWithWall();
		bool outOfBounds();
		
		int getCorner(bool, int);
		int getCollisionPoint(bool, int, int, int, int, int);
		int whichPlayer();

		int vx; //velocity along x-axis
		int vy; //velocity along y-axis
		int x;
		int y;
		unsigned w;
		unsigned h;
		unsigned collisionType;
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

void Particle :: renderToScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_Rect particleRect = {x, y, w, h};
	SDL_RenderFillRect( gRenderer,&particleRect);
}

void Particle :: reboundFromWall(){
	if (y <= 0){
		x = getCollisionPoint(X, getCorner(X, TL), getCorner(Y, TL), vx, vy, 0);
		y = 0;
		reverse(Y);
	}
	else if(y >= SCREEN_HEIGHT - h){
		x = getCollisionPoint(X, getCorner(X, BR), getCorner(Y, BR), vx, vy, SCREEN_HEIGHT) - w;
		y = SCREEN_HEIGHT -h;
		reverse(Y);
	}
}

void Particle :: reboundFrom(Particle player){
	if (collisionType == TLBR){
		if(y - vy < player.getCorner(Y, BR)){
			y = getCollisionPoint(Y, getCorner(X, TL), getCorner(Y, TL), vx, vy, player.getCorner(X, BR));
			x = player.getCorner(X, BR);
		}
		else if(y - vy > player.getCorner(Y, BR)){
			x = getCollisionPoint(X, getCorner(X, TL), getCorner(Y, TL), vx, vy, player.getCorner(Y, BR));
			y = player.getCorner(Y, BR);
			changeVel(Y, -(vy - player.vy));
		}
		else{
			x = player.getCorner(X, BR);
			y = player.getCorner(Y, BR);
			//add special here
		}
		reverse(X);
	}
	else if (collisionType == TRBL){
		if(y - vy < player.getCorner(Y, BL)){
			y = getCollisionPoint(Y, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(X, BL));
			x = player.getCorner(X, BL) - w;
		}
		else if(y - vy > player.getCorner(Y, BL)){
			x = getCollisionPoint(X, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(Y, BL)) - w;
			y = player.getCorner(Y, BL);
			changeVel(Y, -(vy - player.vy));
		}
		else{
			x = player.getCorner(X, BL)-w;
			y = player.getCorner(Y, BL);
			//add special here
		}
		reverse(X);		
	}
	else if (collisionType == BLTR){
		if(y - vy > player.getCorner(Y, TR)){
			y = getCollisionPoint(Y, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(X, TR)) - h;
			x = player.getCorner(X, TR);
		}
		else if(y - vy < player.getCorner(Y, TR)){
			x = getCollisionPoint(X, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(Y, TR));
			y = player.getCorner(Y, TR) - h;
			changeVel(Y, -(vy - player.vy));
		}
		else{
			x = player.getCorner(X, TR);
			y = player.getCorner(Y, TR) - h;
			//add special here
		}
		reverse(X);		
	}
	else if (collisionType == BRTL){
		if(y - vy > player.getCorner(Y, TL)){
			y = getCollisionPoint(Y, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(X, TL)) - h;
			x = player.getCorner(X, TL) - w;
		}
		else if(y - vy < player.getCorner(Y, TL)){
			x = getCollisionPoint(X, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(Y, TL)) - w;
			y = player.getCorner(Y, TL) - h;
			changeVel(Y, -(vy - player.vy));
		}
		else{
			x = player.getCorner(X, TL) - w;
			y = player.getCorner(Y, TL) - h;
			//add special here
		}
		reverse(X);
	} 
}

bool Particle :: collisionWith(Particle player){
	if (getCorner(X, TL) > player.getCorner(X, TL) && getCorner(X, TL) < player.getCorner(X, BR) && getCorner(Y, TL) > player.getCorner(Y, TL) && getCorner(Y, TL) < player.getCorner(Y, BR)){
		collisionType = TLBR;
		return true;
	} 
	if (getCorner(X, TR) > player.getCorner(X, TL) && getCorner(X, TR) < player.getCorner(X, BR) && getCorner(Y, TR) > player.getCorner(Y, TL) && getCorner(Y, TR) < player.getCorner(Y, BR)){
		collisionType = TRBL;
		return true;	
	}
	if (getCorner(X, BL) > player.getCorner(X, TL) && getCorner(X, BL) < player.getCorner(X, BR) && getCorner(Y, BL) > player.getCorner(Y, TL) && getCorner(Y, BL) < player.getCorner(Y, BR)){
		collisionType = BLTR;
		return true;
	}
	if (getCorner(X, BR) > player.getCorner(X, TL) && getCorner(X, BR) < player.getCorner(X, BR) && getCorner(Y, BR) > player.getCorner(Y, TL) && getCorner(Y, BR) < player.getCorner(Y, BR)){
		collisionType = BRTL;
		return true;
	}
	collisionType = -1;
	return false;
}

bool Particle :: collisionWithWall(){
	return (y <= 0 || y >= SCREEN_HEIGHT - h);
}

bool Particle :: outOfBounds(){
	return (x + w <= 0 || x >= SCREEN_WIDTH);
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

int Particle :: getCollisionPoint(bool choice, int x1, int y1, int vx, int vy, int corner){
	float m = (float)vy/vx;
	if(choice == X){
		return x1 + (corner-y1)/m;
	}
	else{
		return y1 + m*(corner - x1);
	}
}

int Particle :: whichPlayer(){
	if (x == PLAYER_ONE_POS_X && h == PLAYER_ONE_HEIGHT) return 1;
	if (x == PLAYER_TWO_POS_X && h == PLAYER_TWO_HEIGHT) return 2;
	return -1;
}

//*******************************************************************************************//

//GAME FUNCTIONS*****************************************************************************//

void newGame();
void reset();

void render();

void newGame(){
	PLAYERONE = {PLAYER_ONE_VEL_X, PLAYER_ONE_VEL_Y, PLAYER_ONE_POS_X, PLAYER_ONE_POS_Y, PLAYER_ONE_WIDTH, PLAYER_ONE_HEIGHT, 0};
	PLAYERTWO = {PLAYER_TWO_VEL_X, PLAYER_TWO_VEL_Y, PLAYER_TWO_POS_X, PLAYER_TWO_POS_Y, PLAYER_TWO_WIDTH, PLAYER_TWO_HEIGHT, 0};
	BALL = {BALL_VEL_X, BALL_VEL_Y, BALL_POS_X, BALL_POS_Y, BALL_WIDTH, BALL_HEIGHT, 0};
}

void reset(){
	newGame();
} 

void renderAll(){
		clearScreen();

		PLAYERONE.renderToScreen();
		PLAYERTWO.renderToScreen();
		BALL.renderToScreen();

		render();
}

//*******************************************************************************************//