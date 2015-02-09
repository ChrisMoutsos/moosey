/*
----------------------------------
	~Moosey Chess Engine~
	      square.h
----------------------------------
*/

#ifndef SQUARE_H
#define SQUARE_H

class LTexture;

enum PieceSprite {
	e = -1,
	wRook = 0, wKnight, wBishop, wQueen, wKing, wPawn,
	bRook, bKnight, bBishop, bQueen, bKing, bPawn
};

class Square {
	public:
		Square();
		void setPos(int x, int y);
		void handleEvent(SDL_Event* e);
		void render();
		int getPiece() const { return piece; };
		void setPiece(int p) { piece = p; };
		int getX() const { return pos.x; };
		int getY() const { return pos.y; };
	private:
		int piece;
		SDL_Point pos;
		PieceSprite currentSprite;
};

extern LTexture spriteSheetTexture;
extern SDL_Rect spriteClips[12];

#endif
