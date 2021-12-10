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

//simple error throwing function
void error(const char *msg) 
{
    perror(msg);
    exit(1);
}

BufferMinHeap *createHeap()
{
    BufferMinHeap *buffer = (BufferMinHeap *)malloc(sizeof(BufferMinHeap));
    if(buffer == NULL)
        error("Error Alocating memory\n");

    buffer->indexOfLastFilled = 0; //no elements in the buffer

    for(int i = 0; i < MAXPRIOS; i++)
        buffer->data[i] = NULL; //initialization of the buffer

    return buffer;
}

//returns 1 if pos1 is bigger(in case of minHeap) than pos2
int shouldSwitchNodes(BufferMinHeap *buff,int pos1,int pos2)
{
    if((buff->data[pos1]->prio > buff->data[pos2]->prio) || (buff->data[pos1]->prio == buff->data[pos2]->prio && buff->data[pos1]->timeStamp > buff->data[pos2]->timeStamp))
        return 1;
    return 0;
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
    if(aux == NULL)
        error("Error Alocating memory\n");
    aux->prio = buff->data[pos1]->prio;
    aux->timeStamp = buff->data[pos1]->timeStamp;

    buff->data[pos1]->prio = buff->data[pos2]->prio;
    buff->data[pos1]->timeStamp = buff->data[pos2]->timeStamp;

    buff->data[pos2]->prio = aux->prio;
    buff->data[pos2]->timeStamp = aux->timeStamp;

    free(aux);
    
    return 1;
}

//checks if parent is bugger than children and changes it recursivelly down the tree
void promoteNode(BufferMinHeap *buff, int position)
{
    //check if it has children:
    if(position*2 > buff->indexOfLastFilled)
        return;

    //check if it only has 1 child:
    if(position*2 == buff->indexOfLastFilled)
    {
        if(shouldSwitchNodes(buff, position, position*2))
            swithPositions(buff, position, position*2);

        return;
    }

    //if it has 2 children:
    int leftChildPosition = position*2;
    int rightChildPosition = position*2+1;

    //best child will be 1 if rightChild is bigger than leftChild
    int bestChild = shouldSwitchNodes(buff, rightChildPosition, leftChildPosition);

    //if left child is better:
    if(bestChild)
        if(shouldSwitchNodes(buff, position, leftChildPosition))
        {
            swithPositions(buff, position, leftChildPosition);
            promoteNode(buff, leftChildPosition);
            return;
        }
    //if right child is better:
    if(shouldSwitchNodes(buff, position, rightChildPosition))
    {
        swithPositions(buff, position, rightChildPosition);
        promoteNode(buff, rightChildPosition);
        return;
    }
    
    //otherwise there's nothing to do, we are good here
    return;

}

//checks if parent is bugger than children and changes it recursivelly up the tree
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

        
    if(shouldSwitchNodes(buff, parentPos, position))
    {
        swithPositions(buff, parentPos, position);
        demoteNode(buff, parentPos);
        return;
    }
    return;
}


BinHeap *pop(BufferMinHeap *buff)
{
    //check if the queue is empty
    if(buff->indexOfLastFilled == 0)
        return NULL;


    //first lets get the thing we want to pop
    BinHeap *popNode = (BinHeap *)malloc(sizeof(BinHeap));
    if(popNode == NULL)
        error("Error Alocating memory\n");

    popNode->prio = buff->data[1]->prio;
    popNode->timeStamp = buff->data[1]->timeStamp;

    //check if the queue only has one node:
    if(buff->indexOfLastFilled == 1)
    {
        free(buff->data[1]);
        buff->data[1] = NULL;

        buff->indexOfLastFilled = 0;
        return popNode;
    }    
    
    
    //for every other case:
    swithPositions(buff, 1, buff->indexOfLastFilled);
    free(buff->data[buff->indexOfLastFilled]);
    buff->data[buff->indexOfLastFilled] = NULL;
    buff->indexOfLastFilled--;
    
    promoteNode(buff, 1);

    return popNode;
}

BinHeap *push(BufferMinHeap *buff, BinHeap *info)
{
    int positionToInsert = buff->indexOfLastFilled +1;
    buff->indexOfLastFilled++;

    printf("putting it in the position %d\n", positionToInsert);
    //found it
    buff->data[positionToInsert] = info;
    demoteNode(buff, positionToInsert);
    return info;
}

BinHeap *createNode(int priority, int timeStamp)
{
    BinHeap *new = (BinHeap *)malloc(sizeof(BinHeap));
    if(new == NULL)
        error("Error Alocating memory\n");
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

void freeBuffer(BufferMinHeap *buff)
{
    for(int i = 1; i < buff->indexOfLastFilled+1; i++)
    {
        free(buff->data[i]);
        buff->data[i] = NULL;
    }

    free(buff);
    buff = NULL;

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

    printAsBuffer(buffer);
    printTree(buffer);

    pop(buffer);
    pop(buffer);
    pop(buffer);
    BinHeap *prio13 = createNode(1, 4);    
    push(buffer, prio13);
    printAsBuffer(buffer);
    printTree(buffer);
    freeBuffer(buffer);
    return 0;
}