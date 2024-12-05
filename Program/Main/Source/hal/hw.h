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
#define  P_TEM_ONOFF              P7.5
#define  TURN_ON_TEM()            do{ P_TEM_ONOFF = 1; }while(0)
#define  TURN_OFF_TEM()           do{ P_TEM_ONOFF = 0; }while(0)
#define  GET_STATUS_TEM()         ( P_TEM_ONOFF )



#endif /* __HW__H__ */

