/*
----------------------------------
	~Moosey Chess Engine~
	      search.cpp
----------------------------------
*/

#include <iostream>
#include <cstring>
#include <chrono>
#include "common.h"
#include "search.h"
#include "board.h"
#include "display.h"

int nodes, qNodes;
LINE prinVarLine, oldPrinVarLine;
double totalTimeW = 0, totalTimeB = 0;

int think(Board& b, int depth) {
	using std::vector;

	auto beginTime1 = std::chrono::high_resolution_clock::now();
	typedef std::chrono::duration<float> fsec;

	int bestScore = 0;
	int alpha = -99999, beta = 99999;
	int asp = 100;
	vector<int> moveList;

	b.genOrderedMoveList(b.getSide(), moveList);
	b.checkCheck(b.getSide(), moveList);
	
	for (int i = 1; i <= depth; i++) {
		oldPrinVarLine = prinVarLine;

		auto beginTime2 = std::chrono::high_resolution_clock::now();
		nodes = 0;
		qNodes = 0;
		
		//bestScore = alphaBeta(b, alpha, beta, i, 0, &prinVarLine);

		alpha = bestScore - asp;
		beta = bestScore + asp;
		for (;;) {
			int times = 1;
			bestScore = alphaBeta(b, alpha, beta, i, 0, &prinVarLine);
			if (bestScore <= alpha)  {
				//if (i == depth)
				std::cout << "FAIL LOW DEPTH " << i << "\n";
				alpha = -99999;
				alpha -= times++*asp;
			}
			else if (bestScore >=beta) { 
				//if (i == depth)
				std::cout << "FAIL HIGH DEPTH " << i << "\n";
				beta += times++*asp;
			}
			else break;
		}

		if (i == depth) {
			std::cout << "Search to ply " << i << "...\n";
			std::cout << "Main nodes searched: " << nodes << '\n';
			std::cout << "Quies nodes searched: " << qNodes << '\n';
			std::cout << "Time elapsed: ";
			auto endTime1= std::chrono::high_resolution_clock::now();
			fsec diff1 = endTime1 - beginTime2;
			std::cout << diff1.count() << '\n';
			std::cout << "Nodes / sec: ";
			std::cout << (nodes+qNodes) / diff1.count() << '\n';
			std::cout << "Best score: " << bestScore << "\n\n";
		}
	}
	for (int i = 0; i < prinVarLine.count; i++) {
		std::cout << intToSquare(prinVarLine.move[i]/100) << " to ";
		std::cout << intToSquare(prinVarLine.move[i]%100) << "\n";
	}
	
	auto endTime2 = std::chrono::high_resolution_clock::now();
	fsec diff2 = endTime2 - beginTime1;
	std::cout << "Total time elapsed: " << diff2.count() << '\n';
	if (b.getSide())
		totalTimeW += diff2.count();
	else
		totalTimeB += diff2.count();

	std::cout << "Total time taken so far by ";
	if (b.getSide()) std::cout << " White: " << totalTimeW << '\n';
	else std::cout << "Black: " << totalTimeB << '\n';

	std::cout << "Best move: " << prinVarLine.move[0] << "\n\n";
	return prinVarLine.move[0];
}

int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone, LINE* pline) {
	using std::vector;

	LINE line;

	if (depthLeft == 0) {
		pline->count = 0;
		return quies(b, alpha, beta, depthGone, pline);
	}

	bool foundPV = false;
	int score, mF, mT;
	vector<int> moveList;

	b.genOrderedMoveList(b.getSide(), moveList);
	//b.checkCheck(b.getSide(), moveList);

	int temp;
	for (int i = 0; i < (int)moveList.size(); i++) {
		if (moveList[i] == oldPrinVarLine.move[depthGone]) {
			temp = moveList[i];
			moveList.erase(moveList.begin()+i);
			moveList.insert(moveList.begin()+0, temp);
		}
	}	


	for (int i = 0; i < (int)moveList.size(); i++) {
		vector<int> localPV;
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
/*		if (b.inCheck(b.getSide())) {
			b.unmovePiece();
			continue;
		}*/
		b.changeTurn();
		if (foundPV) {
			score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-1, depthGone+1, &line);
			if ((score > alpha) && (score < beta))
				score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line);
		}	
		else
			score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line);

		b.unmovePiece();
		b.changeTurn();

		if (score >= beta)
			return beta;
		if (score > alpha) {
			alpha = score;
			foundPV = true;
			pline->move[0] = mF*100 + mT;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;
		}
	}
	
	return alpha;
}

int quies(Board& b, int alpha, int beta, int depthGone, LINE* pline) {
	using std::vector;

	vector<int> captureList;

	//if (b.checkCheck(b.getSide(), captureList))
	//	return alphaBeta(b, alpha, beta, 1, depthGone+1, pline); 

	int score = b.eval();
	int mF, mT;
	
	if (score >= beta)
		return beta;
	if (score > alpha)
		alpha = score;

	b.generatePieceMoveLists(b.getSide());
	b.getGoodCaptures(b.getSide(), captureList);

	if ((int)captureList.size() == 0)
		return b.eval();
	
	for (int i = 0; i < (int)captureList.size(); i++) {
		qNodes++;
		mF = captureList[i]/100;
		mT = captureList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		if (b.inCheck(b.getSide())) {
			b.unmovePiece();
			continue;
		}
		b.changeTurn();
		score = -quies(b, -beta, -alpha, depthGone+1, pline);
		b.unmovePiece();
		b.changeTurn();
		
		if (score >= beta)
			return beta;
		if (score > alpha)
			alpha = score;
	}

	return alpha;
}
