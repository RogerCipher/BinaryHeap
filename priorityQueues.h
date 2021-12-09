#ifndef __PRIORITYQUEUES_H__
#define __PRIORITYQUEUES_H__

#define MAXPRIOS 32


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


#endif
