#ifndef MOVE_H
#define MOVE_H

void movePiece(int mF = moveFrom, int mT = moveTo);	//Moves piece on mF to mT
void unmovePiece(int mF = moveFrom, int mT = moveTo);	//Unmoves piece on mT to mF
void moveInfo();					//Outputs in form of "White moved Pawn from a2 to b3( and captured a Knight)"
void changeTurns();				

void generateCompleteMoveLists();		//Calls below function. Puts all of the piece's moves into global moveLists
void generateMoveListFor(int p);		//Generates moveList for p
void generateHozMoves(int p, int& counter);
void generateDiagMoves(int p, int& counter);
void generateKnightMoves(int p, int& counter);
void generateKingMoves(int p, int& counter);
void generatePawnMoves(int p, int& counter);

extern int pieceMoved;
extern int pieceMovedFrom;
extern int prevOnMoveTo;

#endif
