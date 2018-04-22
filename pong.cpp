#ifdef _WIN32 
#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

#include <string.h>
#include "timer.h"

enum Axes
{
	X,
	Y
};

class Particle
{
	public:
		void move();
		void changeVel(bool choice, int v);
		unsigned getPos(bool choice);
		
		int vx; //velocity along x-axis
		int vy; //velocity along y-axis
		unsigned x; //x coordinate
		unsigned y; //y coordinate
};

void Particle :: move(){
	x += vx;
	y += vy;
}

void Particle :: changeVel(bool choice, int v){
	if (!choice){
		vx = v;
	}
	else{
		vy = v;
	}
}

unsigned Particle :: getPos(bool choice){
	if (!choice){
		return x;
	}
	else{
		return y;
	}
}

const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;

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

	Particle p1 = {0,0, 20, 30}, p2 = {0, 0, 50, 30};


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

		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear( gRenderer );

		SDL_Rect fillRect = { p1.x, p1.y, SCREEN_WIDTH / 30, SCREEN_HEIGHT / 5};
		SDL_Rect part = {p2.getPos(X), p2.getPos(Y), SCREEN_WIDTH / 30, SCREEN_HEIGHT / 5};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer, &fillRect );
		SDL_RenderFillRect(gRenderer, &part);

		SDL_RenderPresent( gRenderer );

		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME ){
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}
	}

	close();

	return 0;
}