#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#ifdef _OPENMP
    // omp library
    #include <omp.h>
#endif

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

void freeMatrix(char**, int, int);
char** generateMatrixFileInput(int*, int*);
char** generateMatrixUserInput(int , int);
void printMatrix(char**, int, int, int, bool);
char** runGen(char** , char**, int, int);
char** fillHalo(char**, int, int);

int main(int argc, char** argv){

    if(argc == 1){
        printf("./ompgol <maxNumGens> <displayFreq> <seed: <0 implies time is used> "); 
        printf("<rows (opt)> <cols (opt)>\n");
        return 0;
    }
    else if(argc != 3 & argc != 6){
        printf("error");
        return 1;
    }

    //Initialize Variables and defines variables based on user input.
    int r = 0, c = 0; //Row and Columns
    int maxNumGens = atoi(argv[1]); //The number of generations to run the simulations.
    int displayFreq = atoi(argv[2]); // The frequency for displaying the grid.
    int seed = -1;
    char** array;    

    //User input Matrix
    if(argc == 6){
        seed = atoi(argv[3]);
        if(seed <0){
            srand(time(0));
        }
        else{
            srand(seed);
        }
        r = atoi(argv[4]) + 2;
        c = atoi(argv[5]) + 2;
        array = generateMatrixUserInput(r, c);
    }
    else{
        //File input matrix
        array = generateMatrixFileInput(&r, &c);
    }
    
    //The flip flop 2D Matrix, stores changes after each generation update.
    char** newarray = generateMatrixUserInput(r, c);

    //Clock
    struct timeval myTVstart, myTVend;      
    gettimeofday(&myTVstart, NULL); 

    //Loop that runs the number of generations.
    for(int i = 0; i < maxNumGens; i++){

        //Print to screen with display frequency
        //If a negative displayFreq is set then updates and final matrix result
        //are not printed.
        if((displayFreq > 0)&(i % displayFreq) == 0){
            printMatrix(array, r, c, i, false);
        }

        //Swaps points of the update array and old array so we can reuse the
        //allocated data.
        char** tmp = array;
        array = runGen(array, newarray, r, c);
        newarray = tmp;

        //Fills the Halo
        array = fillHalo(array, r, c);
    }

    //Prints time
    gettimeofday(&myTVend, NULL);                                              
    TIME_DIFF* difference;                                                      
    difference = my_difftime(&myTVstart, &myTVend);                            
    printf("time: sec %3d  microsec %6d \n", difference->secs, difference->usecs);
    free(difference);

    //Frees allocated memory for both 2D Arrays
    freeMatrix(array, r, c);
    freeMatrix(newarray, r, c);
}

//This function prints the data from a 2D array to stdout
void printMatrix(char** a, int r, int c, int count, bool final){

    if(final==true){
        printf("final\n");
    }
    else{
        printf("count: %d\n", count);
    }

    printf("------------\n");

    for(int i = 0; i < c; i++){
        for(int j = 0; j < r; j++){
            printf("%c ", a[i][j]);
        }
        printf("\n");
    }

    printf("------------\n");
}

//This function generates the 2D array given number of rows and columns.
//Generates random starting points given seed.
char** generateMatrixUserInput(int r, int c){
    char** a = malloc(c * sizeof(char*));
    #pragma omp parallel for shared(a) num_threads(12)
    for(int i = 0; i < c; i++){
        a[i] = malloc(r * sizeof(char));        
        #pragma omp parallel for shared(a) num_threads(12)
        for(int j = 0; j < r; j++){
            if(j == 0 | j == r-1 | i==0 | i==c-1){
                a[i][j] = 'x'; // reflect hallo
            }
            else{
                if((int)(2.0 * (double) rand()/(RAND_MAX+1.0)) == 1){
                    a[i][j] = '1';
                }
                else{
                    a[i][j] = '0';
                }
            }
        }
    }
    a = fillHalo(a, r, c);
    return a;
}

//Frees allocated dat for 2D Matrix
void freeMatrix(char** a, int r, int c){
    #pragma omp parallel for shared(a) num_threads(128)
    for(int i = 0; i < r; i++){
        free(a[i]);
    }

    free(a);
}

//Returns a 2D array given file inputted by user with points in file.
char** generateMatrixFileInput(int *r, int *c){
    //Initialize variables to read in and use the file.
    FILE *f;
    char fname[100];
    int inputSize = 0;

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
    fscanf(f, "%d %d %d", r, c, &inputSize);
    
    *r = *r+2;
    *c = *c+2;

    char** array = malloc(*r * sizeof(char*));
    #pragma omp parallel for shared(array) num_threads(128)
    for(int i = 0; i < *c; i++){
        array[i] = malloc(*r * sizeof(char));        
        #pragma omp parallel for shared(array) num_threads(128)
        for(int j = 0; j < *r; j++){
            if(j == 0 | j == *r-1 | i==0 | i== *c-1){
                array[i][j] = 'x';
            }
            else{
                array[i][j] = '0';
            }
        }
    }

    //Reads the remaining lines of the file and sets point in matrix to 
    //those values.       
    while(inputSize!=0){
        int x, y;
        fscanf(f, "%d %d", &x, &y);    
        array[x+1][y+1] = '1';
            
        inputSize--;
    }
    
    //Closes file.
    fclose(f);
    
    array = fillHalo(array, *r, *c);
    
    return array;
    }

char** runGen(char** a, char** b, int r, int c){
    //Fills in HALO
    a = fillHalo(a, r, c);

    #pragma omp parallel for shared(a,b) num_threads(50)  
    for(int i = 1; i < c - 1; i++){
        for(int j = 1; j < r - 1; j++){
            
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
    
    b = fillHalo(b, r, c);
    
    return b;
}

//This function fills in the outside ranks of the matrix to reflect the otherside
//of the matrix.
char** fillHalo(char** a, int r, int c){
    #pragma omp parallel for shared(a) num_threads(128)
    for(int i = 1; i < c-1; i++){
        a[i][0] = a[i][c-2];
        a[i][c-1] = a[i][1];
        
    }
    #pragma omp parallel for shared(a) num_threads(128)
    for(int i = 1; i < r-1; i++){
        a[0][i] = a[r-2][i];
        a[r-1][i] = a[1][i];
    }
    
    //Fills in the corners
    a[0][0] = a[c-2][r-2];
    a[c-1][r-1] = a[1][1];
    a[c-1][0] = a[1][r-2]; 
    a[0][r-1] = a[c-2][1]; 
    
    return a;
}