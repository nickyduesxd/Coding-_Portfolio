import heapq

#Designs a min priority queue given a heap queue - very simple - doesn't sort
class PriorityQueue:
    def __init__(self):
        self.queue = []
        self.index = 0

    # Pushes and orders the object onto the queue
    def push(self, item, priority):
        heapq.heappush(self.queue, (priority, self.index, item)) #Automatically heapifies when pushed
        self.index += 1

    # Returns lowest priority from the heap.
    def pop(self): 
        return heapq.heappop(self.queue)[-1]

    def empty(self):  # Checks if empty
        if len(self.queue) == 0:
            return true
        else:
            return false