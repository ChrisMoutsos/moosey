This is a work in progress.

##Overview
Moosey is a chess engine designed to run without the help of an external GUI. It uses the SDL2 library. Complete with beautiful hand-drawn pieces!

###Features
The following features have been implemented.
- Negamax search with alpha-beta pruning
- Quiescence search
- Principal variation search
- Iterated deepening, using old princ. var. as first nodes
- Aspiration window
- Sorting captures by MVVLVA
- Null move heuristic
- History heuristic
- Killer heuristic

####In the Future
- Better move ordering
- Transposition tables
- Static exchange evaluation (attempted, such a pain I gave up for now)
- Futility pruning

![Screenshot](https://raw.githubusercontent.com/ChrisMoutsos/moosey/master/res/screenshot.png)
