/*
----------------------------------
	~Moosey Chess Engine~
	      search.cpp
----------------------------------
*/

#include "search.h"
#include "board.h"
#include <vector>
#include <iostream>
#include "string"

int search(Board& b) {
	std::cout << "\n\n\nNEW SEARCH!\n\n";
	using std::vector;
	using std::string;

	int bestMove, bestScore = -99999, score;
	int alpha = -99999, beta = 99999;
	vector<int> moves;

	if (b.getSide())  {
		moves = b.whiteMoveList;
		std::cout << "moves = b.whiteMoveList, size: " << moves.size() << "\n";
	}
	else {
		moves = b.blackMoveList;
		std::cout << "moves = b.blackMoveList, size: " << moves.size() << "\n";
	}

	for (int i = 0; i < (int)moves.size(); i++) {
		std::cout << "i: " << i << ", testing move: " << intToSquare(moves[i]/100) << " to " << intToSquare(moves[i]%100) << " side: " << b.getSide() << '\n';
		alpha = -99999;
		beta = -alpha;

		b.setMove(moves[i]/100, moves[i]%100);
		b.movePiece();
		b.changeTurn();
		b.generateMoveLists();
		b.checkCheck(b.getSide());

		score = -alphaBeta(b, alpha, beta, 1);
		std::cout << "Score: " << score << " for " << b.getSide() << '\n';
		if (score > bestScore) {
			bestMove = moves[i];	
			bestScore = score;
			std::cout << "NEW BEST SCORE\n";
		}
		std::cout << "Best score is : " << bestScore << '\n';
		b.unmovePiece();
		b.changeTurn();
	}

	return bestMove;
}

int alphaBeta(Board& b, int alpha, int beta, int depthLeft) {
	using std::vector;

	int score;
	vector<int> moves;
	
	for (int i = 0; i <= depthLeft; i++) {
		std::cout << '\t';
	}
	std::cout << "In alphaBeta, side: " << b.getSide();
	std::cout << " and depthLeft: " << depthLeft << '\n';

	b.generateMoveLists();
	b.checkCheck(b.getSide());
	if (b.getSide())
		moves = b.whiteMoveList;
	else
		moves = b.blackMoveList;

	if (depthLeft == 0) {
		for (int i = 0; i <= depthLeft; i++) {
			std::cout << '\t';
		}
		std::cout << "score: " << b.eval() << " for " << b.getSide() << "\n";
		score = b.eval();	
		return score;
	}

	for (int i = 0; i < (int)moves.size(); i++) {

		b.setMove(moves[i]/100, moves[i]%100);

		for (int i = 0; i <= depthLeft; i++) {
			std::cout << '\t';
		}
		std::cout << "i: " << i << ", testing move: " << intToSquare(moves[i]/100) << " to " << intToSquare(moves[i]%100) << " side: " << b.getSide() << '\n';
		b.movePiece();
		b.changeTurn();
		b.generateMoveLists();
		b.checkCheck(b.getSide());

		score = -alphaBeta(b, -beta, -alpha, depthLeft - 1);

		b.unmovePiece();
		b.changeTurn();

		if (score >= beta) {
			return beta; //Fail hard beta-cutoff
		}
		if (score > alpha) {
			alpha = score; //Alpha acts like max in MiniMax
		}
	}
	
	return alpha;
}
