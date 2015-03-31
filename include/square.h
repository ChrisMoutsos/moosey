/*
----------------------------------
	~Moosey Chess Engine~
	      square.h
----------------------------------
*/

#ifndef SQUARE_H
#define SQUARE_H

#include <SDL2/SDL.h>

class LTexture;

enum PieceSprite {
	noPiece = -1,
	wRook = 0, wKnight, wBishop, wQueen, wKing, wPawn,
	bRook, bKnight, bBishop, bQueen, bKing, bPawn
};

class Square {
	public:
		Square();

		void handleEvent(SDL_Event* e, int& mF, int& mT, const bool& s);

		//ACCESSORS
		int getX() const { return pos.x; };
		int getY() const { return pos.y; };
		bool getDragging() const { return dragging; };
		int getPiece() const { return piece; };
		//MUTATORS
		void setPos(int x, int y);
		void setPiece(int p) { piece = p; };
		void setSq(int i) { square = i; };
		void setDragging(bool x) { dragging = x; };
	private:
		bool dragging;
		int square, piece;
		SDL_Point pos;
};

#endif
