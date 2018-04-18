#ifdef _WIN32 
#include <SDL.h>
#include <SDL_image.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <string.h>

class particle{
	unsigned vx; //velocity along x-axis
	unsigned vy; //velocity along y-axis
	unsigned x; //x coordinate
	unsigned y; //y coordinate
};

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
	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);
}

void close(){
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = 0;
	gRenderer = 0;

	IMG_Quit();
	SDL_Quit();
}



int main(int argc, char* argv[]){
	init();

	bool quit = false;

	SDL_Event e;

	bool UP = false, DOWN = false, RIGHT = false, LEFT= false;
	int x = 10, y = 40;

	while(!quit){

		while(SDL_PollEvent(&e) != 0){
			
			if( e.type == SDL_QUIT ){
				quit = true;
			}

			else if (e.type == SDL_KEYDOWN){

				switch (e.key.keysym.sym){
					case SDLK_UP: y -= 10;
					break; 
					case SDLK_DOWN: y += 10;
					break; 
					case SDLK_LEFT: x -= 10;
					break; 
					case SDLK_RIGHT: x += 10;
					break; 
				}
			}
		}

		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear( gRenderer );

		SDL_Rect fillRect = { x, y, SCREEN_WIDTH / 30, SCREEN_HEIGHT / 5};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer, &fillRect );

		SDL_RenderPresent( gRenderer );
	}

	close();

	return 0;
}