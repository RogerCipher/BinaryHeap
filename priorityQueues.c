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


    BinHeap *aux = buff->data[pos1];
    buff->data[pos1] = buff->data[pos2];
    buff->data[pos2] = aux;
    return 1;
}

//promotes the best element recursivelly (child of the "position")
void promoteNode(BufferMinHeap *buff, int position)
{
    //get the 2 possible nodes for this position:
    //BinHeap *leftOption = buff->data[position*2];
    //BinHeap *rightOption = buff->data[position*2+1];

    if(buff->data[position*2] == NULL || buff->data[position*2+1])
    {
        if(buff->data[position*2] != NULL)
        {
            //this is our only option, promote this
            buff->data[position] = buff->data[position*2];

            //recursivelly check the rest
            promoteNode(buff, position*2);
            return;
        }
        else if(buff->data[position*2+1] != NULL)
        {
            //this is our only option, promote this
            buff->data[position] = buff->data[position*2+1];

            //recursivelly check the rest
            promoteNode(buff, position*2+1);
            return;
        }
        //there are no leaf nodes to choose from, so we kill the parent
        free(buff->data[position]);
        buff->data[position] == NULL;
        return;
    }


    if(buff->data[position*2]->prio == buff->data[position*2+1]->prio)
    {
        //same prio, wins who has the best timestamp:
        if(buff->data[position*2]->timeStamp < buff->data[position*2+1]->prio)
        {
           buff->data[position] = buff->data[position*2];

            //recursivelly check the rest
            promoteNode(buff, position*2);
            return;
        }

        buff->data[position] = buff->data[position*2+1];

        //recursivelly check the rest
        promoteNode(buff, position*2+1);
        return;


    }
    else if(buff->data[position*2]->prio < buff->data[position*2+1]->prio)
    {
        buff->data[position] = buff->data[position*2];

        //recursivelly check the rest
        promoteNode(buff, position*2);
        return;
    }

    buff->data[position] = buff->data[position*2+1];

    //recursivelly check the rest
    promoteNode(buff, position*2+1);
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
    while(buff->data[positionToInsert] == NULL)
        positionToInsert++;

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

    printf("\tn%p [label = (%d, %d)\"\"]\n", buff->data[parentNodePos], buff->data[parentNodePos]->prio, buff->data[parentNodePos]->timeStamp);

    if(buff->data[parentNodePos*2] != NULL)
        printf("\tn%p -- n%p\n", buff->data[parentNodePos], buff->data[parentNodePos*2]);
    if(buff->data[parentNodePos*2+1] != NULL)
        printf("\tn%p -- n%p\n", buff->data[parentNodePos], buff->data[parentNodePos*2+1]);

    printThisAsTree(buff, parentNodePos*2);
    printThisAsTree(buff, parentNodePos*2+1);
} 

int main()
{
    BufferMinHeap *buffer = (BufferMinHeap *)malloc(sizeof(BufferMinHeap));

    buffer->indexOfLastFilled = 0; //no elements in the buffer

    BinHeap *prio1 = createNode(0, 3);
    BinHeap *prio2 = createNode(2, 3);
    BinHeap *prio3 = createNode(6, 5);
    BinHeap *prio4 = createNode(1, 3);
    BinHeap *prio5 = createNode(7, 3);
    BinHeap *prio6 = createNode(0, 2);

    push(buffer, prio1);
    push(buffer, prio2);
    push(buffer, prio3);
    push(buffer, prio4);
    push(buffer, prio5);
    push(buffer, prio6);

    printThisAsTree(buffer, 1);

    return 0;
}