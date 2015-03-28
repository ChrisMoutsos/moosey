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
#include <vector>
#include "square.h"
#include "piece.h"
#include "search.h"

void showMoveLists(Board& b);

//For moveInfo vector
struct info {
	int pmSq;
	int epSq;
	int pieceMoved;
	int prevOnMoveTo;
	int halfMoveClock;
	std::string FEN;
};

class Board {
	
	friend int search(Board& b);
	friend int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int ext);
	
	public:
		//BOARD.CPP
		Board();
		Board(std::string FEN);
		void initializeVars();
		void emptyBoard();
		void botMove();
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
		int getWhiteMaterial() const { return whiteMaterial; };
		int getBlackMaterial() const { return blackMaterial; };
		int getFromMoveList(bool s, int i) const;
		int getMoveListSize(bool s) const;
		int getEpSq(int i) const;
		int getPmSq(int i) const;
		int getMoveMade(int i) const;
		int getPrevOnMoveTo(int i) const;
		int getPieceMoved(int i) const;
		std::string getFEN();
		//MUTATORSS
		void setMove(int mF, int mT) { moveFrom = mF; moveTo = mT; };
		void setPly(int newPly) { ply = newPly; };
		void setSide(bool newSide) { side = newSide; };
		void setCastling(int c) { castling = c; };
		void setSideInCheck(int i) { sideInCheck = i; };
		void setSideInCheckmate(int i) { sideInCheckmate = i; };
		void addToMovelist(bool s, int v);
		void clearMoveList(bool s);
		//OPERATOR OVERLOADS
		int& operator [](const int index);	       //For accessing board120
		const int& operator [](const int index) const; //For accessing board120
	
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
		bool checkCheck(bool s);
		bool checkCheck(bool s, std::vector<int>& moveList);
		bool inCheck(bool s) const;
		bool canCastle(int dir, bool s);
		bool drawCheck(bool bot = false);
	
		//MOVEGEN.CPP
		void genOrderedMoveList();
		void genOrderedMoveList(bool s, std::vector<int>& moveList);
		void getCaptures(bool s, std::vector<int>& moveList);
		void sortCaptures(std::vector<int>& moveList);
		bool MVVLVA(int i, int j);
		void sortNonCaptures(std::vector<int>& moveList);
		bool hhSort(bool s, int i, int j);
		void getNonCaptures(bool s, std::vector<int>& moveList);
		void removeNonCaptures(bool s, std::vector<int>& moveList);
		void cleanMoveList(bool s);
		void cleanMoveList(bool s, std::vector<int>& moveList);
		void generatePieceMoveLists(bool s);
		void generatePieceMoveListFor(int p);
		void generateHozMoves(int p, int& counter);
		void generateDiagMoves(int p, int& counter);
		void generateKnightMoves(int p, int& counter);
		void generateKingMoves(int p, int& counter);
		void generatePawnMoves(int p, int& counter);

		//EVAL.CPP
		int eval();
	
		//DATA
		Square squares[64];	//For the display

		Piece piece[32];	//The pieces

		int hh[2][64][64];	//History heuristic tables
	private:
		int board120[120], moveFrom, moveTo, ply;
		int whiteMaterial, blackMaterial;
		//For the line below.. 0: none, 1: white, 2: black 
		int sideInCheck, sideInCheckmate;
		int castling;
		bool side, whiteCastled, blackCastled;
		std::vector<int> whiteMoveList, blackMoveList;
		std::vector<int> movesMade; 
		std::vector<info> moveInfo;
};

#endif
