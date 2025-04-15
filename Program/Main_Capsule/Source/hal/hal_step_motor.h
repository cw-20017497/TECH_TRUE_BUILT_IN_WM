#ifndef __HAL_STEP_MOTOR_H__
#define __HAL_STEP_MOTOR_H__
#include "prj_type.h"


#define SM_ID_0         0   // FAUCET UP-DOWN
#define MAX_SM_ID       1

/* @at 2ms, */
#define SM_SPEED_0          0    /* 1ms = 1000 pps*/
#define SM_SPEED_1          1    /* 2ms = 500 pps */
#define SM_SPEED_2          2    /* 3ms = 333 pps  */
#define MAX_SM_SPEED_NUM    3    


typedef short int   sm_step_t;
typedef short int   sm_speed_t;


void HAL_InitStepMotor(void);
void HAL_InitStepVal( U8 _id );

void HAL_SetInActive( U8 _id, U8 _in_active );

void      HAL_SetTargetStep( U8 _id, sm_step_t step );
sm_step_t HAL_GetTargetStep( U8 _id );

void      HAL_SetCurrentStep( U8 _id, sm_step_t step );
sm_step_t HAL_GetCurrentStep( U8 _id );

U8 OffsetTargetStep(U8 id, sm_step_t offset, sm_step_t max);

void HAL_SetHoldStep( U8 _id, sm_step_t step );

void       HAL_SetStepSpeed( U8 _id, sm_speed_t speed );
sm_speed_t HAL_GetStepSpeed( U8 _id );

void  HAL_SetStepSpeedCount( U8 _id, U8 _cnt );
U8     HAL_GetStepSpeedCount( U8 _id );
void  HAL_DecStepSpeedCount( U8 _id );

U8    HAL_IsDoneMoveStep( U8 _id );
void  HAL_StopMove( U8 id );
U8    HAL_SetTargetStepDone( U8 id, sm_step_t step );


void  HAL_MoveStep( U8 _id );
void  HAL_ControlStepMotor( U8  _id );


#endif /* __HAL_STEP_MOTOR_H__ */
