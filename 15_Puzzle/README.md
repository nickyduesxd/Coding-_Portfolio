The 15-Puzzle is the obvious extension of the 8-Puzzle to a 4 by 4 grid. Here is the wikipedia article. A good reference paper is here. Your task is to write a python program to enable a player to solve the the 15-puzzle using A*. I have started you off with an implementation of the Board class here. Your primary A* function will be called aStar, and it will take a board and a heuristic function as argument and return a tuple of the moves to solve it, and the number of nodes expanded, as in: [[2,2],[1,2],[0,2],[0,1],[0,0]]. and 154. I've already written part of the manhattan distance heuristic, you should also write the number of mis-placed tiles heuristic. You must name the manhattan distance heuristic manhattanDistance and your misplaced tiles heuristic misplacedTiles.

When developing the code, use can use any main you like, but the submission system will test you code with this code here.

Submited files should be named:

astar.py
idastar.py
Board.py
PriQue.py (if you did not use one of the built in versions)
For the language, you must use python 3.
Some thoughts:

You need to make copies of the board when you generate the children of a state. copy.deepcopy(x) is handy for that.
You will need a priority queue. You could use the built-in python version, but:
You need to be able to find things already in the priority queue and replace them.
Remember that heaps are generally faster, but if you are doing many more inserts than removeMins, an unsorted list is actually faster. Are you doing many more inserts than removeMins?
How are lists stored in python, linked lists or arrays?