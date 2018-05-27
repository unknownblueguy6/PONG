#pragma once
#include "game.hpp"
#include <cstdlib>
#include <ctime>

class AI
{
	public:
		void getBallPosition();
		void move();

		Particle COMP;
		Particle tempBALL;
} COMPUTER;

void AI :: getBallPosition(){
	tempBALL = BALL;
	//int time = 0;
	while(tempBALL.x < COMP.x && BALL.vx > 0){
		tempBALL.move();
		if (tempBALL.collisionWithWall()){
			tempBALL.reboundFromWall();
		}
		//++time;
	}
	//return time;
}

void AI :: move(){
	srand(time(NULL));
	//int availableTime = timeToReachBall();

	int s = tempBALL.y <= COMP.y ? tempBALL.y - COMP.getCorner(Y, TR) : tempBALL.y - COMP.getCorner(Y, BR);
	int dir;
	if (s != 0) dir = std::abs(s)/s;
	else dir = 0;

	// int requiredTime = (float)(((float)(-COMP.vy) + std :: sqrt((float)((COMP.vy * COMP.vy) + 2*COMP.ay*s))) / (float)(COMP.ay));
	// if (requiredTime < 0) requiredTime = (float)(((float)(-COMP.vy) - std :: sqrt((float)((COMP.vy * COMP.vy) + 2*COMP.ay*s))) / (float)(COMP.ay));
	
	int chance  = rand() % 100 + 1;
	if (!tempBALL.collisionWith(COMP) && BALL.vx > 0 && dir != 0){
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
	else{
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

void newGame(int);
void updateScore(int);
void renderAll(int);


void newGame(int gameMode){
	BALL.reset();
	PLAYERONE.reset();
	PLAYERONE.score = 0;
	COMPUTER.COMP.reset();
	COMPUTER.COMP.score = 0;	
}

void updateScore(int gameMode){
	if(BALL.vx < 0){
		++COMPUTER.COMP.score;
	}
	else if(BALL.vx > 0){
		++PLAYERONE.score;
	}

}

void renderAll(int gameMode){
		clearScreen();

		PLAYERONE.renderToScreen();
		COMPUTER.COMP.renderToScreen();
		BALL.renderToScreen();

		renderCenterLine();

		renderText(std::to_string(PLAYERONE.score) + " " + std::to_string(COMPUTER.COMP.score), 100, SCREEN_WIDTH/2, 45);

		SDL_RenderPresent(gRenderer);
}