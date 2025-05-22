#include "hw.h"
#include "capsule.h"

#include "hal_input.h"
#include "hal_step_motor.h"



#define  CAPSULE_DOOR                 SM_ID_0
#define  CAPSULE_DOOR_SPEED           SM_SPEED_1
#define  CAPSULE_DOOR_OPEN_STEP_VAL   (900)
#define  CAPSULE_DOOR_CLOSE_STEP_VAL  (-950)


/* MODE */
#define CAPSULE_MODE_NONE   0x00
#define CAPSULE_MODE_INIT   0x01
#define CAPSULE_MODE_CLOSE  0x02
#define CAPSULE_MODE_OPEN   0x04


#define MAX_ON_TIME     300     /* 30sec @ 100ms */
#define DR_DETECT       0
#define DR_UNDETECT     1

typedef struct _capsule_
{
    /* STATUS */
    U8 Status;      // Open or Close

    /* MODE */
    U8 Mode;        // Init / Close / Open
    U8 InitStep;
    U8 OpenStep;
    U8 CloseStep;
    //U8 TestStep;

    /* Forced Close Capsule Timer */
    U32 CloseTime;

    U16 OpeningTimeIot;        /* @Sec */
    U16 OpeningTime;        /* @Sec */
} Capsule_T;

Capsule_T   Capsule;

void InitCapsule(void)
{
    Capsule.Status      = CAPSULE_CLOSE;

    Capsule.Mode        = 0;
    Capsule.InitStep    = 0;
    Capsule.OpenStep    = 0;
    Capsule.CloseStep   = 0;

    Capsule.OpeningTime     = 0;


    // FAUCET - UP/DOWN
    HAL_InitStepMotor();
    HAL_InitStepVal( CAPSULE_DOOR );
    HAL_SetCurrentStep( CAPSULE_DOOR, 0 );
    HAL_SetTargetStep( CAPSULE_DOOR, 0 );
    HAL_SetStepSpeed( CAPSULE_DOOR, CAPSULE_DOOR_SPEED );

    CapsuleInit();
}


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );

U8 GetCapsuleStatus(void)
{
    return Capsule.Status;
}

// 캡슐 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenCapsule(void)
{
    if( Capsule.Status == CAPSULE_OPEN
            || Capsule.Status == CAPSULE_OPENING )
    {
        return TRUE;
    }

    return FALSE;
}

static void SetCapsuleStatus(U8 mu8Status)
{
    Capsule.Status = mu8Status;
}

void CapsuleInit(void)
{
    SetMode( CAPSULE_MODE_INIT );
}

void CapsuleOpen(void)
{
    SetMode( CAPSULE_MODE_OPEN );
}

void CapsuleClose(void)
{
    SetMode( CAPSULE_MODE_CLOSE );
}

void CapsulePause(void)
{
    ClearMode( CAPSULE_MODE_OPEN );
    ClearMode( CAPSULE_MODE_CLOSE );

    HAL_StopMove( CAPSULE_DOOR );
}

static void SetMode(U8 mu8Mode )
{
    Capsule.Mode |= mu8Mode;
    if( mu8Mode == CAPSULE_MODE_OPEN )
    {
        ClearMode( CAPSULE_MODE_CLOSE );
    }
    else if( mu8Mode == CAPSULE_MODE_CLOSE )
    {
        ClearMode( CAPSULE_MODE_OPEN );
    }
}

static void ClearMode(U8 mu8Mode )
{
    Capsule.Mode &= ~mu8Mode;

    if( mu8Mode == CAPSULE_MODE_INIT )
    {
        Capsule.InitStep = 0;
    }
    else if( mu8Mode == CAPSULE_MODE_OPEN )
    {
        Capsule.CloseStep = 0;
    }
    else if( mu8Mode == CAPSULE_MODE_CLOSE )
    {
        Capsule.OpenStep = 0;
    }
}



static void ClearMode(U8 mu8Mode );
static U8 InitDoor(void);
static U8 OpenDoor(void);
static U8 CloseDoor(void);

void ControlCapsule(void)
{
    U8 mu8Ret = FALSE;


    /* Init */
    if( (Capsule.Mode & CAPSULE_MODE_INIT ) == CAPSULE_MODE_INIT )
    {
        mu8Ret = InitDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( CAPSULE_MODE_INIT );
            SetCapsuleStatus( CAPSULE_CLOSE );
        }
        else
        {
            SetCapsuleStatus( CAPSULE_CLOSING );
        }
        return ;
    }

    /* Open */
    if( (Capsule.Mode & CAPSULE_MODE_OPEN ) == CAPSULE_MODE_OPEN )
    {
        mu8Ret = OpenDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( CAPSULE_MODE_OPEN );
            SetCapsuleStatus( CAPSULE_OPEN );
        }
        else
        {
            SetCapsuleStatus( CAPSULE_OPENING );
        }
        return ;
    }

    /* Close */
    if( (Capsule.Mode & CAPSULE_MODE_CLOSE ) == CAPSULE_MODE_CLOSE )
    {
        mu8Ret = CloseDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( CAPSULE_MODE_CLOSE );
            SetCapsuleStatus( CAPSULE_CLOSE );
        }
        else
        {
            SetCapsuleStatus( CAPSULE_CLOSING );
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
    static U16 mu16DelayTime = 30;     /* 3sec@100ms */


    switch( Capsule.InitStep )
    {
        case 0:
            HAL_InitStepVal( CAPSULE_DOOR );
            HAL_SetTargetStep( CAPSULE_DOOR, CAPSULE_DOOR_OPEN_STEP_VAL );

            Capsule.InitStep++;
            break;

        case 1:
            if( HAL_IsDoneMoveStep( CAPSULE_DOOR ) == TRUE )
            {
                HAL_InitStepVal( CAPSULE_DOOR );
                HAL_SetTargetStep( CAPSULE_DOOR, CAPSULE_DOOR_CLOSE_STEP_VAL );
                Capsule.InitStep++;
            }
            break;

        case 2:
            if( HAL_IsDoneMoveStep( CAPSULE_DOOR ) == TRUE )
            {
                HAL_InitStepVal( CAPSULE_DOOR );
                Capsule.InitStep++;
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
    static U16 mu16DelayTime = 0;     /* @100ms */


    switch( Capsule.OpenStep )
    {
        case 0:
            HAL_StopMove( CAPSULE_DOOR );

            HAL_SetTargetStep( CAPSULE_DOOR, 0 );
            HAL_SetCurrentStep( CAPSULE_DOOR, 0 );
            HAL_SetTargetStep( CAPSULE_DOOR, CAPSULE_DOOR_OPEN_STEP_VAL );

            Capsule.OpenStep++;
            break;

        case 1:
            if( HAL_IsDoneMoveStep( CAPSULE_DOOR ) == TRUE )
                //|| HAL_GetInputValue( IN_CAPSULE_OUT ) == DETECTED )
            {
                //HAL_StopMove( CAPSULE_DOOR );
                Capsule.OpenStep++;
            }
            break;

        case 2:
            return TRUE;
    }

    return FALSE;

}

static U8 CloseDoor(void)
{
    static U16 mu16DelayTime = 0;     /* @100ms */

    switch( Capsule.CloseStep )
    {
        case 0:
            HAL_StopMove( CAPSULE_DOOR );
            
            HAL_SetTargetStep( CAPSULE_DOOR, 0 );
            HAL_SetCurrentStep( CAPSULE_DOOR, 0 );
            HAL_SetTargetStep( CAPSULE_DOOR, CAPSULE_DOOR_CLOSE_STEP_VAL );

            Capsule.CloseStep++;
            break;

        case 1:
            if( HAL_IsDoneMoveStep( CAPSULE_DOOR ) == TRUE )
                    //|| HAL_GetInputValue( IN_CAPSULE_IN ) == DETECTED )
            {
               // HAL_StopMove( CAPSULE_DOOR );
                HAL_InitStepVal( CAPSULE_DOOR );
                Capsule.CloseStep++;
            }
            break;

        case 2:
            return TRUE;

    }

    return FALSE;
}
