#ifndef BOARD_H
#define BOARD_h

#include <string>
#include <vector>

class Board {

public:
	//BOARD.CPP
	Board();
	Board(std::string FEN);
	void initializeVars();
	void emptyBoard();
	void placePiece(int, int);
	void placePiecesDefault();	
	void initializePieces();
	//ACCESSORS
	int getMoveFrom() const { return moveFrom; };
	int getMoveTo() const { return moveTo; };
	int getPly() const { return ply; };
	bool getSide() const { return side; };
	int getPieceMoved() const { return pieceMoved; };
	int getPieceMovedFrom() const { return pieceMovedFrom; };
	int getPrevOnMoveTo() const { return prevOnMoveTo; };	
	int getBoard120(int x) const { return board120[x]; };
	int getPieceAbbr(int x) const { return piece[x].abbr; };
	int getFromMovelist(bool, int) const;
	int getTimesMoved(int x) const { return piece[x].moved; };
	std::string getName(int x) const { return piece[x].name; };
	//MUTATORSS
	void setMove(int, int);
	void setPly(int);
	void setSide(bool);
	void setPieceMoved(int);
	void setPieceMovedFrom(int);
	void setPrevOnMoveTo(int);
	void setBoard120(int, int);
	void addToMovelist(bool, int);	
	void clearMoveList(bool);
	void killPiece(int);
	void unkillPiece(int);
	void setPiecePos(int, int);
	void incrMoved(int);
	void decrMoved(int);

	//MOVE.CPP 
	void movePiece();
	void movePiece(int, int);
	void unmovePiece();
	void unmovePiece(int, int);
	void changeTurn();
	void moveInfo() const;

	//LEGAL.CPP
	bool legalMove(int, int, bool, bool verbose = false);
	bool validateMove(int, int) const;
	bool validatePawnMove(int, int) const;
	bool validateHozMove(int, int) const;
	bool validateDiagMove(int, int) const;
	bool validateKnightMove(int, int) const;
	bool validateKingMove(int, int) const;
	bool checkStalemate() const;
	bool checkCheck(bool, bool v = false);
	bool inCheckmate(bool) const;
	bool inCheck(bool);

	//MOVEGEN.CPP
	void generateMoveLists();
	void cleanMoveList(bool);
	void generateMoveListFor(int);
	void generateHozMoves(int, int&);
	void generateDiagMoves(int, int&);
	void generateKnightMoves(int, int&);
	void generateKingMoves(int, int&);
	void generatePawnMoves(int, int&);

	//DATA	
	struct pieceEntry {
        	std::string name;	//"Pawn", "Rook", etc.
        	char abbr;		//'P' (white), 'p' (black)
        	int value;      
        	int pos;        	//Board64 position
        	int moved;      	//>=0
        	bool alive;    
        	int promoted;   	//0:none, 1:B, 2:R, 3:K, 4:Q
		bool color;
        	int* moveList;  	
		int moveListSize;
	} piece[32];

	int board120[120];
	std::vector<int> whiteMoveList, blackMoveList;
	
private:
	int moveFrom, moveTo, ply;
	int pieceMoved, pieceMovedFrom, prevOnMoveTo;
	bool side;
};

//INLINE CONVERSION FUNCTIONS
inline int to64(int x) {
	if (x < 98 && x > 21 && !(x%10 == 0 || x%10 == 9))
			return x-20-2*((x-x%10)/10-2);
	return 0;
}

inline int from64(int x) {
	return x+20+(((x-1)/8))*2;
}

inline std::string intToSquare(int square) {
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

enum pieceValues_t { P_VAL = 100, N_VAL = 300, B_VAL = 310,
                     R_VAL = 500, Q_VAL = 1000, K_VAL = 9999 
};

#endif
