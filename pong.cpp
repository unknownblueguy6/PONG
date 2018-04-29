#include "game.h"

int main(int argc, char* argv[]){
	init();

	FPS_Timer capTimer;

	newGame();

	while(true){
		
		capTimer.start();
		
		if(quit()){
			break;
		}

		const Uint8* keyStates = SDL_GetKeyboardState( NULL );


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
			PLAYERONE.reset();
			PLAYERTWO.reset();
			BALL.reset();
		}
		
		renderAll();

		capTimer.capFPS();
	}

	close();

	return 0;
}