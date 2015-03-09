This is a work in progress.

#Overview
Moosey is a chess engine designed to run without the help of an external GUI. It uses the SDL2 library. 

#Features
The following features have been implemented.
- Negamax search with alpha-beta pruning
- Quiescence search
- Principal variation search
- Iterated deepening, using old princ. var. as first nodes
- Aspiration window
- Null move heuristic
- Sorting captures by MVVLVA
- (Removed history heuristic for now)

![Screenshot](https://raw.githubusercontent.com/ChrisMoutsos/moosey/master/res/screenshot.png)
