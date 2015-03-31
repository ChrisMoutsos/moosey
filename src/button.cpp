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
}

void Button::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Button::setSize(int x, int y) {
	setW(x);
	setH(y);
}

bool Button::handleEvent(SDL_Event* e, Board& b) {
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
						if (butt == 25)
							Mix_PlayChannel(-1, mTSound, 0);
						else
							Mix_PlayChannel(-1, mFSound, 0);
					}
					//Restart button
					if (butt == 0) {
						b.restart();
						return true;
					}
					//Undo button
					else if (butt == 1) {
						b.undoMove();
						return true;
					}
					//White, "Human"
					else if (butt == 2)
						b.setWhiteIsBot(false);
					//White, "Computer"
					else if (butt == 3)
						b.setWhiteIsBot(true);
					//Black, "Human"
					else if (butt == 4)
						b.setBlackIsBot(false);
					//Black, "Computer"
					else if (butt == 5)
						b.setBlackIsBot(true);
					//White computer levels 1-9
					else if (butt >= 6 && butt <= 14 && b.getWhiteIsBot())
						b.whiteBot.setLevel(butt-5);
					//Black computer levels 1-9
					else if (butt >= 15 && butt <= 23 && b.getBlackIsBot())
						b.blackBot.setLevel(butt-14);
					//Flip board
					else if (butt == 24) 
						b.flipBoard();
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
