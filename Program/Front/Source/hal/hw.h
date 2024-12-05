#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_macrodriver.h"


/* Watchdog timer reset */
#define	RESET_WDT()					R_WDT_Restart()

#define P_KEY_1                         P1.5

/* LED */
#define  P_LED_01                       P1.7
#define  P_LED_02                       P1.6
#define  P_LED_03                       P5.0
#define  P_LED_04                       P5.1
#define  P_LED_05                       P7.0
#define  P_LED_06                       P3.0
#define  P_LED_07                       P7.2

#define  P_LED_08                       P7.1
#define  P_LED_09                       P7.4
#define  P_LED_10                       P7.3
#define  P_LED_11                       P3.1
#define  P_LED_12                       P7.5


#endif /* __HW__H__ */

