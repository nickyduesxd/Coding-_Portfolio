import random

class Board(object):
    # A board is just a 2-d list, plus a location of the blank, fo easier move generation.
    def __init__(self):
        self.b = [[0, 1, 2, 3], [4, 5, 6, 7], [8, 9, 10, 11], [12, 13, 14, 15]] # 0 represents blank - helped solve some issues while running the code
        self.lb = [0, 0] # Location of the blank
    
    #Returns a list of places the blank can be moved to.  Note the use of map and filter.  Good tools for AI
    #programming
    def generateMoves(self):
        delta = [[-1,0],[1,0],[0,-1],[0,1]]
        result = list(map(lambda x: pairAdd(x,self.lb), delta)) 
        result = list(filter(lambda x: inRange(x), result))
        return result

    #Takes a move location, and actually changes the board.
    def makeMove(self,m):
        # It had better be next to the current location.
        if (manhattan(m,self.lb) > 1):
            raise RuntimeError('Bad move executed on board: ' + str(m) + 'lb: ' + str(self.lb))
        self.b[self.lb[0]][self.lb[1]] = self.b[m[0]][m[1]]
        self.b[m[0]][m[1]] = 0
        self.lb = m

    #Mix up the board. 
    def scramble(self,n,s=2018):
        random.seed(s)
        for i in range(n):
            moves = self.generateMoves()
            self.makeMove(moves[random.randint(0,len(moves)-1)])

    #are boards equal?
    def __eq__(self,other):
        return self.b == other.b
    def __ne__(self,other):
        return self.b != other.b
    def key(self):
        return str(self.b)
#---------------------------------
#End of Board class


#apply a list of moves to the board.
def applyMoves(board,moveList):
    #print(moveList)
    for m in moveList:
        board.makeMove(m)


#Some utility functions 
def pairAdd(a,b):
    return [a[0]+b[0],a[1]+b[1]]

def inRange(p):
    return p[0] >= 0 and p[0] < 4 and p[1] >=0 and p[1] < 4

#The heuristics go here

# This is not the actual manhattan distance heuristic, but may
# be helpful
def manhattan(a,b):
    #takes two locations on the board and returns the difference
    return abs(a[0]-b[0])+abs(a[1]-b[1])

def manhattanDistance(board):
    d = 0
    b1 = Board()
    for i in range(4):
        for j in range(4):
            for m in range(4):
                for n in range(4):
                    if board.b[i][j]!= 0 and board.b[i][j] == b1.b[m][n]:
                        d+= manhattan((i,j), (m,n))
                
    return d

def misplacedTiles(board):
    misplaced = 0
    b1 = Board()
    for i in range(4):
        for j in range(4):
            if board.b[i][j] != 0 and board.b[i][j] != b1.b[i][j]:
                misplaced += 1
    return misplaced