#ifdef _WIN32 
#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

#include <string.h>
#include "timer.h"
#include "game.h"

SDL_Window* gWindow = 0;

SDL_Renderer* gRenderer = 0;

void init();
void close();

void init(){
	SDL_Init(SDL_INIT_VIDEO);
	gWindow = SDL_CreateWindow("P O N G", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void close(){
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = 0;
	gRenderer = 0;
	SDL_Quit();
}



int main(int argc, char* argv[]){
	init();

	bool quit = false;

	SDL_Event e;

	LTimer capTimer;

	newGame();



	while(!quit){
		
		capTimer.start();
		
		while(SDL_PollEvent(&e) != 0){
			
			if( e.type == SDL_QUIT ){
				quit = true;
			}
		}

		const Uint8* keyStates = SDL_GetKeyboardState( NULL );

		if(keyStates[SDL_SCANCODE_UP]){
			PLAYERTWO.changeVel(Y, -PLAYER_TWO_VEL_Y);
			PLAYERTWO.move();
			if(PLAYERTWO.y < 0){
				PLAYERTWO.y = 0;
			}
			collision(BALL, PLAYERTWO);
			PLAYERTWO.changeVel(Y, 0);
		}
		if(keyStates[SDL_SCANCODE_DOWN]){
			PLAYERTWO.changeVel(Y, PLAYER_TWO_VEL_Y);
			PLAYERTWO.move();
			if (PLAYERTWO.y > SCREEN_HEIGHT - PLAYERTWO.h){
				PLAYERTWO.y = SCREEN_HEIGHT - PLAYERTWO.h;
			}
			collision(BALL, PLAYERTWO);
			PLAYERTWO.changeVel(Y, 0);
		}
		if(keyStates[SDL_SCANCODE_W]){
			PLAYERONE.changeVel(Y, -PLAYER_ONE_VEL_Y);
			PLAYERONE.move();
			if(PLAYERONE.y < 0){
				PLAYERONE.y = 0;
			}
			collision(BALL, PLAYERONE);
			PLAYERONE.changeVel(Y, 0);
		}
		if(keyStates[SDL_SCANCODE_S] && PLAYERONE.y < SCREEN_HEIGHT - PLAYERONE.h){
			PLAYERONE.changeVel(Y, PLAYER_ONE_VEL_Y);
			PLAYERONE.move();
			if (PLAYERONE.y > SCREEN_HEIGHT - PLAYERTWO.h){
				PLAYERONE.y = SCREEN_HEIGHT - PLAYERTWO.h;
			}
			collision(BALL, PLAYERONE);
			PLAYERONE.changeVel(Y, 0);
		}
		
		collision(BALL);
		collision(BALL, PLAYERONE);
		collision(BALL, PLAYERTWO);
		reset(BALL);
		BALL.move();
		SDL_Rect PLAYERONERect = {PLAYERONE.x, PLAYERONE.y, PLAYERONE.w, PLAYERONE.h}; 
		SDL_Rect PLAYERTWORect = {PLAYERTWO.x, PLAYERTWO.y, PLAYERTWO.w, PLAYERTWO.h}; 
		SDL_Rect BALLRect = {BALL.x, BALL.y, BALL.w, BALL.h};
		
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear(gRenderer);

		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer,&PLAYERONERect);
		SDL_RenderFillRect(gRenderer, &PLAYERTWORect);
		SDL_RenderFillRect(gRenderer, &BALLRect);

		SDL_RenderPresent(gRenderer);

		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME ){
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}
	}

	close();

	return 0;
}