#include "comm_queue.h"
#include "queue.h"

#include "timer.h"
#include "timer_id.h"

#define MAX_QUEUE_NUM   30
CommData_T comm_front[ MAX_QUEUE_NUM ];
SQueue_T comm_queue[ MAX_COMM_ID ];     


#define RETRY_REQ_COUNT    3
#define RETRY_ACK_COUNT    1
#define RETRY_WAIT_TIME   50
CommData_T front;

void InitCommQueue(void)
{
    InitQueue( &comm_queue[ COMM_ID_FRONT ], &comm_front, sizeof( CommData_T ), MAX_QUEUE_NUM );
}


void SetCommQueueFront(U8 packet)
{
    CommData_T data;

    data.packet         = packet;

    if( packet == PKT_FRONT_ACK_LED )
    {
        data.retry_count = RETRY_ACK_COUNT;
    }
    else
    {
        data.retry_count = RETRY_REQ_COUNT;
    }

    EnQueue( &comm_queue[ COMM_ID_FRONT ], &data);
}



void SendPacketQueueFront(void)
{
    if( IsExpiredTimer( TIMER_ID_COMM_WAIT_1 ) != TIMER_EXPIRE )
    {
        return ;
    }

    if( IsExpiredTimer( TIMER_ID_COMM_FRONT_TX_DONE ) != TIMER_EXPIRE )
    {
        return ;
    }

    if( front.retry_count == 0 )
    {
        if( DeQueue( &comm_queue[ COMM_ID_FRONT ], (void *)&front ) == TRUE )
        {
            // decrement count -> send message 
            front.retry_count--;

            SetCommHeader( COMM_ID_FRONT, front.packet);
            StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );
        }
    }
    else
    {
        front.retry_count--;

        SetCommHeader( COMM_ID_FRONT, front.packet);
        StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );
    }

    StartTimer( TIMER_ID_COMM_WAIT_1, RETRY_WAIT_TIME );
}


void ReceivedFrontAck(U8 packet)
{
    if( packet == front.packet )
    {
        front.retry_count = 0;
    }
}


