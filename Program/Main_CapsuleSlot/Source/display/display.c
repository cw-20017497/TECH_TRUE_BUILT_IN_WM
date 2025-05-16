#include "display.h"
#include "hal_led.h"
#include "display_lib.h"
#include "display_clock.h"
#include "front.h"


void TurnOffAllLED(void)
{
    HAL_TurnOnOffLEDAll( OFF );
    HAL_DimmingLedAll( OFF );
}

void TurnOnAllLED(void)
{
    HAL_TurnOnOffLEDAll( ON );
    HAL_DimmingLedAll( OFF );
}

#if CONFIG_TEST_LED 
U8 dbg_duty_on           = 10;
U8 dbg_duty_dimming      = 40;

U8 dbg_duty_dark         = 50;
U8 dbg_duty_dimming_dark = 20;
#endif

U8 dbg_duty_onoff = FALSE;
void DispLedDuty(void)
{
#if CONFIG_TEST_LED
    if( dbg_duty_onoff == TRUE )
    {
        HAL_SetLedDuty( dbg_duty_dark );
        HAL_SetLedDimmingDuty( dbg_duty_dimming_dark );
    }
    else
    {
        HAL_SetLedDuty( dbg_duty_on );
        HAL_SetLedDimmingDuty( dbg_duty_dimming );
    }
#else
    if( dbg_duty_onoff == TRUE )
    {
        HAL_SetLedDuty( DEFAULT_ONOFF_DUTY_DARK );
        HAL_SetLedDimmingDuty( DEFAULT_DIMMING_DUTY_DARK );
    }
    else
    {
        HAL_SetLedDuty( DEFAULT_ONOFF_DUTY );
        HAL_SetLedDimmingDuty( DEFAULT_DIMMING_DUTY );
    }
#endif
}


