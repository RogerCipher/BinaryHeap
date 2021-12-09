/*
--------------------------------------------------------
by: Rogério Chaves (AKA CandyCrayon), 2021
//    __                       __                       
//   /  )             /       /  )                      
//  /   __.  ____  __/ __  , /   __  __.  __  , __ ____ 
// (__/(_/|_/ / <_(_/_/ (_/_(__// (_(_/|_/ (_/_(_)/ / <_
//                       /                  /           
//                      '                  '            
--------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>

#include "priorityQueues.h"

BufferMinHeap *createHeap()
{
    BufferMinHeap *buffer = (BufferMinHeap *)malloc(sizeof(BufferMinHeap));

    buffer->indexOfLastFilled = 0; //no elements in the buffer

    for(int i = 0; i < MAXPRIOS; i++)
        buffer->data[i] = NULL;

    return buffer;
}


BinHeap *top(BufferMinHeap *buff)
{
    BinHeap *topNode = buff->data[1];
    return topNode;
}

//switches 2 elements, in pos1 and pos2
int swithPositions(BufferMinHeap *buff, int pos1, int pos2)
{
    if(buff->data[pos2] == NULL || buff->data[pos1] == NULL)
        return 0;


    BinHeap *aux = (BinHeap *)malloc(sizeof(BinHeap));
    aux->prio = buff->data[pos1]->prio;
    aux->timeStamp = buff->data[pos1]->timeStamp;

    buff->data[pos1]->prio = buff->data[pos2]->prio;
    buff->data[pos1]->timeStamp = buff->data[pos2]->timeStamp;

    buff->data[pos2]->prio = aux->prio;
    buff->data[pos2]->timeStamp = aux->timeStamp;

    free(aux);
    
    return 1;
}

//promotes the best element recursivelly (child of the "position")
void promoteNode(BufferMinHeap *buff, int position)
{
    //get the 2 possible nodes for this position:
    BinHeap *leftOption = buff->data[position*2];
    BinHeap *rightOption = buff->data[position*2+1];

    if(leftOption == NULL && rightOption == NULL)
    {
        //leaf node, we simply delete the current position
        free(buff->data[position]);
        buff->data[position] = NULL;
        return;
    }

    if(leftOption == NULL || rightOption == NULL)
    {
        //only one option
        if(leftOption == NULL)
        {
            //promote right
            swithPositions(buff, position*2+1, position);
            promoteNode(buff, position*2+1);
        }
            
        else
        {
            //promote left
            swithPositions(buff, position*2, position);
            promoteNode(buff, position*2);
        }
            

        return;
    }

    //both options are possible:
    if(leftOption->prio == rightOption->prio)
    {
        //prios are the same, check time stamps
        if(leftOption->timeStamp > rightOption->timeStamp)
        {
            //promote right
            swithPositions(buff, position*2+1, position);
            promoteNode(buff, position*2+1);
        }
        else
        {
            //promote left
            swithPositions(buff, position*2, position);
            promoteNode(buff, position*2);         
        }
    }
    else
    {
        //best prio wins
        if(leftOption->prio > rightOption->prio)
        {
            //promote right
            swithPositions(buff, position*2+1, position);
            promoteNode(buff, position*2+1);
        }
        else
        {
            //promote left
            swithPositions(buff, position*2, position);
            promoteNode(buff, position*2);         
        }
    }

    return;

}

//checks if parent is bigger than the current node recursivelly and changes it
void demoteNode(BufferMinHeap *buff, int position)
{
    //this is root, nothing to check here
    if(position == 1)
        return;

    int parentPos = 0;

    if(position%2)
        parentPos = (position-1)/2;
    else
        parentPos = position/2;


    if(buff->data[parentPos]->prio < buff->data[position]->prio)
        return;

    if(buff->data[parentPos]->prio == buff->data[position]->prio)
        if(buff->data[parentPos]->timeStamp > buff->data[position]->timeStamp)
            swithPositions(buff, parentPos, position);

    if(buff->data[parentPos]->prio > buff->data[position]->prio)
        swithPositions(buff, parentPos, position);

    demoteNode(buff, parentPos);
    return;
}


BinHeap *pop(BufferMinHeap *buff)
{
    //first lets get the thing we want to pop
    BinHeap *popNode = buff->data[1];
    promoteNode(buff, 1);

    return popNode;
}

BinHeap *push(BufferMinHeap *buff, BinHeap *info)
{
    //start from position 1
    int positionToInsert = 1;

    //see where we can insert this
    while(buff->data[positionToInsert] != NULL)
        positionToInsert++;

    printf("putting it in the position %d\n", positionToInsert);
    //found it
    buff->data[positionToInsert] = info;
    demoteNode(buff, positionToInsert);
    return info;
}

BinHeap *createNode(int priority, int timeStamp)
{
    BinHeap *new = (BinHeap *)malloc(sizeof(BinHeap));
    new->prio = priority;
    new->timeStamp = timeStamp;

    return new;
}
//simple print
void printThisAsTree(BufferMinHeap *buff, int parentNodePos)
{
    if(buff->data[parentNodePos] == NULL)
        return;

    printf("\tn%p [label = \"(%d, %d)\"]\n", buff->data[parentNodePos], buff->data[parentNodePos]->prio, buff->data[parentNodePos]->timeStamp);

    if(buff->data[parentNodePos*2] != NULL)
        printf("\tn%p -- n%p\n", buff->data[parentNodePos], buff->data[parentNodePos*2]);
    if(buff->data[parentNodePos*2+1] != NULL)
        printf("\tn%p -- n%p\n", buff->data[parentNodePos], buff->data[parentNodePos*2+1]);

    if(parentNodePos*2 > MAXPRIOS)
        return;


    printThisAsTree(buff, parentNodePos*2);
    printThisAsTree(buff, parentNodePos*2+1);
} 

void printTree(BufferMinHeap *buff)
{
    printf("\nCopy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");
    printThisAsTree(buff, 1);
    printf("}\n");
}

void printAsBuffer(BufferMinHeap *buff)
{
    int currentNode = 1;
    printf("\n");
    while (currentNode < MAXPRIOS)
    {
        printf("[");
        if(buff->data[currentNode] != NULL)
            printf("%d,%d", buff->data[currentNode]->prio, buff->data[currentNode]->timeStamp);
        else
            printf("X");
        printf("]");
        currentNode++;
    }
    printf("\n");
    return;
}

int main()
{
    BufferMinHeap *buffer = createHeap();
    BinHeap *prio1 = createNode(0, 3);
    BinHeap *prio2 = createNode(2, 3);
    BinHeap *prio3 = createNode(6, 5);
    BinHeap *prio4 = createNode(1, 3);
    BinHeap *prio5 = createNode(7, 3);
    BinHeap *prio6 = createNode(0, 2);


    BinHeap *prio7 = createNode(1, 5);
    BinHeap *prio8 = createNode(3, 3);
    BinHeap *prio9 = createNode(1, 5);
    BinHeap *prio10 = createNode(0, 6);
    BinHeap *prio11 = createNode(1, 12);
    BinHeap *prio12 = createNode(3, 6);

    push(buffer, prio1);
    push(buffer, prio2);
    push(buffer, prio3);
    push(buffer, prio4);
    push(buffer, prio5);
    push(buffer, prio6);


    push(buffer, prio7);
    push(buffer, prio8);
    push(buffer, prio9);
    push(buffer, prio10);
    push(buffer, prio11);
    push(buffer, prio12);

    printTree(buffer);

    pop(buffer);
    pop(buffer);

    //BinHeap *prio13 = createNode(1, 4);    
    //push(buffer, prio13);
    printAsBuffer(buffer);
    printTree(buffer);
    return 0;
}