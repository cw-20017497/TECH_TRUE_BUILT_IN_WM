/**
 * File : queue.c
 * 
 * Queue
**/
#include "queue.h"


// 
void InitQueue(SQueue_T *xStQueue, void *mUdata, U8 mUdataSize, U8 mUqueueNum )
{
    MEMSET( (void __FAR *)mUdata, 0, (mUdataSize * mUqueueNum) );

    xStQueue->mUrear = 0;
    xStQueue->mUfront = 0;
    xStQueue->mUdata = mUdata;
    xStQueue->mUdataSize = mUdataSize;
    xStQueue->mUqueueNum = mUqueueNum;
}

U8 IsQueueEmpty(SQueue_T *xStQueue)
{
    if( xStQueue->mUrear == xStQueue->mUfront )
    {
        return TRUE;
    }

    return FALSE;
}

U8 IsQueueFull(SQueue_T *xStQueue)
{
    if( xStQueue->mUfront == ( xStQueue->mUrear + 1 ) % xStQueue->mUqueueNum )
    {
        return TRUE;
    }

    return FALSE;
}

void EnQueue(SQueue_T *xStQueue, void *xUdata)
{
    if( IsQueueFull(xStQueue) == TRUE )
    {
        return;
    }

    MEMCPY((U8 *)xStQueue->mUdata + (xStQueue->mUrear * xStQueue->mUdataSize), xUdata, xStQueue->mUdataSize);
    xStQueue->mUrear = (xStQueue->mUrear + 1) % xStQueue->mUqueueNum;
}

U8 DeQueue(SQueue_T *xStQueue, void *xUptrData)
{
    if( IsQueueEmpty(xStQueue) == TRUE )
    {
        return FALSE;
    }

    MEMCPY(xUptrData, (U8 *)xStQueue->mUdata + (xStQueue->mUfront * xStQueue->mUdataSize), xStQueue->mUdataSize);
    xStQueue->mUfront = (xStQueue->mUfront + 1) % xStQueue->mUqueueNum;

    return TRUE;
}

U8 PeekQueue(SQueue_T *xStQueue, void *xUptrData)
{
    if( IsQueueEmpty(xStQueue) == TRUE )
    {
        return FALSE;
    }

    MEMCPY(xUptrData, (U8 *)xStQueue->mUdata + ((xStQueue->mUfront + 1) % xStQueue->mUqueueNum * xStQueue->mUdataSize), xStQueue->mUdataSize);

    return TRUE;
}
