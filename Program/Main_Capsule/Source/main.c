#include "hw.h"
#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "parser.h"
#include "process_sys_event.h"

/* Init Timer */
static void InitStartTimers( void )
{
    InitTimer();

    StartTimer( TIMER_ID_1MS,   1);
    StartTimer( TIMER_ID_10MS,  10);
    StartTimer( TIMER_ID_100MS, 100);
    StartTimer( TIMER_ID_1SEC,  SEC(1));

    StartTimer( TIMER_ID_FRONT, 20 );
    StartTimer( TIMER_ID_COMM_FRONT_RX_ERR, SEC(10) );
    StartTimer( TIMER_ID_COMM_WAIT_1, 1);
    StartTimer( TIMER_ID_COMM_FRONT_TX_DONE, 1);
}


void main( void )
{
    Delay_MS( 1000 );

    R_WDT_Restart();

    InitSystem();
    InitStartTimers();
    EI();

    /* EEPROM */
    R_WDT_Restart();
 
    TURN_ON_SMPS_ONOFF();
    TURN_ON_TEM();

    while(1)
    {
        R_WDT_Restart();

        ProcessEventHandler();

        /* COMM */
        RecvPacketHandler();
        SendPacketHandler();
    }
}
