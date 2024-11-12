Problem: Matrix Calculator
Functionality: Addition, Subtraction, Multiplication, Scalar Multiplication, Determinant, Transpose, Adjoint (up to a 2 by 2 matrix)

Ponylang is a parallel computing language where actors act concurrently. In our code, we have 2 actors, the main actor and the Node actor.
We show concurrency in the addition, subtraction, multiplication, and scalar multiplication functionality where a Matrix is split into
3 parts by rows and set to 3 different nodes to perform calculations then we print from each 3 nodes the result in order. We also
used the reference capabilities especially with passing data to the nodes, having to read the data, store it, then transfer ownership
to the Node class who can read and write to it while the main cannot do either.

We firmly believe that this program represent a significant accomplishment since we spent 3 weeks learning a new language, dipping into
concurrent programming which is a very advance topic, and defining operations for matrices which is a very complicated mathematical field.
We are both very proud of the work we have done.

We incorperated alot of error checking and usage statements.

With the submitted material these are the tests we recommend you run to show the functionality of our code.

Finding the determinant of a 1 by 1 matrix:
./Phase2 det det_1.txt
Finding the determinant of a 2 by 2 matrix:
./Phase2 det det_2.txt
Finding the determinant of a 4 by 4 matrix:
./Phase2 det det_4.txt
Finding the adjoint of a 2 by 2 matrix:
./Phase2 adj det_2.txt
Find the transpose of a 18 by 18 matrix:
./Phase2 transp matrix1.txt
Adding 2 matrices:
./Phase2 add matrix1.txt matrix2.txt
Subtracting 2 matrices:
./Phase2 sub matrix1.txt matrix2.txt
Multiplying 2 matrices:
./Phase2 mul matrix1.txt matrix2.txt
Scalar multiplying a matrix:
./Phase2 scalmul matrix1.txt 5

Matrix files follow a specific format of row col then new line followed by values.

Output is printed to the screen.


