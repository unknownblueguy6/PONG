#pragma once
#include "gui.hpp"
#include <cmath>
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Change base attributes here.
const int PLAYER_ONE_VEL_X = 0;
const int PLAYER_ONE_VEL_Y = 10;
const int PLAYER_ONE_WIDTH = SCREEN_WIDTH/30;
const int PLAYER_ONE_HEIGHT = SCREEN_HEIGHT/5;
const int PLAYER_ONE_POS_X = 10 + PLAYER_ONE_WIDTH/2;
const int PLAYER_ONE_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5 + PLAYER_ONE_HEIGHT/2;

const int PLAYER_TWO_VEL_X = 0;
const int PLAYER_TWO_VEL_Y = 10;
const int PLAYER_TWO_WIDTH = SCREEN_WIDTH/30;
const int PLAYER_TWO_HEIGHT = SCREEN_HEIGHT/5;
const int PLAYER_TWO_POS_X = SCREEN_WIDTH - 10 - SCREEN_WIDTH/30 + PLAYER_TWO_WIDTH/2;
const int PLAYER_TWO_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5 + PLAYER_TWO_HEIGHT/2;

const unsigned BALL_MAX_VEL_Y = 15;
const unsigned BALL_MIN_VEL_Y = 1;
const int BALL_VEL_X = 5;
const int BALL_VEL_Y = 10;
const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15; 
const int BALL_POS_X = SCREEN_WIDTH/2;
const int BALL_POS_Y = SCREEN_HEIGHT/2; 

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
	//Member functions	
		Particle(int, int, int, int, int, int);
		void move();
		void renderToScreen();
		void changeVel(int, int);
		void reboundFromWall();
		void reboundFrom(Particle);
		void reset();

		bool collisionWith(Particle);
		bool collisionWithWall();
		bool outOfBounds();
		
		int getCorner(bool, int);
		int getNearestCorner(Particle);
		double slopeDiff(int, int);
		int getCollisionPoint(bool, int, int, int);
		int whichPlayer();

	//Data members 	
	int score = 0;
	int vx; //velocity along x-axis
	int vy; //velocity along y-axis
	private:
		int x; //x coordinate of centre 
		int y; //y coordinates of centre
		int w; //width of particle
		int h; //height of particle
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Particle PLAYERONE(PLAYER_ONE_VEL_X, PLAYER_ONE_VEL_Y, PLAYER_ONE_POS_X, PLAYER_ONE_POS_Y, PLAYER_ONE_WIDTH, PLAYER_ONE_HEIGHT);
Particle PLAYERTWO(PLAYER_TWO_VEL_X, PLAYER_TWO_VEL_Y, PLAYER_TWO_POS_X, PLAYER_TWO_POS_Y, PLAYER_TWO_WIDTH, PLAYER_TWO_HEIGHT);
Particle BALL(BALL_VEL_X, BALL_VEL_Y, BALL_POS_X, BALL_POS_Y, BALL_WIDTH, BALL_HEIGHT);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Particle :: Particle(int a, int b, int c, int d, int e, int f){
	vx = a;
	vy = b;
	x = c;
	y = d;
	w = e;
	h = f;	
}

void Particle :: move(){
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	if(whichPlayer() == -1){
		x += vx;
		y += vy;
	}


	else if(whichPlayer() == 1){
		if(keyStates[SDL_SCANCODE_W]){
			vy = -PLAYER_ONE_VEL_Y;
			x += vx;
			y += vy;
			if(y  - h/2 < 0) y = h/2;
		}
		if(keyStates[SDL_SCANCODE_S]){
			vy = PLAYER_ONE_VEL_Y;
			x += vx;
			y += vy;
			if (y + h/2 > SCREEN_HEIGHT) y = SCREEN_HEIGHT - h/2;
		}
		vy = 0;
	}


	else if(whichPlayer() == 2){
		if(keyStates[SDL_SCANCODE_UP]){
			vy = -PLAYER_TWO_VEL_Y;
			x += vx;
			y += vy;
			if(y  - h/2 < 0) y = h/2;
		}
		if(keyStates[SDL_SCANCODE_DOWN]){
			vy = PLAYER_TWO_VEL_Y;
			x += vx;
			y += vy;
			if (y + h/2 > SCREEN_HEIGHT) y = SCREEN_HEIGHT - h/2;
		}
		vy = 0;
	}
}

void Particle :: changeVel(int vel, int dir){
	//dir = 1 does not change direction, dir = -1 changes direction
	int initialDir = std :: abs(vy)/vy;
	if (std :: abs(vel) > BALL_MAX_VEL_Y) vy = dir * initialDir * BALL_MAX_VEL_Y;
	else if (std :: abs(vel) < BALL_MIN_VEL_Y) vy = dir * initialDir * BALL_MIN_VEL_Y;
	else vy = dir * initialDir * std::abs(vel);
}

void Particle :: renderToScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_Rect particleRect = {getCorner(X, TL), getCorner(Y, TL), w, h};
	SDL_RenderFillRect( gRenderer,&particleRect);
}

void Particle :: reboundFromWall(){
	if (y - h/2 <= 0){
		x = getCollisionPoint(X, x, y, h/2);
		y = h/2;
		vy *= -1;
	}
	else if(y + h/2 >= SCREEN_HEIGHT){
		x = getCollisionPoint(X, x, y, SCREEN_HEIGHT - h/2);
		y = SCREEN_HEIGHT - h/2;
		vy *= -1;
	}
}

void Particle :: reboundFrom(Particle player){
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	if(keyStates[SDL_SCANCODE_W] && player.whichPlayer() == 1 && player.y != player.h/2) 
		player.vy = -PLAYER_ONE_VEL_Y;
	else if(keyStates[SDL_SCANCODE_S] && player.whichPlayer() == 1 && player.y != SCREEN_HEIGHT - player.h/2) 
		player.vy = PLAYER_ONE_VEL_Y;
	else if(keyStates[SDL_SCANCODE_UP] && player.whichPlayer() == 2 && player.y != player.h/2) 
		player.vy = -PLAYER_TWO_VEL_Y;
	else if(keyStates[SDL_SCANCODE_DOWN] && player.whichPlayer() == 2 && player.y != SCREEN_HEIGHT - player.h/2)
		player.vy = PLAYER_TWO_VEL_Y;
	
	y -= vy;
	x -= vx;
	
	if(collisionWith(player)){
		int corner = getNearestCorner(player);
		int y1 = player.getCorner(Y, corner); 
		y1 += (corner == BL || corner == BR) ? h/2 : -h/2;
		x = getCollisionPoint(X, x, y, y1);
		y = y1;
		changeVel(vy - player.vy, 1);
		if(((corner == BR || corner == TR) && vx < 0 && getCorner(X, TL) >= player.x) ||
			((corner == BL || corner == TL) && vx > 0 && getCorner(X, TR) <= player.x))
			vx *= -1;
	}

	else{
		int corner = getNearestCorner(player);
		int x1 = player.getCorner(X, corner), y1 = player.getCorner(Y, corner);

		x1 += (corner == TR || corner == BR) ? w/2 : -w/2;
		y1 += (corner == BL || corner == BR) ? h/2 : -h/2;
		
		if(((corner == TL || corner == TR) && y < y1) ||
			((corner == BL || corner == BR) && y > y1)){
			
			double slopeChange = slopeDiff(x1, y1); 
			if(corner == TL || corner == BR) slopeChange *= -1; 

			if (slopeChange < 0){
				y = getCollisionPoint(Y, x, y, x1);
				x = x1;
				changeVel(vy - player.vy, 1);
				vx *= -1;
			} 
			else if (slopeChange > 0){
				x = getCollisionPoint(X, x, y, y1);
				y = y1;
				changeVel(vy - player.vy, -1);
				if ((vx < 0 && getCorner(X, TL) >= player.x) || 
 					(vx > 0 && getCorner(X, TR) <= player.x)) vx *= -1;	
			}
			else{
				x = x1, y = y1;
				vy = 20 * -(std :: abs(vy)/vy);
				vx = 10 * -(std :: abs(vx)/vx);
			} 
		}
			
		else{
			y = getCollisionPoint(Y, x, y, x1);
			x = x1;
			changeVel(vy - player.vy, 1);
			vx *= -1;	
		}
	}
	
	if(collisionWithWall()){
		if(y - h/2 < 0){
			y = h/2;
		}
		else{
			y = SCREEN_HEIGHT - h/2;
		}
		if(collisionWith(player)){
			if(vx > 0){
				x = player.getCorner(X, BR) + w/2;
			}
			else{
				x = player.getCorner(X, BL) - w/2;
			}
		}
	}
}

void Particle :: reset(){
	if (whichPlayer() == -1){
		if(vx < 0){
			vx = BALL_VEL_X;	
		}
		else if(vx > 0){
			vx = -BALL_VEL_X;
		}
		else{
			vx = BALL_VEL_X;
		}
		vy = BALL_VEL_Y;
		x = BALL_POS_X;
		y = BALL_POS_Y;
	}
	else if (whichPlayer() == 1){
		x = PLAYER_ONE_POS_X;
		y = PLAYER_ONE_POS_Y;
	}
	else if (whichPlayer() == 2){
		x = PLAYER_TWO_POS_X;
		y = PLAYER_TWO_POS_Y;
	}
}

bool Particle :: collisionWith(Particle player){
	return ((x >= player.getCorner(X, TL) - w/2 && x <= player.getCorner(X, BR) + w/2) &&
			(y >= player.getCorner(Y, TL) - h/2 && y <= player.getCorner(Y, BR) + h/2));
}

bool Particle :: collisionWithWall(){
	return (y - h/2 <= 0 || y  + h/2 >= (int)(SCREEN_HEIGHT));
}

bool Particle :: outOfBounds(){
	return (x + w/2 <= 0 || x  - w/2 >= (int)(SCREEN_WIDTH));
}

double Particle :: slopeDiff(int x1, int y1){
	float m1 = (float)((float)(y - y1)/(float)(x - x1));
	float m2 = (float)((float)(vy)/(float)(vx));
	return m1 - m2; 
}

int Particle :: getCorner(bool choice1, int choice2){
	switch(choice1){
		case X:
			switch(choice2){
				case TL: return x - w/2;
				case TR: return x + w/2;
				case BL: return x - w/2;
				case BR: return x + w/2;
			}
			break;
		case Y:
			switch(choice2){
				case TL: return y - h/2;
				case TR: return y - h/2;
				case BL: return y + h/2;
				case BR: return y + h/2;
			}
		break;
	}
}

int Particle :: getNearestCorner(Particle player){
	int c1, c2;
	if(player.whichPlayer() == 1) c1 = TR, c2 = BR;
	else c1 = TL, c2 = BL;
	
	if (abs(abs(player.getCorner(Y, c1)) - abs(y)) <= abs(abs(player.getCorner(Y, c2)) - abs(y))) return c1; 
	return c2; 
}

int Particle :: getCollisionPoint(bool choice, int x1, int y1, int coordinate){
	float m = (float)((float)(vy)/(float)(vx));
	
	if(choice == X) return x1 + (coordinate-y1)/m;
	
	else return y1 + m*(coordinate - x1);
}

int Particle :: whichPlayer(){
	if (x == PLAYER_ONE_POS_X && h == PLAYER_ONE_HEIGHT) return 1;
	if (x == PLAYER_TWO_POS_X && h == PLAYER_TWO_HEIGHT) return 2;
	return -1;
}

//*******************************************************************************************//

//GAME FUNCTIONS*****************************************************************************//

void newGame();
void updateScore();
void render();


void newGame(){
	BALL.reset();
	PLAYERONE.reset();
	PLAYERONE.score = 0;
	PLAYERTWO.reset();
	PLAYERTWO.score = 0;	
}

void updateScore(){
	if(BALL.vx < 0){
		++PLAYERTWO.score;
	}
	else if(BALL.vx > 0){
		++PLAYERONE.score;
	}

}

void renderAll(){
		clearScreen();

		PLAYERONE.renderToScreen();
		PLAYERTWO.renderToScreen();
		BALL.renderToScreen();

		renderText(std::to_string(PLAYERONE.score) + " " + std::to_string(PLAYERTWO.score), 100, SCREEN_WIDTH/2, 45);

		SDL_RenderPresent(gRenderer);
}

//*******************************************************************************************//