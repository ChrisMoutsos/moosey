This is a work in progress.

##Overview
Moosey is a chess engine designed to run without the help of an external GUI. It uses the SDL2 library. Complete with beautiful hand-drawn pieces!

###Internal Features
The following features have been implemented.
- Negamax search with alpha-beta pruning
- Quiescence search
- Principal variation search
- Iterated deepening, using old princ. var. as first nodes
- Aspiration window
- Sorting captures by MVVLVA (for now)
- Null move reductions
- History heuristic
- Killer heuristic (saving 2 moves/ply)
- Futility pruning

####In the Future
- Better move ordering
- Transposition tables
- Static exchange evaluation (attempted, such a pain I gave up for now)
- Search extensions

![Screenshot](https://raw.githubusercontent.com/ChrisMoutsos/moosey/master/res/screenshot.png)
