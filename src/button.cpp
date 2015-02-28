/*
----------------------------------
	~Moosey Chess Engine~
	      button.cpp
----------------------------------
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "button.h"
#include "board.h"
#include "ltexture.h"
#include "display.h"

Button::Button() {
	pos.x = 0;
	pos.y = 0;
	inside = false;
	clicking = false;
	butt = 0;
}

void Button::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

bool Button::handleEvent(SDL_Event* e, Board& b) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		inside = true;
		if (x < pos.x) 
			inside = false;
		else if (x > pos.x + BUT_SIZEX)
			inside = false;
		else if (y < pos.y)
			inside = false;
		else if (y > pos.y + BUT_SIZEY)
			inside = false;
		if (inside) {
			switch (e->type) { 
					case SDL_MOUSEBUTTONDOWN:
					clicking = true;
					if (butt == 1 && b.getPly() > 0) {
						b.undoMove();
						return true;
					}
					else if (butt == 0 && b.getPly() > 0) {
						b.restart();
						return true;
					}
					break;
					case SDL_MOUSEBUTTONUP:
					clicking = false;
					break;
			}
		}
	return false;
}
