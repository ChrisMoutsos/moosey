/*
----------------------------------
	~Moosey Chess Engine~
	      sdl.h
----------------------------------
*/

#ifndef SDL_H
#define SDL_H

class LTexture;

const int SCREEN_W = 1250;
const int SCREEN_H = 700;

bool init_SDL();
bool loadMedia();
void close_SDL();

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern LTexture spriteSheetTexture, undoTexture, restartTexture;
extern SDL_Color textColor;

#endif
