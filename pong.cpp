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
			p2.changeVel(Y, -10);
			p2.move();
		}
		if(keyStates[SDL_SCANCODE_DOWN]){
			p2.changeVel(Y, 10);
			p2.move();
		}
		if(keyStates[SDL_SCANCODE_W]){
			p1.changeVel(Y, -10);
			p1.move();
		}
		if(keyStates[SDL_SCANCODE_S]){
			p1.changeVel(Y, 10);
			p1.move();
		}
		
		collision(ball);
		ball.move();
		SDL_Rect p1Rect = {p1.x, p1.y, p1.w, p1.h}, p2Rect = {p2.x, p2.y, p2.w, p2.h}, ballRect = {ball.x, ball.y, ball.w, ball.h};
		
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear(gRenderer);

		
		
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer,&p1Rect);
		SDL_RenderFillRect(gRenderer, &p2Rect);
		SDL_RenderFillRect(gRenderer, &ballRect);

		SDL_RenderPresent(gRenderer);

		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME ){
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}
	}

	close();

	return 0;
}