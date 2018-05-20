#include "game.hpp"

void runGame();

int main(int argc, char* argv[]){
	init();

	runGame();

	close();

	return 0;
}

void runGame(){
	FPS_Timer capTimer;

	newGame();

	while(true){
		
		capTimer.start();
		
		if(quit()){
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