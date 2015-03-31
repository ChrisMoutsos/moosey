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
#include "bot.h"

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
		void placePiece(unsigned int p, unsigned int sq);
		void placePiecesDefault();	
		void placePieces(std::string FEN);	
		void initializePieces();
		void handleInput(int& mF, int& mT, SDL_Event* e);
		void setSquarePositions();
		void setPiecesOnSquares();
		void changeTurn();
		void undoMove();
		void restart();
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
		int getLastMove() const;
		int getNumMovesMade() const;
		int getPrevOnMoveTo(int i) const;
		int getPieceMoved(int i) const;
		std::string getFEN();
		bool getWhiteIsBot() const { return whiteIsBot; };
		bool getBlackIsBot() const { return blackIsBot; };
		bool getFlipped() const { return flipped; };
		//MUTATORSS
		void setMove(int mF, int mT) { moveFrom = mF; moveTo = mT; };
		void setPly(int newPly) { ply = newPly; };
		void setSide(bool newSide) { side = newSide; };
		void setCastling(int c) { castling = c; };
		void setSideInCheck(int i) { sideInCheck = i; };
		void setSideInCheckmate(int i) { sideInCheckmate = i; };
		void addToMovelist(bool s, int v);
		void clearMoveList(bool s);
		void setWhiteIsBot(bool b) { whiteIsBot = b; };
		void setBlackIsBot(bool b) { blackIsBot = b; };
		void flipBoard() { flipped = flipped ? 0 : 1; };
		//OPERATOR OVERLOADS
		int& operator [](const int index);
		const int& operator [](const int index) const;
	
		//MOVE.CPP 
		void movePiece();
		void movePiece(int mF, int mT);
		void unmovePiece();
		void unmovePiece(int mF, int mT);
	
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
		void addPromotions(bool s, std::vector<int>& moveList);
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

		Bot whiteBot, blackBot; //The bots
	private:
		int board120[120], moveFrom, moveTo, ply;
		int whiteMaterial, blackMaterial, castling;
		//For the line below.. 0: none, 1: white, 2: black 
		int sideInCheck, sideInCheckmate;
		bool side, whiteCastled, blackCastled;
		bool whiteIsBot, blackIsBot, flipped;
		int whiteBotLevel, blackBotLevel;
		std::vector<int> whiteMoveList, blackMoveList;
		std::vector<int> movesMade; 
		std::vector<info> moveInfo;
};

#endif
