/*
----------------------------------
	~Moosey Chess Engine~
	      pieces.cpp
----------------------------------
*/

#include "pieces.h"

pieceEntry piece[32] = {
	{"Rook", 'R', R_VAL, 0, 0, 1, 0},{"Knight", 'N', N_VAL, 0, 0, 1, 0}, 
	{"Bishop", 'B', B_VAL, 0, 0, 1, 0}, {"Queen", 'Q', Q_VAL, 0, 0, 1, 0}, 
	{"King", 'K', K_VAL, 0, 0, 1, 0}, {"Bishop", 'B', B_VAL, 0, 0, 1, 0},
        {"Knight", 'N', N_VAL, 0, 0, 1, 0}, {"Rook", 'R', R_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'P', P_VAL, 0, 0, 1, 0}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'P', P_VAL, 0, 0, 1, 0}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'P', P_VAL, 0, 0, 1, 0}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'P', P_VAL, 0, 0, 1, 0}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0}, 
	{"Rook", 'r', R_VAL, 0, 0, 1, 0}, {"Knight", 'n', N_VAL, 0, 0, 1, 0}, 
	{"Bishop", 'b', B_VAL, 0, 0, 1, 0}, {"Queen", 'q', Q_VAL, 0, 0, 1, 0}, 
	{"King", 'k', K_VAL, 0, 0, 1, 0}, {"Bishop", 'b', B_VAL, 0, 0, 1, 0}, 
	{"Knight", 'n', N_VAL, 0, 0, 1, 0}, {"Rook", 'r', R_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'p', P_VAL, 0, 0, 1, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'p', P_VAL, 0, 0, 1, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0},
	{"Pawn", 'p', P_VAL, 0, 0, 1, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0}, 
	{"Pawn", 'p', P_VAL, 0, 0, 1, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0}
        };
