/*
----------------------------------
	~Moosey Chess Engine~
	      zobrist.h
----------------------------------
*/

#ifndef ZOBRIST_H
#define ZOBRIST_H

class Zobrist {
	public:
		//Initializes with random numbers
		Zobrist();

		//The final key
		unsigned long key;
		//Each piece at each square
		unsigned long piece[6][2][64];
		//Side to move
		unsigned long side;
		//White and black king/queenside castling rights
		unsigned long castling[2][2];
		//Which file the en passant square is on, if any
		unsigned long enPassant[8];
};



#endif
