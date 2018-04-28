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

		// if(keyStates[SDL_SCANCODE_UP]){
		// 	PLAYERTWO.changeVel(Y, -PLAYER_TWO_VEL_Y);
		// 	PLAYERTWO.move();
		// 	if(PLAYERTWO.y < 0){
		// 		PLAYERTWO.y = 0;
		// 	}
		// 	collision(BALL, PLAYERTWO);
		// 	PLAYERTWO.changeVel(Y, 0);
		// }
		// if(keyStates[SDL_SCANCODE_DOWN]){
		// 	PLAYERTWO.changeVel(Y, PLAYER_TWO_VEL_Y);
		// 	PLAYERTWO.move();
		// 	if (PLAYERTWO.y > SCREEN_HEIGHT - PLAYERTWO.h){
		// 		PLAYERTWO.y = SCREEN_HEIGHT - PLAYERTWO.h;
		// 	}
		// 	collision(BALL, PLAYERTWO);
		// 	PLAYERTWO.changeVel(Y, 0);
		// }
		// if(keyStates[SDL_SCANCODE_W]){
		// 	PLAYERONE.changeVel(Y, -PLAYER_ONE_VEL_Y);
		// 	PLAYERONE.move();
		// 	if(PLAYERONE.y < 0){
		// 		PLAYERONE.y = 0;
		// 	}
		// 	collision(BALL, PLAYERONE);
		// 	PLAYERONE.changeVel(Y, 0);
		// }
		// if(keyStates[SDL_SCANCODE_S] && PLAYERONE.y < SCREEN_HEIGHT - PLAYERONE.h){
		// 	PLAYERONE.changeVel(Y, PLAYER_ONE_VEL_Y);
		// 	PLAYERONE.move();
		// 	if (PLAYERONE.y > SCREEN_HEIGHT - PLAYERTWO.h){
		// 		PLAYERONE.y = SCREEN_HEIGHT - PLAYERTWO.h;
		// 	}
		// 	collision(BALL, PLAYERONE);
		// 	PLAYERONE.changeVel(Y, 0);
		// }

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