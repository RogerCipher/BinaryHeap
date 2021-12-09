#ifndef __PRIORITYQUEUES_H__
#define __PRIORITYQUEUES_H__

#define MAXPRIOS 32

/*---data structures---*/

typedef struct st_binHeap
{
    int prio;
    int timeStamp;
}BinHeap;

typedef struct st_bufferMinHeap
{
    BinHeap *data[MAXPRIOS];
    
    int indexOfLastFilled;
}BufferMinHeap;

/*---low level operations on the heap---*/

int shouldSwitchNodes(BufferMinHeap *buff,int pos1,int pos2);
int swithPositions(BufferMinHeap *buff, int pos1, int pos2);
void promoteNode(BufferMinHeap *buff, int position);
void demoteNode(BufferMinHeap *buff, int position);

/*---high level operations on the heap---*/

BinHeap *top(BufferMinHeap *buff);
BinHeap *pop(BufferMinHeap *buff);
BinHeap *push(BufferMinHeap *buff, BinHeap *info);

/*---element creators---*/

BinHeap *createNode(int priority, int timeStamp);
BufferMinHeap *createHeap();

/*---stuff for printing---*/

void printThisAsTree(BufferMinHeap *buff, int parentNodePos);
void printTree(BufferMinHeap *buff);
void printAsBuffer(BufferMinHeap *buff);

#endif
