/*
----------------------------------
	~Moosey Chess Engine~
	      button.cpp
----------------------------------
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "button.h"
#include "board.h"
#include "ltexture.h"
#include "display.h"
#include "common.h"

Button::Button() {
	pos.x = pos.y = 0;
	w = h = 0;
	inside = clicking = false;
	butt = 0;
	boardPtr = NULL;
}

void Button::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Button::setSize(int x, int y) {
	setW(x);
	setH(y);
}

bool Button::handleEvent(SDL_Event* e, int& sound) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		inside = true;
		if (x < pos.x) 
			inside = false;
		else if (x > pos.x + w)
			inside = false;
		else if (y < pos.y)
			inside = false;
		else if (y > pos.y + h)
			inside = false;
		if (inside) {
			switch (e->type) { 
					case SDL_MOUSEBUTTONDOWN:
					clicking = true;
					if (!muted) {
						if (butt == 25 && !start)
							sound = 2;
						else {
							if ((butt > 1 && !start) || butt < 2)
								sound = 1;
						}
					}
					//Restart button
					if (butt == 0) {
						boardPtr->restart();
						return true;
					}
					//Undo button
					else if (butt == 1) {
						boardPtr->undoMove();
						return true;
					}
					//White, "Human"
					else if (butt == 2 && !start)
						boardPtr->setWhiteIsBot(false);
					//White, "Computer"
					else if (butt == 3 && !start)
						boardPtr->setWhiteIsBot(true);
					//Black, "Human"
					else if (butt == 4 && !start)
						boardPtr->setBlackIsBot(false);
					//Black, "Computer"
					else if (butt == 5 && !start)
						boardPtr->setBlackIsBot(true);
					//White computer levels 1-9
					else if (butt >= 6 && butt <= 14 &&
						 boardPtr->getWhiteIsBot() && !start)
						boardPtr->whiteBot.setLevel(butt-5);
					//Black computer levels 1-9
					else if (butt >= 15 && butt <= 23 && 
						 boardPtr->getBlackIsBot() && !start)
						boardPtr->blackBot.setLevel(butt-14);
					//Flip board
					else if (butt == 24 && !start) 
						boardPtr->flipBoard();
					//Start
					else if (butt == 25)
						start = true;
					break;
					case SDL_MOUSEBUTTONUP:
					clicking = false;
					break;
			}
		}
	return false;
}
