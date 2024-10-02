import main
import json
class World:
    def __init__(self, inputName):
    
        with open(inputName, 'r') as filehandle:
            input = json.load(filehandle)
        self.shape = input["shape"]
        self.gamma = input['gamma']
        self.values = {}
        self.policy = {}
        for x in range(0, self.shape[0]):
            for y in range(0, self.shape[1]):
                self.values[(x,y)] = 0.0
                self.policy[(x,y)] = 'N'
        self.rl= input['rl']
        self.tl= input['tl']

        for (x, y) in self.tl:
            self.policy[(x, y)] = "."
        
        self.bl= input['bl']

        for (x, y) in self.bl:
            self.policy[(x, y)] = "."

    
    def get_neighbors(self, location):
        neighbors = {} #[]
        x,y = location
        for action in ['N', 'S', 'E', 'W']:
            new_location = location
            if action == 'N':
                new_location = (x-1, y)
            if action == 'S':
                new_location = (x+1, y)
            if action == 'E':
                new_location = (x, y+1)
            if action == 'W':
                new_location = (x, y-1)

            if 0 <= new_location[0] < self.shape[0] and 0 <= new_location[1] < self.shape[1]:
                a, b = new_location
                if [a,b] in self.bl:
                    neighbors[action] = location
                else:
                    neighbors[action] = new_location
            else:
                neighbors[action] = location
        return neighbors
        

    def valueIteration(self, iterations):
        
        for _ in range(iterations):
            new_values = self.values.copy()

            for x in range(self.shape[0]):
                for y in range(self.shape[1]):
                    location = (x, y)
                    if [x,y] in self.tl:
                        new_values[location] = 0.0
                        continue
                    if [x,y] in self.bl:
                        new_values[location] = 0.0
                        continue                


                    max_value = float('-inf')
                    chosen_action = None
                    neighbors = self.get_neighbors(location)
                    
                    r = {}
                    for action in ['N', 'S', 'E', 'W']:
                        x,y = neighbors[action]
                        if [x,y] in self.tl:
                            
                            for j in self.rl:
                                if j[0] == [x,y]:
                                    r[action] = j[1]
                                    
                        else:
                            r[action] = 0

                    for action in ['N', 'S', 'E', 'W']:

                        tmp_value = 0
                    
                        if action == 'N':
                            tmp_value = .8*(r[action] + self.gamma*self.values[neighbors['N']]) + .1*(r['W'] + self.gamma*self.values[neighbors['W']]) + .1*(r['E'] + self.gamma*self.values[neighbors['E']])
                        if action == 'S':
                            tmp_value = .8*(r[action] + self.gamma*self.values[neighbors['S']]) + .1*(r['W'] + self.gamma*self.values[neighbors['W']]) + .1*(r['E'] + self.gamma*self.values[neighbors['E']])
                            
                        if action == 'E':
                            tmp_value = .8*(r[action] + self.gamma*self.values[neighbors['E']]) + .1*(r['N'] + self.gamma*self.values[neighbors['N']]) + .1*(r['S'] + self.gamma*self.values[neighbors['S']])
                        if action == 'W':
                            tmp_value = .8*(r[action] + self.gamma*self.values[neighbors['W']]) + .1*(r['N'] + self.gamma*self.values[neighbors['N']]) + .1*(r['S'] + self.gamma*self.values[neighbors['S']])

                        if tmp_value > max_value:
                            max_value = tmp_value
                            chosen_action = action
                        
                        new_values[location] = max_value
                        
                        if self.policy[location] != ".":
                            self.policy[location] = chosen_action
                        
            self.values = new_values
        
        #print(self.policy)
        switch = []
        switch2 = []
        for x in range(0,self.shape[0]):
            line = ""
            line2 = []
            for y in range(0,self.shape[1]):
                line += str(self.policy[(x,y)])
                line2.append(float(self.values[(x,y)]))
            switch.append(line)
            switch2.append(line2)
        #switch = []
        #for x in self.policy:
        #    switch.append([x, self.policy[x]])
        #print(switch)
        #self.policy = [("NENE"), ("HIFDS")]
        self.policy = switch
        self.values = switch2

