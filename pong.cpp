#include "game.hpp"
#include "ai.hpp"

void vsPlayer();
void vsComp(); 

int main(int argc, char* argv[]){
	init();

	while(!quit() && !QUIT_GAME){
		switch(gameMode){
			case ON_MENU:
				gameMode = menu();
				break;
			case VS_COMP:
				vsComp();
				break;
			case VS_HUMAN:
				vsPlayer();
				break;
		}
	}

	close();

	return 0;
}

void vsPlayer(){
	FPS_Timer capTimer;

	newGame();

	while(true){
		
		capTimer.start();
		
		if(quit()){
			QUIT_GAME = true;
			break;
		}

		PLAYERONE.move();
		PLAYERTWO.move();
		
		
		BALL.move();

		if(BALL.collisionWithWall()){
			BALL.reboundFromWall();
		}
		
		if(BALL.collisionWith(PLAYERONE)){
			BALL.reboundFrom(PLAYERONE);
		}
		else if(BALL.collisionWith(PLAYERTWO)){
			BALL.reboundFrom(PLAYERTWO);
		}


		if (BALL.outOfBounds()){
			updateScore();
			PLAYERONE.reset();
			PLAYERTWO.reset();
			BALL.reset();
		}
		
		renderAll();

		capTimer.capFPS();
	}
}

void vsComp(){
	FPS_Timer capTimer;

	newGame(gameMode);

	while(true){
		
		capTimer.start();
		
		if(quit()){
			QUIT_GAME = true;
			break;
		}

		PLAYERONE.move();
		COMPUTER.getBallPosition();
		COMPUTER.move();
		
		
		BALL.move();

		if(BALL.collisionWithWall()){
			BALL.reboundFromWall();
		}
		
		if(BALL.collisionWith(PLAYERONE)){
			BALL.reboundFrom(PLAYERONE);
		}
		else if(BALL.collisionWith(COMPUTER.COMP)){
			BALL.reboundFrom(COMPUTER.COMP);
		}


		if (BALL.outOfBounds()){
			updateScore();
			PLAYERONE.reset();
			COMPUTER.COMP.reset();
			BALL.reset();
		}
		
		renderAll(gameMode);

		capTimer.capFPS();
	}
}