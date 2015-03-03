/*
----------------------------------
	~Moosey Chess Engine~
	     	board.h
----------------------------------
*/
#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "square.h"
#include "piece.h"
#include "search.h"

class Board {
	
	friend int search(Board& b);
	friend int alphaBeta(Board& b, int alpha, int beta, int depthLeft);
	
	public:
		//BOARD.CPP
		Board();
		Board(std::string FEN);
		void initializeVars();
		void emptyBoard();
		void placePiece(int p, int sq);
		void placePiecesDefault();	
		void initializePieces();
		void handleInput(int& mF, int& mT, SDL_Event* e);
		void setSquarePositions();
		void setPiecesOnSquares();
		//ACCESSORS
		int getMoveFrom() const { return moveFrom; };
		int getMoveTo() const { return moveTo; };
		int getPly() const { return ply; };
		bool getSide() const { return side; };
		int getSideInCheck() const { return sideInCheck; };
		int getSideInCheckmate() const { return sideInCheckmate; };
		int getCastling() const { return castling; };
		int getBoard120(int sq) const;
		int getFromMoveList(bool s, int i) const;
		int getMoveListSize(bool s) const;
		int getEpSq(int i) const;
		int getPmSq(int i) const;
		int getMoveMade(int i) const;
		int getPrevOnMoveTo(int i) const;
		int getPieceMoved(int i) const;
		//MUTATORSS
		void setMove(int mF, int mT) { moveFrom = mF; moveTo = mT; };
		void setPly(int newPly) { ply = newPly; };
		void setSide(bool newSide) { side = newSide; };
		void setCastling(int c) { castling = c; };
		void setSideInCheck(int i) { sideInCheck = i; };
		void setSideInCheckmate(int i) { sideInCheckmate = i; };
		void setBoard120(int i, int v);
		void addToMovelist(bool s, int v);
		void clearMoveList(bool s);
	
		//MOVE.CPP 
		void movePiece();
		void movePiece(int mF, int mT);
		void unmovePiece();
		void unmovePiece(int mF, int mT);
		void undoMove();
		void restart();
		void changeTurn();
	
		//LEGAL.CPP
		bool legalMove(int mF, int mT, bool s, bool v = false);
		bool validateMove(int mF, int mT, bool s);
		bool validatePawnMove(int mF, int mT, bool s) const;
		bool validateHozMove(int mF, int mT) const;
		bool validateDiagMove(int mF, int mT) const;
		bool validateKnightMove(int mF, int mT) const;
		bool validateKingMove(int mF, int mT, bool s);
		bool canCastle(int dir, bool s);
		bool checkStalemate() const;
		bool checkCheck(bool s);
		bool inCheckmate(bool s) const;
		bool inCheck(bool s) const;
	
		//MOVEGEN.CPP
		void generateMoveLists();
		void cleanMoveList(bool s);
		void generateMoveListFor(int p);
		void generateHozMoves(int p, int& counter);
		void generateDiagMoves(int p, int& counter);
		void generateKnightMoves(int p, int& counter);
		void generateKingMoves(int p, int& counter);
		void generatePawnMoves(int p, int& counter);

		//EVAL.CPP
		int eval();
	
		Square squares[64]; //For the display

		Piece piece[32];	//The pieces
	private:
		int board120[120], moveFrom, moveTo, ply;
		//For the line below.. 0: none, 1: white, 2: black 
		int sideInCheck, sideInCheckmate;
		int castling;
		bool side, whiteCastled, blackCastled;
		std::vector<int> whiteMoveList, blackMoveList;
		std::vector<int> movesMade, pmSq, epSq;
		std::vector<int> pieceMoved, prevOnMoveTo, pieceKilled;
};

//INLINE CONVERSION FUNCTIONS
inline int to64(int x) { //returns 1 to 64
	if (x < 99 && x > 20 && !(x%10 == 0 || x%10 == 9))
			return x-20-2*((x-x%10)/10-2);
	return 0;
}

inline int FR2SQ64(int f, int r) { //returns 1 to 64
	return (f)+((r-1)*8);
}

inline int from64(int x) { //returns 21 to 98
	return x+20+(((x-1)/8))*2;
}

inline std::string intToSquare(int square) { //Board120 to A1, etc
	std::string squareName;
	if (square==0) return "";
	squareName = char(int('a') + ((square)%10)-1);
	squareName += char((square)/10 + 49 - 2);
	return squareName;
}

//ENUMERATIONS
enum side_t { BLACK = 0, WHITE = 1 
};

enum square_t { empty = -1, null = 0, invalid = -99,
                A1 = 1, B1, C1, D1, E1, F1, G1, H1,
                A2 = 9, B2, C2, D2, E2, F2, G2, H2,
                A3 = 17, B3, C3, D3, E3, F3, G3, H3,
                A4 = 25, B4, C4, D4, E4, F4, G4, H4,
                A5 = 33, B5, C5, D5, E5, F5, G5, H5,
                A6 = 41, B6, C6, D6, E6, F6, G6, H6,
                A7 = 49, B7, C7, D7, E7, F7, G7, H7,
                A8 = 57, B8, C8, D8, E8, F8, G8, H8 
};
enum square2_t { _A1 = 21, _B1, _C1, _D1, _E1, _F1, _G1, _H1,
                 _A2 = 31, _B2, _C2, _D2, _E2, _F2, _G2, _H2,
                 _A3 = 41, _B3, _C3, _D3, _E3, _F3, _G3, _H3,
                 _A4 = 51, _B4, _C4, _D4, _E4, _F4, _G4, _H4,
                 _A5 = 61, _B5, _C5, _D5, _E5, _F5, _G5, _H5,
                 _A6 = 71, _B6, _C6, _D6, _E6, _F6, _G6, _H6,
                 _A7 = 81, _B7, _C7, _D7, _E7, _F7, _G7, _H7,
                 _A8 = 91, _B8, _C8, _D8, _E8, _F8, _G8, _H8 
};

enum dir_t { L = -1, R = 1, U = 10, D = -10, 
	     UL = 9, UR = 11, DL = -11, DR = -9, 
	     K1 = 8, K2 = -8, K3 = 12, K4 = -12, 
	     K5 = 19, K6 = -19, K7 = 21, K8 = -21
};

enum piece_t { wqR = 0, wqN, wqB, wQ, wK, wkB, wkN, wkR,
               wPa, wPb, wPc, wPd, wPe, wPf, wPg, wPh,
               bqR, bqN, bqB, bQ, bK, bkB, bkN, bkR,
               bPa, bPb, bPc, bPd, bPe, bPf, bPg, bPh 
};

enum pieceValue_t { P_VAL = 100, N_VAL = 300, B_VAL = 310,
                     R_VAL = 500, Q_VAL = 1000, K_VAL = 9999 
};

enum castleside_t { QUEENSIDE = 2, KINGSIDE = 1
};

#endif
