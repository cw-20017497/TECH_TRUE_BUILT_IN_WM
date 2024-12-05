#include "hw.h"
#include "grinding.h"

#include "hal_input.h"

/* MODE */
#define  GRINDING_MODE_NONE    0x00
#define  GRINDING_MODE_INIT    0x01
#define  GRINDING_MODE_CLOSE   0x02
#define  GRINDING_MODE_OPEN    0x04


#define MAX_ON_TIME     300     /* 30sec @ 100ms */
#define DR_DETECT       0
#define DR_UNDETECT     1

typedef struct _grinding_
{
    /* STATUS */
    U8 Status;       // Open or Close
    
    /* MODE */
    U8 Mode;         // Init / Close / Open 
    U8 InitStep;
    U8 OpenStep;
    U8 CloseStep;
    //U8 TestStep;

    /* Forced Close Grinding Timer */
    U32 CloseTime;

    U16 OpeningTimeIot;        /* @Sec */
    U16 OpeningTime;        /* @Sec */
} Grinding_T;

Grinding_T  Grinding;

void InitGrinding(void)
{
    Grinding.Status      = GRINDING_CLOSE;

    Grinding.Mode        = GRINDING_MODE_INIT;
    Grinding.InitStep    = 0;
    Grinding.OpenStep    = 0;
    Grinding.CloseStep   = 0;

    Grinding.OpeningTime    = 0;

}


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );

U8 GetGrindingStatus(void)
{
    return Grinding.Status;
}

// 아이스 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenGrinding(void)
{
    if( Grinding.Status == GRINDING_OPEN 
            || Grinding.Status == GRINDING_OPENING )
    {
        return TRUE;
    }

    return FALSE;
}

static void SetGrindingStatus(U8 mu8Status)
{
    Grinding.Status = mu8Status;
}

void OpenGrinding(void)
{
    SetMode( GRINDING_MODE_OPEN );
}

void CloseGrinding(void)
{
    SetMode( GRINDING_MODE_CLOSE );
}


static void SetMode(U8 mu8Mode )
{
    Grinding.Mode |= mu8Mode;
    if( mu8Mode == GRINDING_MODE_OPEN )
    {
        ClearMode( GRINDING_MODE_CLOSE );
    }
    else if( mu8Mode == GRINDING_MODE_CLOSE )
    {
        ClearMode( GRINDING_MODE_OPEN );
    }
}

static void ClearMode(U8 mu8Mode )
{
    Grinding.Mode &= ~mu8Mode;

    if( mu8Mode == GRINDING_MODE_INIT )
    {
        Grinding.InitStep = 0;
    }
    else if( mu8Mode == GRINDING_MODE_OPEN )
    {
        Grinding.CloseStep = 0;
    }
    else if( mu8Mode == GRINDING_MODE_CLOSE )
    {
        Grinding.OpenStep = 0;
    }
}



static void ClearMode(U8 mu8Mode );
static U8 InitDoor(void);
static U8 OpenDoor(void);
static U8 CloseDoor(void);

void ControlGrinding(void)
{
    U8 mu8Ret = FALSE;


    /* Init */
    if( (Grinding.Mode & GRINDING_MODE_INIT ) == GRINDING_MODE_INIT )
    {
        mu8Ret = InitDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( GRINDING_MODE_INIT );
            SetGrindingStatus( GRINDING_CLOSE );
        }
        else
        {
            SetGrindingStatus( GRINDING_CLOSING );
        }
        return ;
    }

    /* Open */
    if( (Grinding.Mode & GRINDING_MODE_OPEN ) == GRINDING_MODE_OPEN )
    {
        mu8Ret = OpenDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( GRINDING_MODE_OPEN );
            SetGrindingStatus( GRINDING_OPEN );
        }
        else
        {
            SetGrindingStatus( GRINDING_OPENING );
        }
        return ;
    }

    /* Close */
    if( (Grinding.Mode & GRINDING_MODE_CLOSE ) == GRINDING_MODE_CLOSE )
    {
        mu8Ret = CloseDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( GRINDING_MODE_CLOSE );
            SetGrindingStatus( GRINDING_CLOSE );
        }
        else
        {
            SetGrindingStatus( GRINDING_CLOSING );
        }
        return ;
    }

}



static U8 IsClose(void)
{
    if( GET_STATUS_MOTOR_CLOSE() == 0 )
    {
        return TRUE;
    }

    return FALSE;
}


static U8 InitDoor(void)
{
    static U16 mu16DelayTime = 30;   /* 3sec@100ms */


    switch( Grinding.InitStep )
    {
        case 0:
            TURN_OFF_GRINDING_MOTOR();
            TURN_OFF_DOOR_MOTOR();

            Grinding.InitStep++;
            break;

        case 1:
            // power on stable time... for detect input value... 
            // plz wait... a few second..
            if( mu16DelayTime != 0 )
            {
                mu16DelayTime--;
                break;
            }

            // ok check out input status
            if( HAL_GetInputValue( IN_DOOR_CLOSE ) == DR_UNDETECT )
            {
                TURN_ON_DOOR_MOTOR();

                mu16DelayTime = MAX_ON_TIME;   /* max operating time : 30sec */
                Grinding.InitStep++;
            }
            else
            {
                return TRUE;
            }
            break;

        case 2:
            // open door and then wait detect close door


            // time out max operation...
            if( mu16DelayTime != 0 )
            {
                mu16DelayTime--;
            }

            if( HAL_GetInputValue( IN_DOOR_CLOSE ) == DR_DETECT
                    || mu16DelayTime == 0 )
            {
                TURN_OFF_DOOR_MOTOR();
                Grinding.InitStep++;
            }
            break;

        case 3:
            return TRUE;

    }

    return FALSE;
}

// 도어 OPEN 제어
static U8 OpenDoor(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */


    switch( Grinding.OpenStep )
    {
        case 0:
            if( HAL_GetInputValue( IN_DOOR_OPEN ) == DR_UNDETECT )
            {
                TURN_ON_DOOR_MOTOR();

                mu16DelayTime = MAX_ON_TIME;   /* max operating time : 30sec */
            }

            Grinding.OpenStep++;
            break;

        case 1:
            if( HAL_GetInputValue( IN_DOOR_OPEN ) == DR_DETECT )
            {
                TURN_OFF_DOOR_MOTOR();
                mu16DelayTime = 10;   /*  1sec@100ms */

                Grinding.OpenStep++;
            }
            break;

        case 2:
            if( mu16DelayTime != 0 )
            {
                mu16DelayTime--;
            }

            TURN_ON_GRINDING_MOTOR();
            return TRUE;
    }

    return FALSE;

}

static U8 CloseDoor(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */


    switch( Grinding.CloseStep )
    {
        case 0:
            if( GET_STATUS_GRINDING_MOTOR() != 0 )
            {
                TURN_OFF_GRINDING_MOTOR();
                mu16DelayTime = 10;   /*  1sec@100ms */
            }
            else
            {
                mu16DelayTime = 0;   
            }

            Grinding.CloseStep++;
            break;

        case 1:
            // delay..
            if( mu16DelayTime != 0 )
            {
                mu16DelayTime--;
                break;
            }

            if( HAL_GetInputValue( IN_DOOR_CLOSE ) == DR_UNDETECT )
            {
                TURN_ON_DOOR_MOTOR();
            }

            mu16DelayTime = MAX_ON_TIME;   /* max operating time : 30sec */
            Grinding.CloseStep++;
            break;

        case 2:
            if( mu16DelayTime != 0 )
            {
                mu16DelayTime--;
            }

            if( HAL_GetInputValue( IN_DOOR_CLOSE ) == DR_DETECT 
                    || mu16DelayTime == 0 )
            {
                TURN_OFF_DOOR_MOTOR();
                Grinding.CloseStep++;
            }
            break;

        case 3:
            return TRUE;

    }

    return FALSE;
}
