/*
----------------------------------
	~Moosey Chess Engine~
	      search.cpp
----------------------------------
*/

#include "search.h"
#include "board.h"
#include <iostream>

int search(Board& b) {
	using std::vector;

	int bestMove = 0, bestScore = -99999, score;
	int mF, mT;
	int alpha = -99999, beta = 99999;
	vector<int> moveList;

//	std::cout << "\n\n\n\nNEWSEARCH, BOT SEARCHING FOR: " << b.getSide() << "\n\n";
	if (b.getSide())
		moveList = b.whiteMoveList;
	else
		moveList = b.blackMoveList;

	for (int i = 0; i < (int)moveList.size(); i++) {
		alpha = -99999;
		beta = -alpha;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
//		std::cout << "Searching ROOT " << i << " mF: " << mF << " mT: " << mT << '\n';
		b.setMove(mF, mT);
		b.movePiece();

		score = -alphaBeta(b, alpha, beta, 3);

		if (score > bestScore) {
			bestScore = score;
			bestMove = mF*100 + mT;
//			std::cout << "NEW BEST SCORE IN ROOT: " << bestScore << '\n';;
		}
//		std::cout << "ROOT " << i << " score was " << score << '\n';


		b.unmovePiece();
		b.changeTurn();

	}
	std::cout << "\nBest score : " << bestScore << " Best move : " << bestMove << '\n';
	
	if (bestMove == 0) std::cout << "CHECKMATED\n";
	return bestMove;
}

int alphaBeta(Board& b, int alpha, int beta, int depthLeft) {
	using std::vector;

	int score;
	int mF, mT;
	vector<int> moveList;

	b.changeTurn();
	if (b.getSide()) {
		b.generateMoveLists(moveList, b.blackMoveList);
	}
	else {
		b.generateMoveLists(b.whiteMoveList, moveList);
	}
	b.checkCheck(b.getSide());

	if (depthLeft == 0) {
//		std::cout << "Scoring... " << b.eval() << '\n';
		//b.changeTurn();
		return b.eval();
	}

	for (int i = 0; i < (int)moveList.size(); i++) {
		mF = moveList[i]/100;
		mT = moveList[i]%100;
		for (int i = 0; i <= depthLeft; i++) std::cout << '\t';
//		std::cout << "AB depth: " << depthLeft << " mF: " << mF << " mT: " << mT << '\n';
		b.setMove(mF, mT);
		b.movePiece();
		
		score = -alphaBeta(b, -beta, -alpha, depthLeft - 1);
		b.changeTurn();

		for (int i = 0; i <= depthLeft; i++) std::cout << '\t';
//		std::cout << "score: " << score << '\n';
		if (score >= beta) {
			b.unmovePiece();
			return beta;
		}
		if (score > alpha)
			alpha = score;

		b.unmovePiece();

	}
	
	return alpha;
}
