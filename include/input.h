/*
----------------------------------
	~Moosey Chess Engine~
	      input.h
----------------------------------
*/

#ifndef INPUT_H
#define INPUT_H

void userInput(int& moveFrom, int& moveTo, bool side); //Ensures move is legal and valid, tells player
		  // if either of those is false or if they're in check
bool getInput(int& moveFrom, int& moveTo, bool side); //Validates the syntax of input

#endif
