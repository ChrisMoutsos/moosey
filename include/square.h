/*
----------------------------------
	~Moosey Chess Engine~
	      square.h
----------------------------------
*/

#ifndef SQUARE_H
#define SQUARE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class LTexture;

enum PieceSprite {
	noPiece = -1,
	wRook = 0, wKnight, wBishop, wQueen, wKing, wPawn,
	bRook, bKnight, bBishop, bQueen, bKing, bPawn
};

class Square {
	public:
		Square();
		void setPos(int x, int y);
		void handleEvent(SDL_Event* e, int& mF, int& mT, const bool& s);
		void render();
		int getPiece() const { return piece; };
		void setPiece(int p) { piece = p; };
		int getX() const { return pos.x; };
		int getY() const { return pos.y; };
		void setSq(int i) { square = i; };
		bool getDragging() const { return dragging; };
		void setDragging(bool x) { dragging = x; };
	private:
		bool dragging;
		int square;
		int piece;
		SDL_Point pos;
		PieceSprite currentSprite;
};

extern LTexture spriteSheetTexture;
extern SDL_Rect spriteClips[12];

#endif
