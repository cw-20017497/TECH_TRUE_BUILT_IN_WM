#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_intc.h"
#include "r_cg_macrodriver.h"


/* HW
 *
 */


/* Watchdog timer reset */
#define	RESET_WDT()					R_WDT_Restart()


/* GRINDING MOTOR  ONOFF */
#define  P_GRINDING_MOTOR_ONOFF             P7.1
#define  TURN_ON_GRINDING_MOTOR()           do{ P_GRINDING_MOTOR_ONOFF = 1; }while(0)
#define  TURN_OFF_GRINDING_MOTOR()          do{ P_GRINDING_MOTOR_ONOFF = 0; }while(0)
#define  GET_STATUS_GRINDING_MOTOR()        ( P_GRINDING_MOTOR_ONOFF )


/* DOOR MOTOR ONOFF */
#define  P_DOOR_MOTOR_ONOFF             P7.0
#define  TURN_ON_DOOR_MOTOR()           do{ P_DOOR_MOTOR_ONOFF = 1; }while(0)
#define  TURN_OFF_DOOR_MOTOR()          do{ P_DOOR_MOTOR_ONOFF = 0; }while(0)
#define  GET_STATUS_DOOR_MOTOR()        ( P_DOOR_MOTOR_ONOFF )


/* SMPS STAND BY ONOFF */
#define  P_SMPS_ONOFF             P7.4
#define  TURN_ON_SMPS_ONOFF()           do{ P_SMPS_ONOFF = 1; }while(0)
#define  TURN_OFF_SMPS_ONOFF()          do{ P_SMPS_ONOFF = 0; }while(0)
#define  GET_STATUS_SMPS_ONOFF()        ( P_SMPS_ONOFF )

/* MOTOR_POSITION 1 - open*/
#define  P_MOTOR_OPEN             P7.2 
#define  GET_STATUS_MOTOR_OPEN()  ( P_MOTOR_OPEN )


/* MOTOR_POSITION 2 - close*/
#define  P_MOTOR_CLOSE             P7.3
#define  GET_STATUS_MOTOR_CLOSE()  ( P_MOTOR_CLOSE )


/* TEM ONOFF */
#define  P_TEM_ONOFF                P7.5
#define  TURN_ON_TEM()              do{ P_TEM_ONOFF = 1; }while(0)
#define  TURN_OFF_TEM()             do{ P_TEM_ONOFF = 0; }while(0)
#define  GET_STATUS_TEM()           ( P_TEM_ONOFF )

// BLDC FREQUENCY GENERATOR
#define BLDC_MOTOR_FG               P3.0
#define GET_BLDC_MOTOR_FG()         ( BLDC_MOTOR_FG )

#define BLDC_MOTOR_BRK              P14.0
#define TURN_ON_BLDC_MOTOR_BRK()    do{ BLDC_MOTOR_BRK = 0; }while(0) 
#define TURN_OFF_BLDC_MOTOR_BRK()   do{ BLDC_MOTOR_BRK = 1; }while(0) 


#define BLDC_MOTOR_FR               P12.0
#define TURN_ON_BLDC_MOTOR_FR()     do{ BLDC_MOTOR_FR = 0; }while(0)        // CW
#define TURN_OFF_BLDC_MOTOR_FR()    do{ BLDC_MOTOR_FR = 1; }while(0)        // CCW

#define BLDC_MOTOR_PWM              P4.1    


/* STEP MOTOR - MOVING FAUCET UP/DOW  */
#define  P_STEP_MOTOR_FAUCET_4      P1.0  
#define  P_STEP_MOTOR_FAUCET_3      P1.1  
#define  P_STEP_MOTOR_FAUCET_2      P1.2  
#define  P_STEP_MOTOR_FAUCET_1      P1.3  

#define  TURN_ON_STEP_MOTOR_FAUCET_1()     do { P_STEP_MOTOR_FAUCET_1 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_FAUCET_2()     do { P_STEP_MOTOR_FAUCET_2 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_FAUCET_3()     do { P_STEP_MOTOR_FAUCET_3 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_FAUCET_4()     do { P_STEP_MOTOR_FAUCET_4 = 1; }while(0)

#define  TURN_OFF_STEP_MOTOR_FAUCET_1()     do { P_STEP_MOTOR_FAUCET_1 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_FAUCET_2()     do { P_STEP_MOTOR_FAUCET_2 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_FAUCET_3()     do { P_STEP_MOTOR_FAUCET_3 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_FAUCET_4()     do { P_STEP_MOTOR_FAUCET_4 = 0; }while(0)

#define  GET_STEP_MOTOR_FAUCET_4()   (P_STEP_MOTOR_FAUCET_4)
#define  GET_STEP_MOTOR_FAUCET_3()   (P_STEP_MOTOR_FAUCET_3)
#define  GET_STEP_MOTOR_FAUCET_2()   (P_STEP_MOTOR_FAUCET_2)
#define  GET_STEP_MOTOR_FAUCET_1()   (P_STEP_MOTOR_FAUCET_1)


/* CAPSULE IN */
#define  P_CAPSULE_IN             P1.4
#define  GET_STATUS_CAPSULE_IN()  ( P_CAPSULE_IN )


/* CAPSULE OUT */
#define  P_CAPSULE_OUT             P1.5
#define  GET_STATUS_CAPSULE_OUT()  ( P_CAPSULE_OUT )

#endif /* __HW__H__ */

