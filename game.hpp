#pragma once
#include "gui.hpp"

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Change base attributes here.
const int PLAYER_ONE_VEL_X = 0;
const int PLAYER_ONE_VEL_Y = 10;
const int PLAYER_ONE_POS_X = 10;
const int PLAYER_ONE_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5 + 50;
const unsigned PLAYER_ONE_WIDTH = SCREEN_WIDTH/30;
const unsigned PLAYER_ONE_HEIGHT = SCREEN_HEIGHT/5;

const int PLAYER_TWO_VEL_X = 0;
const int PLAYER_TWO_VEL_Y = 10;
const int PLAYER_TWO_POS_X = SCREEN_WIDTH - 10 - SCREEN_WIDTH/30;
const int PLAYER_TWO_POS_Y = SCREEN_HEIGHT/2 - SCREEN_HEIGHT/5;
const unsigned PLAYER_TWO_WIDTH = SCREEN_WIDTH/30;
const unsigned PLAYER_TWO_HEIGHT = SCREEN_HEIGHT/5;

const int BALL_MAX_VEL_Y = 15;
const int BALL_MIN_VEL_Y = 1;
const int BALL_VEL_X = 5;
const int BALL_VEL_Y = 10;
const unsigned BALL_WIDTH = 15;
const unsigned BALL_HEIGHT = 15; 
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
		Particle(int, int, int, int, unsigned, unsigned);
		void move();
		void renderToScreen();
		void reboundFromWall();
		void reboundFrom(Particle);
		void reset();

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Particle PLAYERONE(PLAYER_ONE_VEL_X, PLAYER_ONE_VEL_Y, PLAYER_ONE_POS_X, PLAYER_ONE_POS_Y, PLAYER_ONE_WIDTH, PLAYER_ONE_HEIGHT),
		 PLAYERTWO(PLAYER_TWO_VEL_X, PLAYER_TWO_VEL_Y, PLAYER_TWO_POS_X, PLAYER_TWO_POS_Y, PLAYER_TWO_WIDTH, PLAYER_TWO_HEIGHT),
		 BALL(BALL_VEL_X, BALL_VEL_Y, BALL_POS_X, BALL_POS_Y, BALL_WIDTH, BALL_HEIGHT);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Particle :: Particle(int a, int b, int c, int d, unsigned e, unsigned f){
	vx = a;
	vy = b;
	x = c;
	y = d;
	w = e;
	h = f;
	collisionType = -1;	
}

void Particle :: move(){
	if(whichPlayer() == -1){
		x += vx;
		y += vy;
	}


	else if(whichPlayer() == 1){
		if(keyStates[SDL_SCANCODE_W]){
			vy = -PLAYER_ONE_VEL_Y;
			x += vx;
			y += vy;
			if(y < 0) y = 0;
		}
		if(keyStates[SDL_SCANCODE_S] && PLAYERONE.y < SCREEN_HEIGHT - PLAYERONE.h){
			vy = PLAYER_ONE_VEL_Y;
			x += vx;
			y += vy;
			if (y > SCREEN_HEIGHT - PLAYERTWO.h) y = SCREEN_HEIGHT - PLAYERTWO.h;
		}
		vy = 0;
	}


	else if(whichPlayer() == 2){
		if(keyStates[SDL_SCANCODE_UP]){
			vy = -PLAYER_TWO_VEL_Y;
			x += vx;
			y += vy;
			if(y < 0) y = 0;
		}
		if(keyStates[SDL_SCANCODE_DOWN]){
			vy = PLAYER_TWO_VEL_Y;
			x += vx;
			y += vy;
			if (y > SCREEN_HEIGHT - h) y = SCREEN_HEIGHT - h;
		}
		vy = 0;
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
		vy *= -1;
	}
	else if(y >= SCREEN_HEIGHT - h){
		x = getCollisionPoint(X, getCorner(X, BR), getCorner(Y, BR), vx, vy, SCREEN_HEIGHT) - w;
		y = SCREEN_HEIGHT -h;
		vy *= -1;
	}
}

void Particle :: reboundFrom(Particle player){
	if(keyStates[SDL_SCANCODE_W] && player.whichPlayer() == 1) player.vy = -PLAYER_ONE_VEL_Y;
	else if(keyStates[SDL_SCANCODE_S] && player.whichPlayer() == 1) player.vy = PLAYER_ONE_VEL_Y;
	else if(keyStates[SDL_SCANCODE_UP] && player.whichPlayer() == 2) player.vy = -PLAYER_TWO_VEL_Y;
	else if(keyStates[SDL_SCANCODE_S] && player.whichPlayer() == 2) player.vy = PLAYER_TWO_VEL_Y;

	if (collisionType == TLBR){
		if(y - vy < player.getCorner(Y, BR)){
			y = getCollisionPoint(Y, getCorner(X, TL), getCorner(Y, TL), vx, vy, player.getCorner(X, BR));
			x = player.getCorner(X, BR);
			vy =  (vy - player.vy);
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = BALL_MIN_VEL_Y;
			}		
			if(collisionWithWall()){
				y = SCREEN_HEIGHT - h;
			}
		}
		else if(y - vy > player.getCorner(Y, BR)){
			x = getCollisionPoint(X, getCorner(X, TL), getCorner(Y, TL), vx, vy, player.getCorner(Y, BR));
			y = player.getCorner(Y, BR);
			vy =  -(vy - player.vy);
			if(collisionWithWall()){
				y = SCREEN_HEIGHT - h;
				x = player.getCorner(X, BR);
			}
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = BALL_MIN_VEL_Y;
			}
		}
		else{
			x = player.getCorner(X, BR);
			y = player.getCorner(Y, BR);
			if(collisionWithWall()){
				y = SCREEN_HEIGHT - h; 
			}
			//add special here
		}
		vx *= -1;
	}
	else if (collisionType == TRBL){
		if(y - vy < player.getCorner(Y, BL)){
			y = getCollisionPoint(Y, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(X, BL));
			x = player.getCorner(X, BL) - w;
			vy =  (vy - player.vy);
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = BALL_MIN_VEL_Y;
			}			
			if(collisionWithWall()){
				y = SCREEN_HEIGHT - h;
			}
		}
		else if(y - vy > player.getCorner(Y, BL)){
			x = getCollisionPoint(X, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(Y, BL)) - w;
			y = player.getCorner(Y, BL);
			vy =  -(vy - player.vy);
			if(collisionWithWall()){
				y = SCREEN_HEIGHT - h;
				x = player.getCorner(X, BL) - w;
			}
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = BALL_MIN_VEL_Y;
			}
		}
		else{
			x = player.getCorner(X, BL) - w;
			y = player.getCorner(Y, BL);
			if(collisionWithWall()){
				y = SCREEN_HEIGHT - h; 
			}
			//add special here
		}
		vx *= -1;		
	}
	else if (collisionType == BLTR){
		if(y - vy > player.getCorner(Y, TR)){
			y = getCollisionPoint(Y, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(X, TR)) - h;
			x = player.getCorner(X, TR);
			vy =  (vy - player.vy);
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = BALL_MIN_VEL_Y;
			}			
			if(collisionWithWall()){
				y = 0;
			}
		}
		else if(y - vy < player.getCorner(Y, TR)){
			x = getCollisionPoint(X, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(Y, TR));
			y = player.getCorner(Y, TR) - h;
			vy =  -(vy - player.vy);
			if(collisionWithWall()){
				y = 0;
				x = player.getCorner(X, TR); 
			}
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = -BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = -BALL_MIN_VEL_Y;
			}
		}
		else{
			x = player.getCorner(X, TR);
			y = player.getCorner(Y, TR) - h;
			if(collisionWithWall()){
				y = 0; 
			}
			//add special here
		}
		vx *= -1;		
	}
	else if (collisionType == BRTL){
		if(y - vy > player.getCorner(Y, TL)){
			y = getCollisionPoint(Y, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(X, TL)) - h;
			x = player.getCorner(X, TL) - w;
			vy =  (vy - player.vy);
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = BALL_MIN_VEL_Y;
			}			
			if(collisionWithWall()){
				y = 0;
			}
		}
		else if(y - vy < player.getCorner(Y, TL)){
			x = getCollisionPoint(X, getCorner(X, TR), getCorner(Y, TR), vx, vy, player.getCorner(Y, TL)) - w;
			y = player.getCorner(Y, TL) - h;
			vy =  -(vy - player.vy);
			if(collisionWithWall()){
				y = 0;
				x = player.getCorner(X, TL) - w;
			}
			if (abs(vy) > BALL_MAX_VEL_Y){
				vy = -BALL_MAX_VEL_Y;
			}
			if (abs(vy) < BALL_MIN_VEL_Y){
				vy = -BALL_MIN_VEL_Y;
			}
		}
		else{
			x = player.getCorner(X, TL) - w;
			y = player.getCorner(Y, TL) - h;
			if(collisionWithWall()){
				y = 0;
			}
			//add special here
		}
		vx *= -1;
	} 
}

void Particle :: reset(){
	if (whichPlayer() == -1){
		vx = BALL_VEL_X;
		vy = BALL_VEL_Y;
		x = BALL_POS_X;
		y = BALL_POS_Y;
		collisionType = -1;
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
void render();


void newGame(){
	BALL.reset();
	PLAYERONE.reset();
	PLAYERTWO.reset();	
}

void renderAll(){
		clearScreen();

		PLAYERONE.renderToScreen();
		PLAYERTWO.renderToScreen();
		BALL.renderToScreen();

		render();
}

//*******************************************************************************************//