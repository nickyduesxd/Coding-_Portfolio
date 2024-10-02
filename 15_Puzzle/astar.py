from Board import *
from PriQue import PriorityQueue
import copy
import pdb

def aStar(board, heuristic):
    moves = [] #Keeps track of moves
    nodes_expanded = 0
    Open = PriorityQueue() 
    Open.push((board, moves), 0)

    closed = dict() #Keeps record
    current_board, moves = Open.pop()

    """
    BestFirst(StartState s)
  while (isNotGoal(s))
    generate successors to s
    point successors back to s
    add successors to open
    add s to closed
    s ←  best from open
    """

    while current_board.key() != Board().key():
        nodes_expanded += 1
        
        for move in current_board.generateMoves(): #generate successors
            next_board = copy.deepcopy(current_board) #Copy the current board
            next_board.makeMove(move) #make the move
            if next_board.key() not in closed: #checked if after move if we are equal to another baord already in closed so we can ignore it if we passed over it already
                new_moves = moves + [move] #Links sucessors to current_board
                priority = len(new_moves) + heuristic(next_board) #Boards with the least amount of new moves and the lowest heuristic have priority.
                Open.push((next_board, new_moves), priority) #add succsesors to open
        closed[current_board.key()] = current_board
        current_board, moves = Open.pop()

    return moves, nodes_expanded
if __name__ == "__main__":
    n = int(input("Enter scramble size: ")) 
    b1 = Board()
    b1.scramble(n)
    b2 = Board()
    p , n1  = aStar(b1, misplacedTiles)
    print(len(p))
    print(n1)
    applyMoves(b1,p)
    print(b1==b2)
    b1 = Board()
    b1.scramble(n)
    p , n2  = aStar(b1, manhattanDistance)
    print(len(p))
    print(n2)
    applyMoves(b1,p)
    print(b1==b2)
    print(n1>=n2)