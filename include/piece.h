/*
----------------------------------
	~Moosey Chess Engine~
	     	piece.h
----------------------------------
*/

#ifndef PIECE_H
#define PIECE_h

#include <string>

class Piece {
	public:
		//PIECE.CPP
		Piece();
		~Piece();
		//Accessors
		int getFromMoveList(int i) const;
		char getAbbr() const { return abbr; };
		int getMoved() const { return moved; };
		std::string getName() const { return name; };
		bool getAlive() const { return alive; };
		int getMoveListSize() const { return moveListSize; };
		int getValue() const { return value; };
		int getPos() const { return pos; };
		bool getColor() const { return color; };
		//Mutators
		void setInMoveList(int i, int v);
		void kill() { alive = false; };
		void unkill() { alive = true; };
		void setPos(int newPos) { pos = newPos; };
		void incrMoved() { moved++; };
		void decrMoved() { moved--; };
		void setName(std::string newName) { name = newName; };
		void setMoveList(int * p) { moveList = p; };
		void setMoveListSize(int i) { moveListSize = i; };
		void setColor(bool newColor) { color = newColor; };
		void setAbbr(char newAbbr) { abbr = newAbbr; };
		void setValue(int newValue) { value = newValue; };
		void setPromoted(bool newPromoted) { promoted = newPromoted; };
		void freeMoveList() { delete [] moveList; };
	
	private:
		std::string name;
		char abbr;
		int value;
		int pos;
		int moved;
		bool alive;
		bool promoted;
		bool color;
		int* moveList;
		int moveListSize;	
};



#endif
