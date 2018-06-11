#pragma once
#include "gui.hpp"
#include "attributes.hpp"
#include <iostream>
#include <cmath>

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
		Particle();
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
		static int count; 	
		int score = 0;
		int vx; //velocity along x-axis
		int vy; //velocity along y-axis
		int ay; //acceleration along y - axis 
		int x; //x coordinate of centre 
		int y; //y coordinates of centre
		int w; //width of particle
		int h; //height of particle
		bool isAI;
};

int Particle :: count = 0;

Particle PLAYERONE;
Particle PLAYERTWO;
Particle BALL;

Particle :: Particle(){
	readValues();
	if(count == 0){
		vx = 0;
		vy = 0;
		ay = PLAYER_ONE_ACC_Y;
		x = PLAYER_ONE_POS_X;
		y = PLAYER_ONE_POS_Y; 
		w = PLAYER_ONE_WIDTH;
		h = PLAYER_ONE_HEIGHT;
		++count;
	}
	else if(count == 1 || count == 3){
		vx = 0;
		vy = 0;
		ay = PLAYER_TWO_ACC_Y;
		x = PLAYER_TWO_POS_X;
		y = PLAYER_TWO_POS_Y; 
		w = PLAYER_TWO_WIDTH;
		h = PLAYER_TWO_HEIGHT;
		if(count == 3) isAI = 1;
		++count;
	}
	else if(count == 2 || count > 3){
		vx = BALL_VEL_X;
		vy = BALL_VEL_Y;
		x = BALL_POS_X;
		y = BALL_POS_Y; 
		w = BALL_WIDTH;
		h = BALL_HEIGHT;
		++count;
	}
}

void Particle :: move(){
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	
		if(whichPlayer() == -1){
		x += vx;
		y += vy;
	}


	else if(whichPlayer() == 1){
		if(keyStates[SDL_SCANCODE_W]){
			vy -= ay;
			if(std :: abs(vy) > PLAYER_ONE_MAX_VEL_Y) vy = -PLAYER_ONE_MAX_VEL_Y;
			x += vx;
			y += vy;
			if(y  - h/2 < 0) {
				y = h/2;
				vy = 0;
			}
		}
		else if(keyStates[SDL_SCANCODE_S]){
			vy += ay;
			if(std :: abs(vy) > PLAYER_ONE_MAX_VEL_Y) vy = +PLAYER_ONE_MAX_VEL_Y;
			x += vx;
			y += vy;
			if (y + h/2 > SCREEN_HEIGHT){
				y = SCREEN_HEIGHT - h/2;
				vy = 0;
			}
		}
		else{
			if (vy > 0){
				vy -= ay;
				if(vy < PLAYER_ONE_MIN_VEL_Y) vy = PLAYER_ONE_MIN_VEL_Y;
			}	
			else{
				vy += ay;
				if(vy > PLAYER_ONE_MIN_VEL_Y) vy = PLAYER_ONE_MIN_VEL_Y;
			}
		}
	}


	else if(whichPlayer() == 2){
		if(keyStates[SDL_SCANCODE_UP]){
			vy -= ay;
			if(std :: abs(vy) > PLAYER_TWO_MAX_VEL_Y) vy = -PLAYER_TWO_MAX_VEL_Y;
			x += vx;
			y += vy;
			if(y  - h/2 < 0) {
				y = h/2;
				vy = 0;
			}
		}
		else if(keyStates[SDL_SCANCODE_DOWN]){
			vy += ay;
			if(std :: abs(vy) > PLAYER_TWO_MAX_VEL_Y) vy = +PLAYER_TWO_MAX_VEL_Y;
			x += vx;
			y += vy;
			if (y + h/2 > SCREEN_HEIGHT){
				y = SCREEN_HEIGHT - h/2;
				vy = 0;
			}
		}
		else{
			if (vy > 0){
				vy -= ay;
				if(vy < PLAYER_TWO_MIN_VEL_Y) vy = PLAYER_TWO_MIN_VEL_Y;
			}	
			else{
				vy += ay;
				if(vy > PLAYER_TWO_MIN_VEL_Y) vy = PLAYER_TWO_MIN_VEL_Y;
			}
		}
	}
}

void Particle :: changeVel(int vel, int dir){
	//dir = 1 does not change direction, dir = -1 changes direction
	int initialDir = std :: abs(vy)/vy;
	if (std :: abs(vel) > abs(BALL_MAX_VEL_Y)) vy = dir * initialDir * abs(BALL_MAX_VEL_Y);
	else if (std :: abs(vel) < abs(BALL_MIN_VEL_Y)) vy = dir * initialDir * abs(BALL_MIN_VEL_Y);
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
	y -= vy;
	x -= vx;

	int corner = getNearestCorner(player);
	int x1 = player.getCorner(X, corner);
	int y1 = player.getCorner(Y, corner);
	x1 += (corner == TR || corner == BR) ? w/2 : -w/2;
	y1 += (corner == BL || corner == BR) ? h/2 : -h/2;
	
	if(collisionWith(player)){ 
		x = getCollisionPoint(X, x, y, y1);
		y = y1;
		
		if(!collisionWith(player)){
			y = getCollisionPoint(Y, x, y, x1);
			x = x1;
		}
		
		if((corner == BR || corner == BL && vy < 0) || 
			(corner == TR || corner == TL && vy > 0)) changeVel(vy - player.vy, -1);
		
		else changeVel(vy - player.vy, 1);
		
		if(((corner == BR || corner == TR) && vx < 0 && getCorner(X, TL) >= player.x) ||
			((corner == BL || corner == TL) && vx > 0 && getCorner(X, TR) <= player.x))
			vx *= -1;
	}

	else{
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
				x = player.getCorner(X, BR) + w/2 + 1;
			}
			else{
				x = player.getCorner(X, BL) - w/2 - 1;
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
	else if (whichPlayer() == 2 || whichPlayer() == 3){
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
	if(x == PLAYER_ONE_POS_X && h == PLAYER_ONE_HEIGHT && !isAI) return 1;
	if(x == PLAYER_TWO_POS_X && h == PLAYER_TWO_HEIGHT && !isAI) return 2;
	if(isAI) return 3;
	return -1;
}

//*******************************************************************************************//

// AI code //

enum decisions{
	REACH_POSITION_BEFORE_BALL = 1,
	REACH_BALL_IN_REQUIRED_TIME,
	CLOSE_MISS,
	NORMAL_MISS,
	TERRIBLE_MISS
};

class AI
{
	public:
		void getBallPosition();
		void makeDecision();
		void getRequiredTime();
		void move(int);
		void takeAction();

		int decision;
		int availableTime;
		int requiredTime;
		int distance;
		int direction;
		Particle COMP;
		Particle tempBALL;
};

AI COMPUTER;

void AI :: getBallPosition(){
	tempBALL = BALL;
	int time = 0;
	while(tempBALL.x < COMP.x && BALL.vx > 0){
		tempBALL.move();
		if (tempBALL.collisionWithWall()){
			tempBALL.reboundFromWall();
		}
		++time;
	}
	availableTime = time;

	distance = tempBALL.y <= COMP.y ? tempBALL.y - COMP.getCorner(Y, TR) : tempBALL.y - COMP.getCorner(Y, BR);
	if (distance != 0) direction = std::abs(distance)/distance;
	else direction = 0;

}

void AI :: makeDecision(){
	//not really making decisions, just a simple RNG
	std::uniform_int_distribution<std::mt19937::result_type> dist100(1,100);

	int chance = dist100(rng);

	if(chance <= 80){ //hit the ball
		if(chance <= 24) decision = REACH_POSITION_BEFORE_BALL;
		else decision = REACH_BALL_IN_REQUIRED_TIME;
	}
	else{//miss the ball
		if(chance <= 98) decision = CLOSE_MISS;
		else if(decision <= 99) decision = NORMAL_MISS;
		else decision = TERRIBLE_MISS;
	}

}


void AI :: move(int dir){
	COMP.vy += dir * COMP.ay;
	if(std :: abs(COMP.vy) > PLAYER_TWO_MAX_VEL_Y) COMP.vy = dir * PLAYER_TWO_MAX_VEL_Y;
	COMP.y += COMP.vy;
	if(COMP.y  - COMP.h/2 < 0) {
		COMP.y = COMP.h/2;
		COMP.vy = 0;
	}
	if (COMP.y + COMP.h/2 > SCREEN_HEIGHT){
		COMP.y = SCREEN_HEIGHT - COMP.h/2;
		COMP.vy = 0;
	}

}

void AI :: getRequiredTime(){
	double root1 = (double)(((double)(-COMP.vy) + std :: pow((COMP.vy * COMP.vy + 2 * direction * COMP.ay * distance), 0.5)) / COMP.ay);
	double root2 = (double)(((double)(-COMP.vy) - std :: pow((COMP.vy * COMP.vy + 2 * direction * COMP.ay * distance), 0.5)) / COMP.ay);
	
	if(root1 >= 0 && root2 >= 0) requiredTime = root1 > root2 ? root2 : root1;
	
	else if(root1 < 0 && root2 >= 0) requiredTime = root2;

	else if(root1 >= 0 && root2 < 0) requiredTime = root1;

}
void AI :: takeAction(){
	distance = tempBALL.y <= COMP.y ? tempBALL.getCorner(Y, BR) - COMP.getCorner(Y, TR) : tempBALL.getCorner(Y, TR) - COMP.getCorner(Y, BR);
	--availableTime;
	getRequiredTime();
	static bool override = false;
	bool madeMove = false;
	
	Particle tempBALLCopy = tempBALL;

	switch(decision){
		case REACH_POSITION_BEFORE_BALL:
			std :: cout  << decision;
			std :: cout  << "\nbefore ball\n";
			if(tempBALLCopy.collisionWith(COMP)){
				tempBALLCopy.reboundFrom(COMP);
				if(tempBALLCopy.vx > 0){
					override = true;
				}
			}
			
			if (((!tempBALL.collisionWith(COMP)) && BALL.vx > 0 && direction != 0) || override){
				move(direction);
				madeMove = true;
				if(override){
					override = false;
				}
			}

			break;

		case REACH_BALL_IN_REQUIRED_TIME:
		//std :: cout  << "reqd time\n";
			if(tempBALL.collisionWith(COMP) && availableTime > requiredTime + 10){
				if(direction == 0){
					std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);
					direction = dist2(rng) % 2 == 0 ? -1 : +1;
				}
				move(-direction);
				madeMove = true;

			}
			else if ((!tempBALL.collisionWith(COMP) && availableTime <= requiredTime + 10) || override){
				move(direction);
				madeMove = true;
				if(tempBALLCopy.collisionWith(COMP)){
					tempBALLCopy.reboundFrom(COMP);
					if(tempBALLCopy.vx > 0){
						override = true;
					}
				}
				if(tempBALLCopy.vx < 0 && override){
					override = false;
				}
			}

			break;

		case CLOSE_MISS:
			if(tempBALL.collisionWith(COMP) || availableTime > requiredTime - 1){
				if(direction == 0){
					std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);
					direction = dist2(rng) % 2 == 0 ? -1 : +1;
				}
				move(direction);
				madeMove = true;
			}
			else if(!tempBALL.collisionWith(COMP) && availableTime <= requiredTime - 1){
				move(direction);
				madeMove = true;
			}

			break;

		case NORMAL_MISS:
		//std :: cout  << "normal\n";
			if(tempBALL.collisionWith(COMP)){
				if(direction == 0){
					std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);
					direction = dist2(rng) % 2 == 0 ? -1 : +1;
				}
				move(-direction);
				madeMove = true;
			}
			break;
		case TERRIBLE_MISS:
		//std :: cout  << "terrible\n";
			move(-direction);
			madeMove = true;
			if(tempBALL.collisionWith(COMP)){
				direction *= -1;
			}
			break;
		default:
			break;
	}
	if(!madeMove){
		if(COMP.vy > 0){
			COMP.vy -= COMP.ay;
			if(COMP.vy < PLAYER_TWO_MIN_VEL_Y) COMP.vy = PLAYER_TWO_MIN_VEL_Y;
		}	
		else{
			COMP.vy += COMP.ay;
			if(COMP.vy > PLAYER_TWO_MIN_VEL_Y) COMP.vy = PLAYER_TWO_MIN_VEL_Y;
		}
	}
}

//*******************************************************************************************//

//GAME FUNCTIONS*****************************************************************************//

void newGame();
void updateScore();
void renderCenterLine();
void renderAll();


void newGame(){
	BALL.reset();
	PLAYERONE.reset();
	PLAYERONE.score = 0;
	if(gameMode == VS_HUMAN){
		PLAYERTWO.reset();
		PLAYERTWO.score = 0;
	}
	else{
		COMPUTER.COMP.reset();
		COMPUTER.COMP.score = 0;
	}	
}

void updateScore(){
	if(BALL.vx < 0){
		if(gameMode == VS_HUMAN)
			++PLAYERTWO.score;
		else{
			++COMPUTER.COMP.score;
		}
	}
	else if(BALL.vx > 0){
		++PLAYERONE.score;
	}

}

void renderCenterLine(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	for(int ypos = 0; ypos < SCREEN_HEIGHT; ypos += 18){
		SDL_Rect particleRect = {SCREEN_WIDTH/2 - 3, ypos, 6, 12};
		SDL_RenderFillRect( gRenderer,&particleRect);	
	}
	
}

void renderAll(){
		clearScreen();
		std::string scoreText;
		
		PLAYERONE.renderToScreen();
		
		if(gameMode == VS_HUMAN){
			PLAYERTWO.renderToScreen();
			scoreText  = std::to_string(PLAYERONE.score) + " " + std::to_string(PLAYERTWO.score);
		}
		else{
			COMPUTER.COMP.renderToScreen();
			scoreText  = std::to_string(PLAYERONE.score) + " " + std::to_string(COMPUTER.COMP.score);
		}
		
		BALL.renderToScreen();

		renderCenterLine();

		renderText(scoreText, 100, SCREEN_WIDTH/2, 45);

		SDL_RenderPresent(gRenderer);
}

//*******************************************************************************************//
