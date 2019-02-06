//This file just executes all the functions of game.hpp and gui.hpp
#include "game.hpp"

void runGame();

int main(int argc, char* argv[]){
	init();

	while(!quit() && !QUIT_GAME){
		switch(gameMode){
			case ON_MENU:
				gameMode = menu();
				break;
			case VS_COMP:
			case VS_HUMAN:
				runGame();
				break;
		}
	}

	close();

	return 0;
}

void runGame(){
	FPS_Timer capTimer;

	newGame();

	while(true){

		if(victory()){
			break;
		}

		capTimer.start();

		if(quit()){
			QUIT_GAME = true;
			break;
		}

		PLAYERONE.move();

		if(gameMode == VS_HUMAN){
			PLAYERTWO.move();
		}
		else{
			COMPUTER.takeAction();
		}


		BALL.move();

		if(BALL.collisionWithWall()){
			BALL.reboundFromWall();
		}

		if(BALL.collisionWith(PLAYERONE)){
			BALL.reboundFrom(PLAYERONE);
			COMPUTER.getBallPosition();
			COMPUTER.makeDecision();
		}
		else if(gameMode == VS_HUMAN && BALL.collisionWith(PLAYERTWO)){
				BALL.reboundFrom(PLAYERTWO);
		}
		else if (gameMode == VS_COMP && BALL.collisionWith(COMPUTER.COMP)){
				BALL.reboundFrom(COMPUTER.COMP);
		}


		if (BALL.outOfBounds()){
			updateScore();
			PLAYERONE.reset();
			if(gameMode == VS_HUMAN){
				PLAYERTWO.reset();
			}
			else{
				COMPUTER.COMP.reset();
			}
			BALL.reset();
			if(BALL.vx > 0){
				COMPUTER.getBallPosition();
				COMPUTER.makeDecision();
			}
		}

		renderAll();

		capTimer.capFPS();
	}
	if(!QUIT_GAME){
		displayWinner();
		clearScreen();
		if(playAgain()){
			runGame();
		}

		else{
			QUIT_GAME = true;
		}
	}
}
