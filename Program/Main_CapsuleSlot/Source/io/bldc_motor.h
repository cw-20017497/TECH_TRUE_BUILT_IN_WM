#ifndef __BLDC_MOTOR_H__
#define __BLDC_MOTOR_H__

#include "prj_type.h"

////////////////////////////////////////////////////////////////////
// MODEL : BKL10S
//
// PWM : recommended frequency : 20kHz ( range : 15kHz ~ 30kHz )
//
////////////////////////////////////////////////////////////////////

void InitBldcMotor(void);

void RegisterBldcMotorFeedback( void (*pUserISR)(U16 val) );

#define MAX_BLDC_OUTPUT_VOLT     12.0f

#define MOTOR_DIR_CW        0
#define MOTOR_DIR_CCW       1
void StartBldcMotor(U8 dir, F32 output_volt );
void StopBldcMotor(U8 use_brk);
void BreakBldcMotor( U8 bldc_break );
U16 GetBldcCurrentRpm(void);
void ProcessBldcMotor(void);

#endif /* __BLDC_MOTOR_H__ */
