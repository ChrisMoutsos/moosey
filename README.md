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
- ~~Killer heuristic (saving 2 moves/ply)~~ (disabled for now)
- Futility pruning ("Delta" pruning in quiescent search)
- Extended futility pruning
- Razoring
- Search extensions (evading check, singular reply, two replies, threat)
- Zobrist hashing
- Draw detection
- Late move reductions
- Transposition tables
- Bitboards for move generation

####In the Future
- Better move ordering
- Static exchange evaluation (attempted, gave up for now)
- More search extensions

![Title screenshot](https://raw.githubusercontent.com/ChrisMoutsos/moosey/master/res/titlescreenshot.png)
![Screenshot](https://raw.githubusercontent.com/ChrisMoutsos/moosey/master/res/screenshot.png)
![Checkmate screenshot](https://raw.githubusercontent.com/ChrisMoutsos/moosey/master/res/checkmatescreenshot.png)
