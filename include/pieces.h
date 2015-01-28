/*
----------------------------------
	~Moosey Chess Engine~
	      pieces.h
----------------------------------
*/

#ifndef PIECES_H
#define PIECES_H

#include <string>


struct pieceEntry {
        std::string name;	//"Pawn", "Rook", etc.
        char abbr;		//'P' (white), 'p' (black)
        int value;      
        int pos;        	//Board64 position
        int moved;      	//Number of times moved (>=0)
        bool alive;    
        int promoted;   	//0 = none, 1 = rook, 2 = knight, 3 = bishop, 4 = queen
        int* moveList;  	//Will point at a moveList for the piece
	int moveListSize;	//Size of the array pointed at by moveList
};

enum piece_t { wqR = 0, wqN, wqB, wQ, wK, wkB, wkN, wkR,
               wPa, wPb, wPc, wPd, wPe, wPf, wPg, wPh,
               bqR, bqN, bqB, bQ, bK, bkB, bkN, bkR,
               bPa, bPb, bPc, bPd, bPe, bPf, bPg, bPh
};

enum pieceValues_t { P_VAL = 100, N_VAL = 300, B_VAL = 310,
                     R_VAL = 500, Q_VAL = 1000, K_VAL = 9999
};

extern pieceEntry piece[32];

#endif
