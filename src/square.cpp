/*
----------------------------------
	~Moosey Chess Engine~
	      square.cpp
----------------------------------
*/

#include "iostream"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "display.h"
#include "square.h"
#include "ltexture.h"

Square::Square() {
	pos.x = 0;
	pos.y = 0;
	currentSprite = e;
}

void Square::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Square::handleEvent(SDL_Event* e, int& mF, int& mT) {
	if (e->type == SDL_MOUSEBUTTONDOWN) {
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
		if (!inside) {
		}
		else {
			switch (e->type) {
				case SDL_MOUSEBUTTONDOWN:
					//set moveTo or moveFrom
					std::cout << "CLICKING " << intToSquare(from64(square)) << '\n';
					if (mF == -1) {
						mF = square; 
					}
					else if (mT == -1) {
						mT = square;
					}
				break;
			}
		}
	}
}

void Square::render() {
	spriteSheetTexture.render(pos.x, pos.y, &spriteClips[currentSprite]);	
}
