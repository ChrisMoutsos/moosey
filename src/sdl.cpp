#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "ltexture.h"
#include <sdl.h>
#include <stdio.h>
#include <string>
#include <cmath>

SDL_Window* window; //The window we'll be rendering to
SDL_Renderer* renderer; //The window renderer

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
		window = SDL_CreateWindow("Moosey Chess Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
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
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;
	
	if (!spriteSheetTexture.loadFromFile("../res/spritesheet.png")) {
		success = false;
	}
	else {
	}

	return success;
}

void close_SDL() {
	//Free loaded images
	spriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
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