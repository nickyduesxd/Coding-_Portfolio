#Overview

We have a list of Midshipmen along with their company numbers, listed according to their current rooms in Bancroft Hall.

Unfortunately, due to the companies expanding from 30 to 36, the companies are currently spread out over all the rooms. To fix that, your program needs to say which Mids should swap rooms (in order) so that all the companies are together.

The swaps produced by your program must put all the Mids in the same company together in the final resulting list. However, it doesn't matter what order the companies are in, or what order the Mids are within each company area.

The goal is to accomplish this goal with the fewest number of swaps.

#Requirements

The input to your program will come in the standard input stream (piped in). It will consist of n lines, and each line will have a company number, a single space, and then a name.

(Note, the input comes from standard input; it is not a filename given on the command line.)

You may assume that company numbers will always be between 1 and 100, and names will always consist of letters (no spaces) and have length at most 40. The number of names n will be at most 1 million.

Despite the bounds just given, you should think of your target/typical input as having 36 companies and 4000 names. Other, larger, input sizes will be tested but you should focus on good performance on this input size.

Your program must run within 5 seconds on submit, which has the same specs as the machines in the Linux labs of Hopper Hall. If it takes longer than this, you might not get credit for that example.

Your program should write its output to the standard output stream. Each line of the output should contain two names (with space in between), indicating who should be swapped, in order.

There will be many possible correct outputs for the same input. There is not just one right answer, but there are some answers that are better than others!

#Notes

There was a competition in class for most efficient algorithm. The first 2 were a greedy approach, this code was the 3rd.
