#include "hw.h"
#include "display.h"
#include "process_display.h"
#include "display_lib.h"
#include "timer.h"
#include "parser.h"
#include "parser_front.h"
#include "hal_led.h"

#include "comm_queue.h"




static void UpdateTimer(void);
static void ProcessDisplayBoot(void);



typedef struct _display_
{
    U8 Init;            /* FALSE : 초기화 전, TRUE : 초기화 완료 */
} Display_T;

Display_T   Disp;


void InitDisplay(void)
{
    // TIMER 
    InitDispTimer();

    /* Booting Display - Showing Blink Front Display */
    Disp.Init = FALSE;

    /* TURN OFF ALL LED */
    TurnOffAllLED();
}


U8 GetDisplayInit(void)
{
    return Disp.Init;
}

void StartDisplayInit(void)
{
    Disp.Init = FALSE;

    InitDispTimer();
}

typedef struct _disp_id_time_
{
    U8 id;
    U16 mTime;
} DispTimerVal_T;

DispTimerVal_T DispTimerValTable[] = 
{
    { DISP_TIMER_CONFIG,              TIMER_5SEC }
};
#define SZ_DISP_TIMER_TABLE (sizeof(DispTimerValTable)/sizeof(DispTimerVal_T))

static U16 FindTimeVal(U8 id)
{
    U8 i;

    for( i = 0; i < SZ_DISP_TIMER_TABLE; i++ )
    {
        if( id == DispTimerValTable[ i ].id )
        {
            return DispTimerValTable[ i ].mTime;
        }
    }

    return 0;   // timer 0...
}

// Display Timer start/stop
void StartDispTimerId(U8 id)
{
    U16 mTime;

    mTime = FindTimeVal( id );
    SetDispTimer( id, mTime );
}

void StopDispTimerId(U8 id)
{
    SetDispTimer( id, 0);
}


/* 디스플레이 초기화 표시 */
static void ProcessDisplayBoot(void)
{
    static U8 mu8Count = 5;
    static U8 mu8Toggle = 0;

    if( IsExpiredDispBlinkTimer( DISP_TIMER_ID_500MS ) == TRUE )
    {
        mu8Toggle = !mu8Toggle;

        mu8Count--;
        if( mu8Count == 0 )
        {
            TurnOffAllLED();

            Disp.Init = TRUE;
            mu8Count = 5;
            return;
        }
    }

    if( mu8Toggle == 0 )
    {
        TurnOffAllLED();
    }
    else
    {
        TurnOnAllLED();
    }
}



U8 dbg_the_disp_update = 0;     // for debug
static void Update(void)
{
//    if( HAL_IsUpdateLed() == TRUE )
    {
        SetCommQueueFront( PKT_FRONT_REQ_LED );

        HAL_UpdateLed();
        dbg_the_disp_update++;  // for debug
    }
}

U8 dbg_duty = 10;
U8 led_onoff = TRUE;
static void ProcessDisplayormalMode(void)
{
    HAL_SetLedDuty( dbg_duty );

    if( led_onoff == TRUE )
    {
        TurnOnAllLED();
    }
    else
    {
        TurnOffAllLED();
    }
}

void ProcessDisplay(void)
{
    UpdateDisplayTimer();

    /* BOOT */
    if( Disp.Init == FALSE )
    {
        ProcessDisplayBoot();
        Update();
        return ;
    }

    /* NORMAL MODE */
    ProcessDisplayormalMode();
    Update();
}
