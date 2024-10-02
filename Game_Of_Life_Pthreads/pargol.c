/*
Nicholas Zayfman 257050
- I used the time code Dr. Knoll sent to help time threads, originated from stackoverflow.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define TYPE char

#include <sys/time.h> /*gettimeofday()*/    

//Time struct
typedef struct {                                                          
    int     secs;                                                          
    int     usecs;                                                        
} TIME_DIFF;                                                              

//Time function                                                                  
TIME_DIFF * my_difftime (struct timeval * start, struct timeval * end)    
{                                                                          
    TIME_DIFF * diff = (TIME_DIFF *) malloc ( sizeof (TIME_DIFF) );        
                                                                           
    if (start->tv_sec == end->tv_sec) {                                    
        diff->secs = 0;                                                    
        diff->usecs = end->tv_usec - start->tv_usec;                      
    }                                                                      
    else {                                                                
        diff->usecs = 1000000 - start->tv_usec;                            
        diff->secs = end->tv_sec - (start->tv_sec + 1);                    
        diff->usecs += end->tv_usec;                                      
        if (diff->usecs >= 1000000) {                                      
            diff->usecs -= 1000000;                                        
            diff->secs += 1;                                              
        }                                                                  
    }                                                                      
                                                                           
    return diff;                                                          
}      

//Matrix struct
typedef struct Matrix{
    int row;
    int col;
    TYPE** arr;
} Matrix;

//Thread struct
typedef struct thread_info{
    int id;
    int start;
    int count;
} thread_info;

//Global variables, makes it easier so we don't have to pass alot of variables
//into functions
int numThreads; //Number of threads to use prompted by user
int verbosity; //Debugging purposes
int maxGen; //Maximum number of generations
int displayFreq; //Displays every x generations
Matrix m; //The matrix we are using to store all values
Matrix n; // The matrix we are using to flip flop

pthread_barrier_t barrier;

char** generateMatrixUserInput(int, int);
Matrix generateMatrixFileInput();
void *thread_func(void*);
void printMatrix(Matrix*, int);
void freeMatrix(TYPE** , int , int);
char** fillHalo(TYPE**, int, int);
char** runGen(char**, char**, int, int, int);

int main(int argc, char** argv){

    if(argc != 5){
        printf("./pargol <maxNumGens> <displayFreq> <numThreads> <verbosity>\n"); 
        return 0;
    }
    
    //Read arguments
    maxGen = atoi(argv[1]) + 1;
    displayFreq = atoi(argv[2]);
    numThreads = atoi(argv[3]);
    verbosity = atoi(argv[4]);   
    
    printf("num rows: ");
    scanf("%d", &m.row);
    
    if(m.row == 0){
        m = generateMatrixFileInput();
    }
    else{
        int seed;
        printf("num cols: ");
        scanf("%d", &m.col); 
        printf("seed: ");
        scanf("%d", &seed); 
        if(seed<0){srand(time(0));}
        else{srand(seed);}
        m.arr = generateMatrixUserInput(m.row+2, m.col+2);
    }
    
    //The flip flop 2D Matrix, stores changes after each generation update.
    n.row = m.row;
    n.col = m.col;

    //Initialize n matrix.
    n.arr = malloc((n.row +2)* sizeof(TYPE*));
    for(int i = 0; i < n.row+2; i++){
        n.arr[i] = malloc((n.col+2) * sizeof(TYPE));
    }
    
    struct timeval myTVstart, myTVend;      
    gettimeofday(&myTVstart, NULL);     

    //initalize pthreads
    pthread_t thread_id[numThreads];
    struct thread_info info[numThreads];
    pthread_t printer_thread;

    // initialize barrier
    if (pthread_barrier_init(&barrier, NULL, numThreads+1) != 0){
        printf("pthread barrier init has failed\n");
        return 1;
    }

    //Splits columns per thread resonably
    int columns_per_thread = m.col / numThreads;
    int extra = m.col % numThreads;
    int p = extra;

    int column_tracker = 0;

    //Creates the threads that do the computation and passes information to them.
    for(int i = 0; i < numThreads; i++){
        info[i].id = i;
        info[i].start = column_tracker;
        info[i].count = columns_per_thread;
        if(p!=0){
            info[i].count++;
            p--;
            column_tracker++;
        }
        //Prints out information for debugging purposes.
        if(verbosity > 0){
            printf("tid %d\t columns:\t%d:%d\t(%d)\n", info[i].id, info[i].start, info[i].start+info[i].count, info[i].count);
            if(i == numThreads-1){
                printf("\n");
            }
        }
        column_tracker+= columns_per_thread;
        if(pthread_create(&thread_id[i], NULL, thread_func, (void *)&info[i])!=0){
            printf("thread cannot be created\n"); return 1;
        }
    }

    int currGen = 0;

    while(currGen < maxGen){
        m.arr = fillHalo(m.arr, m.row+2, m.col+2);
        if(displayFreq>0 & currGen%displayFreq == 0){
            printMatrix(&m, currGen);
        }

        pthread_barrier_wait(&barrier); // Initial Sync with the other threads
        pthread_barrier_wait(&barrier); //Sync before swap
        pthread_barrier_wait(&barrier); //Sync after swap
        currGen++;
    }

    //Joins all threads.
    for(int i = 0; i < numThreads; i++){
        pthread_join(thread_id[i], NULL);
    }

    //Prints time
    if(verbosity == 2){
        gettimeofday(&myTVend, NULL);                                              
        TIME_DIFF* difference;                                                      
        difference = my_difftime(&myTVstart, &myTVend);                            
        printf("time: sec %3d  microsec %6d \n", difference->secs, difference->usecs);
        free(difference);   
    }

    // Destroy pthread barrier
    pthread_barrier_destroy(&barrier);

    //Frees allocated memory for both 2D Arrays
    freeMatrix(m.arr, m.row+2, m.col+2);
    freeMatrix(n.arr, n.row+2, n.col+2);
}

void *thread_func(void *varg){
    struct thread_info *info = (void * ) varg;
    
    int currGen = 0;
    while(currGen<maxGen){
        pthread_barrier_wait(&barrier); // Initial Sync - for main thread to print

        //Swaps points of the update array and old array so we can reuse the
        //allocated data.
        TYPE** tmp = runGen(m.arr, n.arr, info->start, info->start + info->count, m.row+2);
        pthread_barrier_wait(&barrier); // Sync before swapping pointers
    
        for(int i = 1+info->start; i <info->start + info->count+1; i++){
           TYPE* x = m.arr[i];
           m.arr[i] = n.arr[i];
           n.arr[i] = x;
        }

        pthread_barrier_wait(&barrier); //Sync after swappings
        currGen++;
    }
    pthread_exit(NULL);
}

char** runGen(char** a, char** b, int start, int end, int row){
    
    for(int i = start+1; i < end+1; i++){
        for(int j = 1; j < row-1; j++){
            
            //Counts neighbors by adding all the values of the 3 by 3 matrix 
            //centered at a point
            int x = 0;
            for(int m = -1; m <= 1; m++){
                for(int n = -1; n <= 1; n++){
                    x = x + a[i+m][j+n] - 48;
                }
            }
            
            //Updates the point based on conditions of Game of Life
            if(a[i][j] == '1'){
                //Subtracts value that would be added to num of neighbors
                //since it does include the point itself.
                x = x -1;
                if(x == 2 | x == 3){
                    b[i][j] = '1';
                }
                else{
                    b[i][j] = '0';
                }
            }
            else{
                if(x == 3){
                    b[i][j] = '1';
                }
                else{
                    b[i][j] = '0';
                }
            }
        }
    }
    
    return b;
}


//This function prints the data from a 2D array to stdout
void printMatrix(Matrix* x, int genNum){
    
    printf("------------\n");
    printf("Generation %d\n", genNum);
    printf("------------\n");

  
    for(int i = 0; i < x->row+2; i++){
        for(int j = 0; j < x->col+2; j++){
            printf("%c ", x->arr[i][j]);
        }
        printf("\n");
    }
    printf("------------\n");
    
}


//Returns a 2D array given file inputted by user with points in file.
Matrix generateMatrixFileInput(){
    //Initialize variables to read in and use the file.
    FILE *f;
    char fname[100];
    int inputSize = 0;

    Matrix a;

    //Asks user for file name.
    printf("Matrix File Path: ");
    scanf("%s", fname);

    //Opens file.
    f = fopen(fname, "r");

    //Error check for if the file does not exist.
    if(f == NULL){
        printf("Error with file");
        exit(0);
    }
    //Scans the first line of the file.
    fscanf(f, "%d %d %d", &a.row, &a.col, &inputSize);

    a.arr = malloc((a.col+2) * sizeof(char*));

    for(int i = 0; i < a.col +2; i++){
        a.arr[i] = malloc((a.row+2) * sizeof(char));        
        for(int j = 0; j < a.row+2; j++){
            a.arr[i][j] = '0';
        }
    }

    //Reads the remaining lines of the file and sets point in matrix to 
    //those values.       
    while(inputSize!=0){
        int x, y;
        fscanf(f, "%d %d", &x, &y);    
        a.arr[x+1][y+1] = '1';
            
        inputSize--;
    }
    
    //Closes file.
    fclose(f);
    
    a.arr = fillHalo(a.arr, a.row+2, a.col+2);
    
    return a;
    }

//This function generates the 2D array given number of rows and columns.
//Generates random starting points given seed.
TYPE** generateMatrixUserInput(int r, int c){
    TYPE** a = malloc(c * sizeof(char*));

    for(int i = 0; i < c; i++){
        a[i] = malloc(r * sizeof(char));        
        for(int j = 0; j < r; j++){
            if((int)(2.0 * (double) rand()/(RAND_MAX+1.0)) == 1){
                a[i][j] = '1';
            }
            else{
                a[i][j] = '0';
            }
        }
    }
    return a;
}

void freeMatrix(TYPE** a, int r, int c){
    for(int i = 0; i < r; i++){
        free(a[i]);
    }
    free(a);
}

//This function fills in the outside ranks of the matrix to reflect the otherside
//of the matrix.
char** fillHalo(TYPE** a, int r, int c){
    for(int i = 1; i < c-1; i++){
        a[0][i] = a[r-2][i];
        a[r-1][i] = a[1][i];
    }
    for(int i = 1; i < r-1; i++){
        a[i][0] = a[i][c-2];
        a[i][c-1] = a[i][1];
    }
    
    //Fills in the corners
    a[0][0] = a[r-2][c-2];
    a[r-1][c-1] = a[1][1];
    a[r-1][0] = a[1][c-2]; 
    a[0][c-1] = a[r-2][1]; 
    
    return a;
}