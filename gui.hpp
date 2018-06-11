#pragma once

#ifdef _WIN32 
#include <SDL.h>
#include <SDL_ttf.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <string>
#include <random>

const unsigned SCREEN_WIDTH = 640;
const unsigned SCREEN_HEIGHT = 480;

bool QUIT_GAME = false;
enum gameStates{
	VS_COMP = 1,
	VS_HUMAN,
	ON_MENU
};

//Random Number Generator
std::mt19937 rng;

int gameMode = ON_MENU; 

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
	rng.seed(std::random_device()());
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

int menu(){
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	clearScreen();
	renderText("PONG", 250, SCREEN_WIDTH/2, 125);
	renderText("VS COMP", 75, SCREEN_WIDTH/2, 250);
	renderText("VS PLAYER", 75, SCREEN_WIDTH/2, 325);
	static int pos = VS_COMP;
	int renderPos = 250;
	
	if(keyStates[SDL_SCANCODE_UP]) pos = VS_COMP;
	if(keyStates[SDL_SCANCODE_DOWN]) pos = VS_HUMAN;
	if(keyStates[SDL_SCANCODE_RETURN]) return pos;
	
	if (pos == VS_COMP) renderPos = 250;
	else renderPos = 325;
	
	renderText(">", 100, SCREEN_WIDTH/2 - 200, renderPos + 5);
	
	SDL_RenderPresent(gRenderer);
	return ON_MENU;
}

void clearScreen(){
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
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
