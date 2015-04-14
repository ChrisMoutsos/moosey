/*
----------------------------------
	~Moosey Chess Engine~
	      square.cpp
----------------------------------
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "common.h"
#include "square.h"
#include "ltexture.h"
#include "display.h"

Square::Square() {
	square = 0;
	piece = 0;
	pos.x = 0;
	pos.y = 0;
	dragging = false;
}

void Square::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Square::handleEvent(SDL_Event* e, int& mF, int& mT, const bool& s, int& sound) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < pos.x) 
			inside = false;
		else if (x > pos.x + 75)
			inside = false;
		else if (y < pos.y)
			inside = false;
		else if (y > pos.y + 75)
			inside = false;
		switch (e->type) {
			case SDL_MOUSEBUTTONDOWN:
				if (!start) break;
				if (!inside) break;
				if (mF == -1) {
					if (piece != noPiece)
						if ((s && piece <= 15) || (!s && piece >= 16)) {
							mF = square; 
							dragging = true;
							if (!muted)
								sound = 1;
						}
				}
				else if (mF != -1) {
					if ((s && piece <= 15) || (!s && piece >= 16)) {
						if (piece != noPiece) {
							mF = square; 
							dragging = true;
							if (!muted)
								sound = 1;
						}	
					}
					else if (square != mF) {
						mT = square;
						if (!muted)
							sound = 2;
					}
					else
						mT = -1;
				}
			break;
			case SDL_MOUSEBUTTONUP:
			if (!start) break;
			if (inside)
				if (mF != -1 && mT == -1) {
					if (square != mF) {
						mT = square;
						if (!muted)
							sound = 1;
					}
					else if (!dragging) {
						mF = -1;
						mT = -1;
					}
				}
			if (dragging) dragging = false;

			break;
		}
}
