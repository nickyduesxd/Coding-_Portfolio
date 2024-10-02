#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Data {
    char name[40];
    int company;
};

struct Node {
    struct Data d; 
    struct Node* next;
};

struct Company {
    int coNum;
    int total;
};

int maxCo; // Maximum number of companies
int listSize; //Size of the list
int numSwaps = 0;

//This function creates a Node.
struct Node* create(int c, char n[]) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->d.company= c;
    strcpy(newNode->d.name, n);
    newNode->next = NULL;
    return newNode;
}

//This function inserts a Node to the end of a list.
void endInsert(struct Node** head, int co, char name[]){
    struct Node* nextNode = create(co, name);
    if(*head == NULL){
        *head = nextNode;
        return;
    }
    struct Node* tmp = *head;
    while(tmp->next!=NULL){
        tmp = tmp->next;
    }
    tmp->next = nextNode;
}

//This function swaps the values of 2 nodes in a list and prints the swapped.
void swap(struct Node* a, struct Node* b) {
    struct Data tmpdata = a->d; 
    a->d = b->d; 
    b->d = tmpdata; 
    printf("%s %s\n", b->d.name, a->d.name); 
    numSwaps++;
}

//This function swaps the 2 indexes in an array.
void arraySwap(struct Company arr[], int index1, int index2) {
    struct Company temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}

/*
This function calculates the locality of a certain company at the start,
we divide the current orientation of people into sections iven by the
average size of companies and find the section with the greatest amount of people
from a single company is located in the relative array. Then we switch the locality
array to reflect this and we run through each company in order to overall limit the
amount of swaps.
*/
struct Company* companyLocality(struct Node* head){
    struct Node* current = head;
    int min = 0;
    while(current!=NULL){
        if(min<current->d.company){
            min = current->d.company;
        }
        current = current->next;
    }
    maxCo = min;    
    struct Company *locality = malloc(min * sizeof(struct Company));

    for(int i = 0; i < min; i++){
        locality[i].coNum = i+1;
        locality[i].total = 0;
    }

    current = head;

    while(current!=NULL){
        locality[current->d.company - 1].total +=1;
        current= current->next;
    }

    current = head;
   
    for(int i = 0; i < min; i++){ 
        int counter = 0;
        current = head;
        int *relative = calloc(min, sizeof(int));
        while(current!=NULL){
            if(current->d.company == locality[i].coNum){
                relative[counter/(listSize/maxCo)]+=1;
            }
            counter++;
            current = current->next;    
        }
        
        int x = 0;
        for(int j = 0; j < min; j++){
            if(relative[x]<relative[j]){ 
                x = j;
            }
        }
        arraySwap(locality, i, x);
        free(relative);
    }
    
    return locality;
}
/*
This function is my algorithm, it goes off the lcoality array and breaks up the list
into two parts, first part *is the companies already sorted and the new company
we are sorted based on the iteration we are on in the locality array, and the second
part which is the rest. So we traverse the company whose location we are at and find nodes 
that do not have that company number and then traverse the second part of the list in order
to swap once we get a match. We do this till we go through the end of the list.
*/
void myAlgorithm(struct Company l[], struct Node* head){
    struct Node* curr = head;
    int listPointer = 0; 
    for(int i = 0; i < maxCo-1; i++){
        struct Node* pastLimit = curr;
        for(int j = 0; j < l[i].total; j++){
            pastLimit = pastLimit->next;
        }
        int x = listPointer + l[i].total;
        while(listPointer != x & curr!=NULL){
            if(curr->d.company!=l[i].coNum){
                while((pastLimit!=NULL) & (pastLimit->d.company!=l[i].coNum)){
                    pastLimit = pastLimit->next;
                }
                if(pastLimit!=NULL){
                    swap(curr, pastLimit);
                }
            }
            if(curr!=NULL){
            curr=curr->next;
            listPointer++;
            }
        }        
    }
}

//This is the main function that runs the program.
int main() {
    struct Node* head = NULL;
    int co;
    listSize = 0;
    char name[40];
    while(scanf("%d %s", &co, name)!=EOF){
        endInsert(&head, co, name);
        listSize++;
    }
    
    struct Company* loc = companyLocality(head);
    myAlgorithm(loc, head); 

    //USED TO DEBUG - to make sure the companies are all in order. Prints
    //the final list.
    /*while(head!= NULL){
        printf("%d\n", head->d.company);
        head = head->next;
    }*/

    //printf("%d\n", numSwaps);

    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(loc);
    return 0;
}