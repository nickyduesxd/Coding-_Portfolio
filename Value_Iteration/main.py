from world import *


def printWorldValues(world):
    #loop across all locations
    for row in world.values: 
        line = ''
        for e in row:
            line += '%3.3f ' % e #build a string of each line
        print(line) #print string

def printWorldPolicy(world):
    #print("rah")
    for row in world.policy: 
        line = ''
        #print(row)
        for e in row:
            #print(e)
            line += e + ' '
        print(line)
  

if __name__ == "__main__":
    worldName = input('World file name: ')
    i = int(input('Number of iterations: '))
    w = World(worldName)
    
    w.valueIteration(i)
    printWorldValues(w)
    printWorldPolicy(w)
