#pragma once

#ifdef _WIN32 
#include <SDL.h>
#include <SDL_ttf.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <string>

const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;

SDL_Window* gWindow = 0;

SDL_Renderer* gRenderer = 0;

void init();
void close();
void changeFontSize(int);
void renderText(std::string, int, int, int, int, int);
void clearScreen();
void render();
bool quit();


void init(){
	SDL_Init(SDL_INIT_VIDEO);
	gWindow = SDL_CreateWindow("P O N G", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	TTF_Init();
}

void close(){
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = 0;
	gRenderer = 0;
	TTF_Quit();
	SDL_Quit();
}

void renderText(std::string text, int size, int x, int y, int red = 0xFF, int green = 0xFF, int blue = 0xFF){
	SDL_Color textColour = {red, green, blue};
	TTF_Font* font = 0;
	font = TTF_OpenFont( "ARCADE.TTF", size);
	
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColour);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	int width = textSurface->w;
	int height = textSurface->h;

	SDL_Rect textRect = {x - width/2, y - height/2, width, height};

	SDL_RenderCopy(gRenderer, textTexture, 0, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	textTexture = 0;
	TTF_CloseFont(font);
}

void clearScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
	SDL_RenderClear(gRenderer);
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
