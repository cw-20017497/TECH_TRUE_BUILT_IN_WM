#include "hw.h"
#include "hal_input.h"



static U8   HAL_GetDoorOpen(void);
static U8   HAL_GetDoorClose(void);

static U8   HAL_GetCapsuleOut(void);
static U8   HAL_GetCapsuleIn(void);

typedef U8 (*Action_T)(void);
typedef struct _input_config_
{
    U8  prevVal; 
    U8  curVal;

    U16 delay;
    U16 setupDelay;

    Action_T    pfGetStatus;    
} InputConfig_T;

InputConfig_T inputList[ IN_MAX_NUM ] = 
{ 
    { 1,      1,      INPUT_DELAY_100MS,   INPUT_DELAY_100MS,    HAL_GetDoorOpen  },
    { 1,      1,      INPUT_DELAY_100MS,   INPUT_DELAY_100MS,    HAL_GetDoorClose },
    { 1,      1,      INPUT_DELAY_100MS,   INPUT_DELAY_100MS,    HAL_GetCapsuleOut  },
    { 1,      1,      INPUT_DELAY_100MS,   INPUT_DELAY_100MS,    HAL_GetCapsuleIn },
}; 

void InitInputConfDetectTime(void)
{
    // do nothing....
}

void SetInputConfDetectTime(U8 mu8Id, U8 mu16DetectTime)
{
    inputList[ mu8Id ].setupDelay = mu16DetectTime; 
}


static U8   HAL_GetDoorOpen(void)
{
    return (U8)GET_STATUS_MOTOR_OPEN();
}

static U8   HAL_GetDoorClose(void)
{
    return (U8)GET_STATUS_MOTOR_CLOSE();
}


static U8   HAL_GetCapsuleOut(void)
{
    return (U8)GET_STATUS_MOTOR_OPEN();
}

static U8   HAL_GetCapsuleIn(void)
{
    return (U8)GET_STATUS_MOTOR_CLOSE();
}


U8 HAL_GetInputValue(U8 mu8Id)
{
    if( mu8Id >= IN_MAX_NUM )
    {
        return 0;
    }

    return inputList[ mu8Id ].curVal;
}


void HAL_ProcessInput(void)
{
    U8 i;
    Action_T    pGet = NULL;
    U8  mu8Val;


    for( i = 0; i < IN_MAX_NUM ; i++ )
    {
        pGet = inputList[ i ].pfGetStatus;
        if( pGet != NULL )
        {
            // Read port value
            mu8Val = pGet();
            if( inputList[ i ].prevVal != mu8Val )
            {
                // Update delay time...
                inputList[ i ].delay = inputList[ i ].setupDelay;
                inputList[ i ].prevVal = mu8Val;
            }
            else
            {
                inputList[ i ].delay--;
                if( inputList[ i ].delay == 0 )
                {
                    // update new value..
                    inputList[ i ].curVal = mu8Val;
                }
            }
        }
    }
}
