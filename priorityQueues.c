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

//promotes the best element (child of the "position")
void promoteNode(BufferMinHeap *buff, int position)
{
    //get the 2 possible nodes for this position:
    BinHeap *leftOption = buff->data[position*2];
    BinHeap *rightOption = buff->data[position*2+1];

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


BinHeap *pop(BufferMinHeap *buff)
{
    //first lets get the thing we want to pop
    BinHeap *popNode = buff->data[1];
    promoteNode(buff, 1);

    return popNode;
}

BinHeap *push(BufferMinHeap *buff)
{

}


void printThisAsTree(BufferMinHeap *buff, int parentNodePos)
{
    
}

int main()
{
    BufferMinHeap *buffer = (BufferMinHeap *)malloc(sizeof(BufferMinHeap));

    buffer->indexOfLastFilled = 0; //no elements in the buffer



}