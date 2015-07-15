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
#include "piece.h"
#include "bot.h"
#include "zobrist.h"
#include "display.h"
#include "bitboards.h"

//For moveInfo vector
struct info {
	int pmSq;
	int epSq;
	int pieceMoved;
	int prevOnMoveTo;
	int halfMoveClock;
	unsigned long zobrist;
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
		void initializeZobrist();
		void initializeBitboards();
		void handleInput(int& mF, int& mT, SDL_Event* e);
		void setSquarePositions();
		void setPiecesOnSquares();
		void changeTurn();
		void undoMove();
		void restart();
		void updateDisplay(const int& mF, const int& mT);
		void zobristXorSide() { zobrist.key ^= zobrist.side; };
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
		unsigned long getZobrist() const { return zobrist.key; };
		int getNumWhiteMoves() const { return numWhiteMoves; };
		int getNumBlackMoves() const { return numBlackMoves; };
		int getNumMoves() const { return side ? numWhiteMoves : numBlackMoves; };
		//MUTATORS
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
		bool putSelfInCheck(int mF, int mT, bool s);
		bool validateMove(int mF, int mT, bool s);
		bool validatePawnMove(int mF, int mT, bool s) const;
		bool validateHozMove(int mF, int mT) const;
		bool validateDiagMove(int mF, int mT) const;
		bool validateKnightMove(int mF, int mT) const;
		bool validateKingMove(int mF, int mT, bool s);
		bool checkCheck(bool s);
		bool checkCheck(bool s, int* moveList);
		bool inCheck(bool s) const;
		bool canCastle(int dir, bool s);
		int drawCheck() const;
	
		//MOVEGEN.CPP
		int perft(int depth);
		void clearMovelist(int* moveList);
		int getNonOrderedAllLegalMoves(bool s, int* moveList);
		int genOrderedMoveList();
		int genOrderedMoveList(bool s, int* moveList);
		int genNonOrderedMoveList(bool s, int* moveList);
		int getCaptures(bool s, int* moveList);
		void sortCaptures(int* moveList);
		bool MVVLVA(int i, int j);
		int addPromotions(bool s, int* moveList);
		void sortNonCaptures(int* moveList);
		bool hhSort(bool s, int i, int j);
		int getNonCaptures(bool s, int* moveList);
		void removeNonCaptures(bool s, int* moveList);
		int cleanMoveList(bool s);
		int cleanMoveList(bool s, int* moveList);
		void generatePieceMoveLists(bool s);
		void generatePieceMoveListFor(int p);
		void generateHozMoves(int p, int& counter);
		void generateDiagMoves(int p, int& counter);
		void generateKnightMoves(int p, int& counter);
		void generateKingMoves(int p, int& counter);
		void generatePawnMoves(int p, int& counter);

		//EVAL.CPP
		int eval(bool verbose = false);
	
		//DATA
		Piece piece[32];	//The pieces

		Bot whiteBot, blackBot; //The bots

		Display display; 	//The display
	private:
		int board120[120], moveFrom, moveTo, ply;
		int whiteMaterial, blackMaterial, castling;
		//For the line below.. 0: none, 1: white, 2: black 
		int sideInCheck, sideInCheckmate;
		bool side, whiteCastled, blackCastled;
		bool whiteIsBot, blackIsBot, flipped;
		int whiteBotLevel, blackBotLevel;
		//Zobrist hash key
		Zobrist zobrist;
		int whiteMoveList[256], blackMoveList[256];
		int numWhiteMoves, numBlackMoves;
		std::vector<int> movesMade; 
		std::vector<info> moveInfo;
		Bitboards bb;
};

#endif
