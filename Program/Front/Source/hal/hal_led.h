#ifndef __HAL_LED_H__
#define __HAL_LED_H__

// PIN2PIN leds count = 35
typedef enum
{
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
#define MAX_LED    2       /* 2bytes * 8bits = 16 */

void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff);

void HAL_SetOnOffLED(U8 *pLeds, U8 mu8Size );
void HAL_SetOnOffDuty(U8 mu8Duty);

void HAL_SetDimmingLED(U8 *pLeds, U8 mu8Size);
void HAL_SetDimmingDuty(U8 mu8Duty);

#endif /* __HAL_LED_H__ */
