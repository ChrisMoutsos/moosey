/*
----------------------------------
	~Moosey Chess Engine~
	      sdl.cpp
----------------------------------
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "ltexture.h"
#include "square.h"
#include "button.h"
#include "sdl.h"

SDL_Window* window = NULL; //The window we'll be rendering to
SDL_Renderer* renderer = NULL; //The window renderer
TTF_Font* Garamond26 = NULL, * Garamond28 = NULL, 
	* Cicero22= NULL, * Cicero26 = NULL;
Mix_Chunk * mFSound = NULL, * mTSound = NULL;

bool init_SDL() {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			printf("Warning: Linear texture filtering not enabled!");

		//Create window
		window = SDL_CreateWindow("Moosey Chess Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (!TTF_Init() == -1) {
					printf("SDL_ttf could not be initialized.\n");
					success = false;
				}
		
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					printf("SDL_mixer could not be initialized.\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;
	Garamond28 = TTF_OpenFont("../res/Garamond.ttf", 28);
	Cicero22 = TTF_OpenFont("../res/Cicero.ttf", 22);
	Cicero26 = TTF_OpenFont("../res/Cicero.ttf", 26);
	Garamond26 = TTF_OpenFont("../res/Garamond.ttf", 26);

	if (Garamond28 == NULL || Garamond26 == NULL)
		success = false;
	else if (Cicero26 == NULL || Cicero22 == NULL)
		success = false;
	else 
		textColor = {0, 0, 0};

	mFSound = Mix_LoadWAV("../res/moveFrom.wav");
	if (mFSound == NULL)
		success = false;
	mTSound = Mix_LoadWAV("../res/moveTo.wav");	
	if (mTSound == NULL)
		success = false;
	
	if (!spriteSheetTexture.loadFromFile("../res/spritesheet2.bmp"))
		success = false;
	if (!buttonTexture.loadFromFile("../res/buttons.bmp"))
		success = false;
	
	SDL_Surface* icon = IMG_Load("../res/icon.png");	
	SDL_SetWindowIcon(window, icon);

	return success;
}

void close_SDL() {
	//Free loaded images
	spriteSheetTexture.free();
	buttonTexture.free();

	//Free sounds
	Mix_FreeChunk(mFSound);
	Mix_FreeChunk(mTSound);
	mFSound = NULL;
	mTSound = NULL;

	//Free global font
	TTF_CloseFont(Garamond26);
	TTF_CloseFont(Garamond28);
	TTF_CloseFont(Cicero22);
	TTF_CloseFont(Cicero26);
	Garamond26 = NULL;
	Garamond28 = NULL;
	Cicero22 = NULL;
	Cicero26 = NULL;

	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
	SDL_Texture* newTexture = NULL; //The final texture

	SDL_Surface* loadedSurface = IMG_Load(path.c_str()); //Load image at specified path
	if (loadedSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	else {
		//Create texture from surface pixels
        	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) 
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}
