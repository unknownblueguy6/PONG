#include "game.hpp"

void runGameWithHumans();

int gameMode = ON_MENU;  

int main(int argc, char* argv[]){
	init();

	while(!quit() && !QUIT_GAME){
		switch(gameMode){
			case ON_MENU:
				gameMode = menu();
				break;
			case VS_COMP:
				clearScreen();
				renderText("Coming Soon", 100, SCREEN_WIDTH/2, 250);
				SDL_RenderPresent(gRenderer);
				SDL_Delay(1000);
				gameMode = ON_MENU;
				break;
			case VS_HUMAN:
				runGameWithHumans();
				break;
		}
	}

	close();

	return 0;
}

void runGameWithHumans(){
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
