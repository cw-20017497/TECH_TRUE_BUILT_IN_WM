#ifndef __HAL_LED_H__
#define __HAL_LED_H__



#define DEFAULT_ONOFF_DUTY          10U
#define DEFAULT_DIMMING_DUTY        10U
#define DEFAULT_ONOFF_DUTY_DARK     10U
#define DEFAULT_DIMMING_DUTY_DARK   10U
typedef enum
{
    // MATRIX LED
    SEG_1,
    SEG_2,
    SEG_3,
    SEG_4,
    SEG_5,
    SEG_6,
    SEG_7,
    SEG_8,
    SEG_9,
    SEG_10,
    SEG_11,
    SEG_12,

    LED_ALL,

    MAX_LED_NUM
} LedId_T;
#define MAX_LED_BUF        2               /* 2bytes x 8bits = 16bits */

void HAL_InitLed(void);

// LED ON/OFF 
void HAL_TurnOnOffLEDAll(U8 mu8OnOff);
void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff);
void HAL_TurnOnOffLEDEx(LedId_T led, U8 mu8OnOff);
void HAL_GetLedOnOffStatus(U8 *pBuf);


// LED DUTY 
void HAL_SetLedDuty(U8 mu8Duty);
U8 HAL_GetLedDuty(void);

// DIMMING ON/OFF
void HAL_DimmingLedAll(U8 mu8OnOff);
void HAL_DimmingLed(LedId_T led, U8 mu8OnOff);
void HAL_GetLedDimmingStatus(U8 *pBuf);

// DIMMING DUTY 
void HAL_SetLedDimmingDuty(U8 mu8Duty);
U8 HAL_GetLedDimmingDuty(void);


U8 HAL_IsUpdateLed(void);
void HAL_UpdateLed(void);

#endif /* __HAL_LED_H__ */
