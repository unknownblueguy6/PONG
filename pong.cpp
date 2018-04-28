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
		
		collision(BALL);
		collision(BALL, PLAYERONE);
		collision(BALL, PLAYERTWO);

		if (outOfBounds(BALL)){
			reset();
		}
		BALL.move();
		
		clearScreen();

		PLAYERONE.renderToScreen();
		PLAYERTWO.renderToScreen();
		BALL.renderToScreen();

		render();

		capTimer.capFPS();
	}

	close();

	return 0;
}