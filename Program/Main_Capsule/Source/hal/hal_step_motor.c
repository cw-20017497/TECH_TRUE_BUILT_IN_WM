#pragma interrupt INTTM04 StepMotorTimerInterrupt

/* 1ms */

#include "hw.h"
#include "hal_step_motor.h"

/* MACRO - TIMER 04 */
#define INTS_BYTE() unsigned char _ints_byte
#define MASK_STEP_MOTOR_TIMER   TMMK04
#define DISABLE_INT_MASK_MOTOR_TIMER()     \
    do { \
        MASK_STEP_MOTOR_TIMER = 1; \
    }while(0)

#define ENABLE_INT_MASK_MOTOR_TIMER()          \
    do { \
        MASK_STEP_MOTOR_TIMER = 0; \
    }while(0)

#define ENTER_CRITICAL_SECTION_STEP_MOTOR() \
    do{ \
        _ints_byte = MASK_STEP_MOTOR_TIMER; \
        DISABLE_INT_MASK_MOTOR_TIMER(); \
    }while(0)

#define EXIT_CRITICAL_SECTION_STEP_MOTOR()  \
    do{ \
        MASK_STEP_MOTOR_TIMER = _ints_byte; \
    } while(0)


/* 
 * SPEED 카운트 1,2는 동일... 추후 필요시 사용
 * SM_HOLD_STEP_x 은 SPEED COUNT에 맞춰서 설정 
 */
U8 dbg_the_speed_cnt_1 = 1;
U8 dbg_the_speed_cnt_2 = 2;
#define SM_SPEED_0_COUNT  0    /* @1ms */
#define SM_SPEED_1_COUNT  1    /* @2ms */
#define SM_SPEED_2_COUNT  2    /* @3ms */

#define SM_HOLD_STEP_0   10    /* 10ms @100ms */
#define SM_HOLD_STEP_1   10    /* 10ms @100ms */


typedef struct _hal_sm_
{
    sm_step_t   target;
    sm_step_t   current;

    /* SPEED */
    sm_speed_t  speed;
    U8  speed_cnt;

    /* DLETA STEP */
    sm_step_t   move_step;

    /* HOLD STEP */
    sm_step_t   hold_start_step;
    sm_step_t   hold_stop_step;

    /* Step Motor Phase  */
    I8  current_mode;
    I8  prev_mode;

    /* TEST MODE */
    U8 in_active;   // FCT Test Mode
} hal_sm_info_t;

LOCAL hal_sm_info_t     sm_info[ MAX_SM_ID ];

#define PHASE_STEP_NUM      8   
static U8   gu8PhaseTable[ MAX_SM_ID ][ PHASE_STEP_NUM ]    = 
{
    //{ 1, 2, 3, 4, 5, 6, 7, 8 },     // 1-2상 
    //{ 8, 7, 6, 5, 4, 3, 2, 1 },     // 1-2상 
    { 1, 2, 3, 4, 1, 2, 3, 4 },     // 2상
//    { 4, 3, 2, 1, 4, 3, 2, 1 }      // 2상
};

static U8   gu8HoldStepTable[ MAX_SM_SPEED_NUM ] = 
{   
    SM_HOLD_STEP_0,
    SM_HOLD_STEP_1,
};



void HAL_InitStepMotor(void)
{
    U8 i;

    
    for( i = 0; i < MAX_SM_ID ; i++ )
    {
        sm_info[ i ].target = 0;
        sm_info[ i ].current = 0;

        sm_info[ i ].speed  = 0;
        sm_info[ i ].speed_cnt  = 0;

        sm_info[ i ].move_step = 0;

        sm_info[ i ].hold_start_step = 0;
        sm_info[ i ].hold_stop_step = 0;

        sm_info[ i ].current_mode = 0;

        sm_info[ i ].in_active = 0;
    }

    R_TAU0_Channel4_Start();   // Start Timer-04
}


void HAL_InitStepVal( U8 _id )
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    sm_info[ _id ].target       = 0;
    sm_info[ _id ].current      = 0;
    EXIT_CRITICAL_SECTION_STEP_MOTOR();
}


void HAL_SetInActive( U8 _id, U8 _in_active )
{
    sm_info[ _id ].in_active = _in_active;
}

 
void HAL_SetTargetStep( U8 _id, sm_step_t step )
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    sm_info[ _id ].target = step;
    EXIT_CRITICAL_SECTION_STEP_MOTOR();
}

sm_step_t       HAL_GetTargetStep( U8 _id )
{
    return sm_info[ _id ].target;
}

void    HAL_SetCurrentStep( U8 _id, sm_step_t step )
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    sm_info[ _id ].current = step;
    EXIT_CRITICAL_SECTION_STEP_MOTOR();
}

sm_step_t HAL_GetCurrentStep( U8 _id )
{
    sm_step_t   step;
    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    step = sm_info[ _id ].current;
    EXIT_CRITICAL_SECTION_STEP_MOTOR();

    return step;
}

U8 OffsetTargetStep(U8 id, sm_step_t offset, sm_step_t max)
{
    U8 ret = TRUE;
    sm_step_t target;


    target = HAL_GetTargetStep(id) + offset;
    if( target > max )
    {
        target = max;
        ret = FALSE;    // error
    }
    else if( target < 0 )
    {
        target = 0;
        ret = FALSE;    // error
    }

    HAL_StopMove( id );
    HAL_SetTargetStep( id, target );

    return ret;
}

void HAL_SetHoldStep( U8 _id, sm_step_t step )
{
    INTS_BYTE();
    sm_speed_t mSpeed;
    sm_step_t mHoldStep;


    mSpeed = sm_info[ _id ].speed;
    mHoldStep    = gu8HoldStepTable[ mSpeed ];

    /* ENTER CRITICAL SECTION */
    ENTER_CRITICAL_SECTION_STEP_MOTOR();

    sm_info[ _id ].hold_start_step  = mHoldStep;  /* about 0.5sec */
    sm_info[ _id ].hold_stop_step   = mHoldStep;  /* aobut 0.5esc */


    EXIT_CRITICAL_SECTION_STEP_MOTOR();
    /* ENTER CRITICAL SECTION */
}

static void HAL_SetDeltaStep( U8 _id, sm_step_t _step )
{
    INTS_BYTE();
    sm_speed_t mSpeed;
    sm_step_t mHoldStep;


    mSpeed = sm_info[ _id ].speed;
    mHoldStep = gu8HoldStepTable[ mSpeed ];

    /* ENTER CRITICAL SECTION */
    ENTER_CRITICAL_SECTION_STEP_MOTOR();

    sm_info[ _id ].move_step        =   _step;
    sm_info[ _id ].hold_start_step  =   mHoldStep;  /* about 0.5sec */
    sm_info[ _id ].hold_stop_step   =   mHoldStep;  /* aobut 0.5esc */

    EXIT_CRITICAL_SECTION_STEP_MOTOR();
    /* ENTER CRITICAL SECTION */

}

static sm_step_t   HAL_GetDeltaStep( U8 _id )
{
    INTS_BYTE();
    sm_step_t   step;


    /* ENTER CRITICAL SECTION */
    ENTER_CRITICAL_SECTION_STEP_MOTOR();

    step = sm_info[ _id ].move_step;

    EXIT_CRITICAL_SECTION_STEP_MOTOR();
    /* ENTER CRITICAL SECTION */

    return step;
}


void    HAL_SetStepSpeed( U8 _id, sm_speed_t    speed )
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    sm_info[ _id ].speed = speed;
    EXIT_CRITICAL_SECTION_STEP_MOTOR();
}

sm_speed_t  HAL_GetStepSpeed( U8 _id )
{
    return sm_info[ _id ].speed;
}


void HAL_SetStepSpeedCount( U8 _id, U8 _cnt )
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    sm_info[ _id ].speed_cnt = _cnt;
    EXIT_CRITICAL_SECTION_STEP_MOTOR();
}

U8  HAL_GetStepSpeedCount( U8 _id )
{
    return sm_info[ _id ].speed_cnt;
}

void    HAL_DecStepSpeedCount( U8 _id )
{
    if( sm_info[ _id ].speed_cnt != 0 )
    {
        sm_info[ _id ].speed_cnt--;
    }
}


U8  HAL_IsDoneMoveStep( U8 _id )
{
    sm_step_t   target;
    sm_step_t   current;

    INTS_BYTE();

    ENTER_CRITICAL_SECTION_STEP_MOTOR();
    target = HAL_GetTargetStep( _id );
    current = HAL_GetCurrentStep( _id );
    EXIT_CRITICAL_SECTION_STEP_MOTOR();

    if( target == current )
    {
        return TRUE;
    }

    return FALSE;
}

void    HAL_StopMove( U8 id )
{
    sm_step_t   current;
    INTS_BYTE();


    ENTER_CRITICAL_SECTION_STEP_MOTOR();

    current = HAL_GetCurrentStep( id );
    HAL_SetTargetStep( id, current );
    HAL_SetDeltaStep( id, 0 );

    EXIT_CRITICAL_SECTION_STEP_MOTOR();
}


U8  HAL_SetTargetStepDone( U8 id, sm_step_t step )
{
    HAL_SetTargetStep( id, step );

    return HAL_IsDoneMoveStep( id );
}



void HAL_ControlStepMotor( U8 _id )
{
    sm_step_t val;
    sm_step_t target;
    sm_step_t current;
    INTS_BYTE();


    ENTER_CRITICAL_SECTION_STEP_MOTOR();

    val     = HAL_GetDeltaStep( _id );
    target  = HAL_GetTargetStep( _id );
    current = HAL_GetCurrentStep( _id );

    EXIT_CRITICAL_SECTION_STEP_MOTOR();

    /* CALC STEP, TARGET - CURRENT */
    if( val == 0 )
    {
        if( target != current )
        {
            val = target - current;
            HAL_SetDeltaStep( _id, val );
        }
    }

    /* CONTROL STEP */
    if( HAL_GetStepSpeed( _id ) == SM_SPEED_1 )
    {
        if( HAL_GetStepSpeedCount( _id ) != 0 )
        {
            HAL_DecStepSpeedCount( _id );
            return ;
        }
        else
        {
            HAL_SetStepSpeedCount( _id, SM_SPEED_1_COUNT );
        }
    }
    else if( HAL_GetStepSpeed( _id ) == SM_SPEED_2 )
    {
        if( HAL_GetStepSpeedCount( _id ) != 0 )
        {
            HAL_DecStepSpeedCount( _id );
            return ;
        }
        else
        {
            HAL_SetStepSpeedCount( _id, SM_SPEED_2_COUNT );
        }
    }

    HAL_MoveStep( _id );
}

static void     HAL_ControlFaucetUpDown( U8 mode );
void    HAL_MoveStep( U8 _id )
{
    hal_sm_info_t       *p_info = NULL;
    U8 val = 0;


    p_info = &sm_info[ _id ];

    /* START HOLD */
    if( p_info->hold_start_step > 0 )
    {
        p_info->hold_start_step--;
        val = gu8PhaseTable[ _id ][ p_info->current_mode ];
    }
    else if( p_info->move_step == 0 )
    {
        /* STOP HOLD */
        if( p_info->hold_stop_step > 0 )
        {
            p_info->hold_stop_step--;
            val = gu8PhaseTable[ _id ][ p_info->current_mode ];
        }
        else
        {
            /* STOP */
            val = 0x00;
        }
    }
    /* MOVE FORWARD */
    else if( p_info->move_step > 0 )
    {
        /* OPEN */
        p_info->move_step--;
        p_info->current++;

        p_info->prev_mode = p_info->current_mode;
        p_info->current_mode++;
        if( p_info->current_mode >= PHASE_STEP_NUM )
        {
            p_info->current_mode    = 0;
        }

        val = gu8PhaseTable[ _id ][ p_info->current_mode ];
    }
    /* MOVE BACKWARD */
    else if( p_info->move_step < 0 )
    {
        /* CLOSE */
        p_info->move_step++;
        p_info->current--;

        p_info->prev_mode = p_info->current_mode;
        p_info->current_mode--;
        if( p_info->current_mode < 0 )
        {
            p_info->current_mode = PHASE_STEP_NUM - 1;
        }

        val = gu8PhaseTable[ _id ][ p_info->current_mode ];
    }

    if( _id == SM_ID_0 )
    {
        HAL_ControlFaucetUpDown( val );
    }
    
}


static void     HAL_ControlFaucetUpDown( U8 mode )
{
    switch( mode )
    {
        case 0:
            P_STEP_MOTOR_FAUCET_1  = 0;
            P_STEP_MOTOR_FAUCET_2  = 0;
            P_STEP_MOTOR_FAUCET_3  = 0;
            P_STEP_MOTOR_FAUCET_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_FAUCET_1  = 1;
            P_STEP_MOTOR_FAUCET_2  = 1;
            P_STEP_MOTOR_FAUCET_3  = 0;
            P_STEP_MOTOR_FAUCET_4  = 0;
            break;
        case 2:
            P_STEP_MOTOR_FAUCET_1  = 0;
            P_STEP_MOTOR_FAUCET_2  = 1;
            P_STEP_MOTOR_FAUCET_3  = 1;
            P_STEP_MOTOR_FAUCET_4  = 0;
            break;
        case 3:
            P_STEP_MOTOR_FAUCET_1  = 0;
            P_STEP_MOTOR_FAUCET_2  = 0;
            P_STEP_MOTOR_FAUCET_3  = 1;
            P_STEP_MOTOR_FAUCET_4  = 1;
            break;
        case 4:
            P_STEP_MOTOR_FAUCET_1  = 1;
            P_STEP_MOTOR_FAUCET_2  = 0;
            P_STEP_MOTOR_FAUCET_3  = 0;
            P_STEP_MOTOR_FAUCET_4  = 1;
            break;

        default:
            P_STEP_MOTOR_FAUCET_1  = 0;
            P_STEP_MOTOR_FAUCET_2  = 0;
            P_STEP_MOTOR_FAUCET_3  = 0;
            P_STEP_MOTOR_FAUCET_4  = 0;
            break;
    }
}


/* INTERRUPT SERVICE ROUTIN */
static void StepMotorTimerInterrupt(void)
{
    if( sm_info[ SM_ID_0 ].in_active == 0 )
    {
        HAL_ControlStepMotor( SM_ID_0 );   // faucet up/down
    }

    //if( sm_info[ SM_ID_1 ].in_active == 0 )
    //{
    //    HAL_ControlStepMotor( SM_ID_1 );   // faucet forward
    //}
}
