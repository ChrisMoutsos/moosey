/*
----------------------------------
	~Moosey Chess Engine~
	      square.cpp
----------------------------------
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "display.h"
#include "square.h"
#include "ltexture.h"

Square::Square() {
	square = 0;
	piece = 0;
	pos.x = 0;
	pos.y = 0;
	dragging = false;
	currentSprite = noPiece;
}

void Square::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Square::handleEvent(SDL_Event* e, int& mF, int& mT, const bool& s) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < pos.x) 
			inside = false;
		else if (x > pos.x + SQ_SIZE)
			inside = false;
		else if (y < pos.y)
			inside = false;
		else if (y > pos.y + SQ_SIZE)
			inside = false;
		switch (e->type) {
			case SDL_MOUSEBUTTONDOWN:
				if (!inside) break;
				if (mF == -1) {
					if (piece != noPiece) {
						if ((s && piece <= 15) || (!s && piece >= 16)) {
							mF = square; 
							dragging = true;
						}
					}
				}
				else if (mF != -1) {	
					if ((s && piece <= 15) || (!s && piece >= 16)) {
						if (piece != noPiece) {
							mF = square;
							dragging = true;
						}	
					}
					else
						mT = (square != mF) ? square : -1;
				}
			break;
			case SDL_MOUSEBUTTONUP:
			if (inside) {
				if (mF != -1 && mT == -1) {
					if (square != mF) {
						mT = square;
					}
					else if (!dragging) {
						mF = -1;
						mT = -1;
					}
				}
			}
			if (dragging) dragging = false;

			break;
		}
}

void Square::render() {
	spriteSheetTexture.render(pos.x, pos.y, &spriteClips[currentSprite]);	
}
