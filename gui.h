#pragma once

#ifdef _WIN32 
#include <SDL.h>

#else
#include <SDL2/SDL.h>
#endif

#include <string.h>

const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;

SDL_Window* gWindow = 0;

SDL_Renderer* gRenderer = 0;

const Uint8* keyStates = SDL_GetKeyboardState( NULL );

void init();
void close();
void clearScreen();
void render();
bool quit();


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

void clearScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
	SDL_RenderClear(gRenderer);
}

void render(){
	SDL_RenderPresent(gRenderer);
}

bool quit(){
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0){
			
		if( e.type == SDL_QUIT ){
			return true;
		}
	}
	return false;
}
