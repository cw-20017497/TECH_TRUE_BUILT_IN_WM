#include "hw.h"
#include "hal_step_motor.h"

#include "moving_faucet.h"
#include "hal_adc.h"

enum
{
    INIT_POWER_ON,
    INIT_WAIT_POWER_ON,

    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_DONE
};

enum
{
    READY_INIT,

    READY_MOVE_ZERO,
    READY_WAIT_ZERO,

    READY_DONE
};

enum
{
    MOVE_INIT,
    MOVE_FORWARD,

    MOVE_UP_DOWN,
    MOVE_UP_DOWN_DONE,

    MOVE_DONE
};

// UP-DOWN
#define  FAUCET_UP_DOWN        SM_ID_0
#define  FAUCET_UP_DOWN_SPEED  SM_SPEED_1
#define  FAUCET_UP_DOWN_INIT_STEP_VAL    (-15)


#define POWER_ON_TIME       10U     // 1sec @100ms
#define READY_TIME          70U     // 7sec @100ms
#define MANUAL_TIME         300U    // 30sec
MovingFaucet_T  Faucet;

void InitMovingFaucet(void)
{
    Faucet.Status     = MOVING_FAUCET_INITING;

    Faucet.Mode       = MOVING_FAUCET_MODE_INIT; 

    // INIT
    Faucet.InitStep    = 0;
    Faucet.PowerOnTime = POWER_ON_TIME; // @100ms, 1sec

    // READY
    Faucet.ReadyStep  = 0;
    Faucet.ReadyTime = READY_TIME; 

    // MOVE
    Faucet.MoveStep   = 0;

    Faucet.ManualTime = 0;


    // FAUCET - UP/DOWN
    HAL_InitStepVal( FAUCET_UP_DOWN );
    HAL_SetCurrentStep( FAUCET_UP_DOWN, FAUCET_UP_DOWN_END_STEP_VAL );
    HAL_SetTargetStep( FAUCET_UP_DOWN, FAUCET_UP_DOWN_END_STEP_VAL );
    HAL_SetStepSpeed( FAUCET_UP_DOWN, FAUCET_UP_DOWN_SPEED );
}

void GetFaucetData(MovingFaucet_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&Faucet, sizeof( Faucet ));
}


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );
FaucetStatus_T GetFaucetStatus(void)
{
    return Faucet.Status;
}

void SetFaucetStatus(FaucetStatus_T mu8Status)
{
    Faucet.Status = mu8Status;
}

U8 GetFaucetMode(void)
{
    return Faucet.Mode;
}


void ResetFaucetManualTime(void)
{
    Faucet.ManualTime = MANUAL_TIME;
}


void MovingFaucetInit(void)
{
    SetMode( MOVING_FAUCET_MODE_INIT );
}

void MovingFaucetReady(void)
{
    SetFaucetStatus( MOVING_FAUCET_GOING_READY );
    SetMode( MOVING_FAUCET_MODE_READY );
}

void MovingFaucetMove(void)
{
    SetFaucetStatus( MOVING_FAUCET_GOING_MOVE );
    SetMode( MOVING_FAUCET_MODE_MOVE );
}

void MovingFaucetStop(void)
{
    HAL_StopMove( FAUCET_UP_DOWN );
}

static void SetMode(U8 mu8Mode )
{
    // Set Mode
    Faucet.Mode |= mu8Mode;

    if( mu8Mode == MOVING_FAUCET_MODE_INIT )
    {
        ClearMode( MOVING_FAUCET_MODE_READY );
        ClearMode( MOVING_FAUCET_MODE_MOVE );
    }
    else if( mu8Mode == MOVING_FAUCET_MODE_READY )
    {
        ClearMode( MOVING_FAUCET_MODE_MOVE );
    }
    else if( mu8Mode == MOVING_FAUCET_MODE_MOVE )
    {
        ClearMode( MOVING_FAUCET_MODE_READY );
    }

    // Set Init Mode by case
}

static void ClearMode(U8 mu8Mode )
{
    Faucet.Mode &= ~mu8Mode;

    if( mu8Mode == MOVING_FAUCET_MODE_INIT )
    {
        Faucet.InitStep = 0;
    }
    else if( mu8Mode == MOVING_FAUCET_MODE_READY )
    {
        Faucet.ReadyStep = 0;
    }
    else if( mu8Mode == MOVING_FAUCET_MODE_MOVE )
    {
        Faucet.MoveStep = 0;
    }
}

static void ClearMode(U8 mu8Mode );
static U8 InitFaucet(void);
static U8 ReadyFaucet(void);
static U8 MoveFaucet(void);
void ControlMovingFaucet(void)
{
    U8 mu8Ret = FALSE;


    /* Init */
    if( (Faucet.Mode & MOVING_FAUCET_MODE_INIT ) == MOVING_FAUCET_MODE_INIT )
    {
        mu8Ret = InitFaucet();
        if( mu8Ret == TRUE )
        {
            ClearMode( MOVING_FAUCET_MODE_INIT );
            SetFaucetStatus( MOVING_FAUCET_READY );
        }
        return ;
    }

    /* READY */
    if( (Faucet.Mode & MOVING_FAUCET_MODE_READY ) == MOVING_FAUCET_MODE_READY )
    {
        mu8Ret = ReadyFaucet();
        if( mu8Ret == TRUE )
        {
            ClearMode( MOVING_FAUCET_MODE_READY );
            SetFaucetStatus( MOVING_FAUCET_READY );
        }
        return ;
    }

    /* MOVE */
    if( (Faucet.Mode & MOVING_FAUCET_MODE_MOVE ) == MOVING_FAUCET_MODE_MOVE )
    {
        mu8Ret = MoveFaucet();
        if( mu8Ret == TRUE )
        {
            ClearMode( MOVING_FAUCET_MODE_MOVE );
            SetFaucetStatus( MOVING_FAUCET_MOVE );
        }
        return ;
    }

}


static U8 InitFaucet(void)
{
    switch( Faucet.InitStep )
    {
        case INIT_POWER_ON:
            Faucet.PowerOnTime = POWER_ON_TIME;

            HAL_SetCurrentStep( FAUCET_UP_DOWN, FAUCET_UP_DOWN_END_STEP_VAL );
            HAL_SetTargetStep( FAUCET_UP_DOWN, FAUCET_UP_DOWN_END_STEP_VAL );

            Faucet.InitStep++;
            break;

        case INIT_WAIT_POWER_ON:
            if( Faucet.PowerOnTime != 0 )
            {
                Faucet.PowerOnTime--;
            }
            else
            {
                Faucet.InitStep++;
            }
            break;

        case INIT_MOVE_ZERO:
            // UP-DOWN
            HAL_SetHoldStep( FAUCET_UP_DOWN, 0 );
            HAL_SetTargetStep( FAUCET_UP_DOWN, FAUCET_UP_DOWN_INIT_STEP_VAL );

            Faucet.InitStep++;
            break;

        case INIT_WAIT_ZERO:
            if( HAL_IsDoneMoveStep( FAUCET_UP_DOWN ) == TRUE )
            {
                Faucet.InitStep++;

                HAL_InitStepVal( FAUCET_UP_DOWN );
            }
            break;

        case INIT_DONE :
            return TRUE;

        default :
            Faucet.InitStep = INIT_POWER_ON;
            break;
    }

    return FALSE;
}


static U8 ReadyFaucet(void)
{
    switch( Faucet.ReadyStep )
    {
        case READY_INIT:
            HAL_StopMove( FAUCET_UP_DOWN );

            Faucet.ReadyStep++;
            break;

        case READY_MOVE_ZERO:
            // UP-DOWN
            HAL_SetHoldStep( FAUCET_UP_DOWN, 0 );
            HAL_SetTargetStep( FAUCET_UP_DOWN, 0 );

            Faucet.ReadyStep++;
            break;

        case READY_WAIT_ZERO:
            if( HAL_IsDoneMoveStep( FAUCET_UP_DOWN ) == TRUE )
            {
                Faucet.ReadyStep++;
            }
            break;

        case READY_DONE :
            return TRUE;

        default :
            Faucet.ReadyStep = READY_INIT;
            break;
    }

    return FALSE;
}

static U8 MoveFaucet(void)
{
    switch( Faucet.MoveStep )
    {
        case MOVE_INIT:
            Faucet.MoveStep++;
            break;

            // 이동하면서 컵 위치 감지
        case MOVE_FORWARD:
            Faucet.MoveStep++;
            break;

        case MOVE_UP_DOWN:
            HAL_SetTargetStep( FAUCET_UP_DOWN, FAUCET_UP_DOWN_END_STEP_VAL );
            Faucet.MoveStep++;
            break;

        case MOVE_UP_DOWN_DONE:
            // 컵이 미감지 되어서 완전 이동한 상태
            // 완전 이동 완료?
            if( HAL_IsDoneMoveStep( FAUCET_UP_DOWN ) == TRUE )
            {
                Faucet.MoveStep++;
            }
            break;

        case MOVE_DONE:
            return TRUE;

        default :
            Faucet.MoveStep = MOVE_INIT;
            break;
    }

    return FALSE;
}



void ProcessMovingFaucet(void)
{
#if 0
    static U8 mu8Prev = FALSE;
    static U8 mu8Run = FALSE;
    U8 mu8Current = OFF;


    mu8Current = GetWaterOut();
    if( mu8Prev != mu8Current )
    {
        mu8Prev = mu8Current;
        if( mu8Current == FALSE )
        {
            Faucet.ReadyTime = READY_TIME;
            mu8Run = TRUE;
        }
    }

    if( GetWaterOut() == FALSE )
    {
        if( Faucet.ReadyTime != 0 )
        {
            Faucet.ReadyTime--;
        }

        if( Faucet.ReadyTime == 0 
                && mu8Run == TRUE)
        {
            mu8Run = FALSE;
            MovingFaucetReady();
            return ;
        }
    }


    // 수동 조작 해제 
    if( GetFaucetStatus() == MOVING_FAUCET_MANUAL )
    {
        if( GetWaterOut() == FALSE )
        {
            if( Faucet.ManualTime != 0 )
            {
                Faucet.ManualTime--;
            }

            if( Faucet.ManualTime == 0 )
            {
                MovingFaucetReady();
            }
        }
        else
        {
            Faucet.ManualTime = MANUAL_TIME;
        }
    }
#endif
}


