/*
----------------------------------
	~Moosey Chess Engine~
	      search.cpp
----------------------------------
*/

#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>
#include "common.h"
#include "search.h"
#include "board.h"
#include "display.h"

int nodes, qNodes, r = 0;
LINE prinVarLine, oldPrinVarLine;
double totalTimeW = 0, totalTimeB = 0;
SDL_Event e; //Event handler
int killers[30][2];

int think(Board& b, int depth) {
	using std::vector;

	auto beginTime1 = std::chrono::high_resolution_clock::now();
	typedef std::chrono::duration<float> fsec;

	int bestMoveSoFar = 0, bestScore = 0;
	int alpha = -99999, beta = 99999;
	int asp = 100;
	vector<int> moveList;

	//Reset everything if you restart the game
	if (b.getPly() == 0) {
		totalTimeW = 0;
		totalTimeB = 0;
		for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++) {
				b.hh[BLACK][f][t] = 0;
				b.hh[WHITE][f][t] = 0;
			}
	}
	//Clear prinVar, oldPrinVar
	for (int i = 0; i < 30; i++) {
		prinVarLine.move[i] = 0;
		oldPrinVarLine.move[i] = 0;
	} 

	b.genOrderedMoveList(b.getSide(), moveList);
	b.checkCheck(b.getSide(), moveList);
	
	int i;
	i = depth%2==1 ? 1 : 2;
	for (i = i; i <= depth; i += 2) {
		//Age HH tables
		for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				b.hh[b.getSide()][f][t] /= 2;
		//Clear killer moves
		for (int i = 0; i < 30; i++) {
			killers[i][0] = 0;
			killers[i][1] = 0;
		}

		oldPrinVarLine = prinVarLine;

		auto beginTime2 = std::chrono::high_resolution_clock::now();
		nodes = 0;
		qNodes = 0;
		
		alpha = bestScore - asp;
		beta = bestScore + asp;
		for (;;) {
			bestScore = alphaBeta(b, alpha, beta, i, 0, &prinVarLine, 1);
			if (bestScore <= alpha)  {
				std::cout << "FAIL LOW DEPTH " << i << "\n";
				alpha = -99999;
			}
			else if (bestScore >= beta) { 
				std::cout << "FAIL HIGH DEPTH " << i << "\n";
				beta = 99999;
			}
			else break;
		}

		bestMoveSoFar = prinVarLine.move[0];

		if (i < depth) {
			std::cout << "Search to ply " << i << "... move: ";
			std::cout << intToSquare(bestMoveSoFar/100) << " to " << intToSquare(bestMoveSoFar%100) << '\n';
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
			std::cout << "Best score: " << bestScore << "\n";
			std::cout << "Current score: " << b.eval() << "\n\n";
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

int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone, LINE* pline, bool allowNull) {
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			exit(0);
	}

	using std::vector;

	LINE line;
	bool s = b.getSide();

	//If our king is dead, return bad score
	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive())) {
		pline->count = 0;
		return -9999 + depthGone-1;
	}

	//Horizon nodes, quiescence search
	if (depthLeft <= 0) {
		//If we're in check, search a little further
		if (allowNull) { //If allowNull is false, we already checked if we were in check
			if (b.inCheck(s))
				return alphaBeta(b, alpha, beta, 1, depthGone, pline, 0); 
		}
		//Otherwise, do a quiescence search
		pline->count = 0;
		return quies(b, alpha, beta, depthGone, pline);
	}

	
	if (b.inCheck(s)) {
		if (s) b.setSideInCheck(1);
		else b.setSideInCheck(2);
	}
	else b.setSideInCheck(0);

	int score;

	//Null move reduction
	if (allowNull && !((s && b.getSideInCheck() == 1) || (!s && b.getSideInCheck() == 2))) {
		if ((s && b.getWhiteMaterial() > ENDGAME_VAL) || (!s && b.getBlackMaterial() > ENDGAME_VAL)) {
			r = depthLeft > 6 ? 4 : 3;
			b.changeTurn();
			score = -alphaBeta(b, -beta, -beta+1, depthLeft-r-1, depthGone, pline, 0);
			b.changeTurn();
			if (score >= beta) { //Fail-high
				depthLeft -= 4;
				if (depthLeft <= 0) {
					pline->count = 0;
					return quies(b, alpha, beta, depthGone, pline);
				}
			}
		}
	}

	bool foundPV = false;
	int mF, mT;
	vector<int> moveList;

	b.genOrderedMoveList(s, moveList);

	//If we are in checkmate
	if ((s && b.getSideInCheck() == 1) || (!s && b.getSideInCheck() == 2)) {
		b.cleanMoveList(s, moveList);
		if ((int)moveList.size() == 0) {
			pline->count = 0;
			return -9999 + depthGone-1;
		}
	}

	//Frontier nodes, futility pruning
	if (depthLeft == 1 && !(abs(alpha) > 9000 || abs(beta) > 9000)) {
		if (!((s && b.getSideInCheck() == 1) || (!s && b.getSideInCheck() == 2))) {
			if (b.eval() + B_VAL < alpha) {
				pline->count = 0;
				return quies(b, alpha, beta, depthGone, pline);
			}
		}
	}

	//Put principal variation and killer moves first
	int temp;
	if (allowNull) { //Except if we already null-moved, or checking a check
		//Killer moves
		int killerMove;
		for (int i = 1; i >= 0; i--) {
			if (depthGone == 0) continue;
			killerMove = killers[depthGone-1][i];
			if (killerMove == 0) continue;
			std::vector<int>::iterator kIndex;
			kIndex = std::find(moveList.begin(), moveList.end(), killerMove);
			if (kIndex != moveList.end()) {
				moveList.erase(kIndex);
				moveList.insert(moveList.begin()+0, killerMove);
			}
		}
		//Then put PV first
		std::vector<int>::iterator pvIndex;
		int pvmove = oldPrinVarLine.move[depthGone];
		if (pvmove != 0 && depthGone < oldPrinVarLine.count) {
			pvIndex = std::find(moveList.begin(), moveList.end(), pvmove);
			if (pvIndex != moveList.end()) {
				temp = *pvIndex;
				moveList.erase(pvIndex);
				moveList.insert(moveList.begin()+0, temp);
			}
		}
	}
	for (int i = 0; i < (int)moveList.size(); i++) {
		vector<int> localPV;
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		b.changeTurn();
		//If we had an alpha cutoff, do a zero-window search (guessing we were right)
		if (foundPV) {
			score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-1, depthGone+1, &line, 1);
			if ((score > alpha) && (score < beta)) //If we were wrong
				score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1);
		}	
		else
			score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1);

		b.unmovePiece();
		b.changeTurn();

		if (score >= beta) {
			//If it wasn't a capture, update HH table and killer moves
			if (b.getBoard120(mT) == empty) {
				b.hh[s][to64(mF)-1][to64(mT)-1] += depthGone*depthGone;
				killers[depthGone][1] = killers[depthGone][0];
				killers[depthGone][0] = mF*100+mT;
			}
			return beta;
		}
		if (score > alpha) {
			alpha = score;
			foundPV = true;
			//Add it to the principal variation
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
	bool s = b.getSide();

	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive()))
		return -9999 + depthGone-1;

	int score = b.eval();
	int mF, mT;
	
	if (score >= beta)
		return beta;
	if (score > alpha)
		alpha = score;

	b.generatePieceMoveLists(s);
	b.getCaptures(s, captureList);

	if ((int)captureList.size() == 0)
		return score;
	
	b.sortCaptures(captureList);

	for (int i = 0; i < (int)captureList.size(); i++) {
		qNodes++;

		mF = captureList[i]/100;
		mT = captureList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		if (b.inCheck(s)) {
			b.unmovePiece();
			continue;
		}
		b.changeTurn();
		score = -quies(b, -beta, -alpha, depthGone+1, pline);
		b.unmovePiece();
		b.changeTurn();
		
		if (score >= beta)
			return beta;
		if (score > alpha) {
			alpha = score;
		}
	}
	
	return alpha;
}
