#include "hw.h"
#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "front.h"

#include "error.h"
#include "display_lib.h"
#include "display_clock.h"
#include "process_display.h"

#include "timer.h"
#include "timer_id.h"

#include "key_event_lib.h"

#include "grinding.h"
#include "hal_led.h"




static void DispOnOff(U8 mu8OnOff);

static U8 SelOn(void);
static U8 SelOff(void);
static U8 SelOnOff(void);
static U8 OnOffLed(void);

static U8 SelLedDuty(void);

// 일반 모드 리스트
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,        2sec,           3sec,      5sec,           Pop,           TS */
#if CONFIG_TEST_LED_KEY
    { K_ONOFF,        SelLedDuty,      NULL,       NULL,      NULL,           NULL,          NULL },
#else
    { K_ONOFF,        SelOff,      NULL,       SelOn,      NULL,           NULL,          NULL },
#endif
};

U8 IsValidNormalKeyCondition(U32 mu32Key)
{
    return TRUE;
}

// 모드에 따라 메멘토 타임쇼트, 일반 3가지를 반환
void* GetNormalKeyEventList(void)
{
    return KeyEventList;
}

// 모드에 따라 메멘토 타임쇼트, 일반 3가지를 반환
U8 GetNormalKeyEventListSize(void)
{
    return ( sizeof(KeyEventList) / sizeof(KeyEventList_T) );
}


static U8 SelOn(void)
{
    if( GetGrindingStatus() == GRINDING_CLOSE
            || GetGrindingStatus() == GRINDING_CLOSING
      )
    {
        OpenGrinding();
    }

    return 0;
}

static U8 SelOff(void)
{
    if( GetGrindingStatus() == GRINDING_OPEN
            || GetGrindingStatus() == GRINDING_OPENING
      )
    {
        CloseGrinding();
    }

    return 0;
}

static U8 SelOnOff(void)
{
    if( GetGrindingStatus() == GRINDING_OPEN
            || GetGrindingStatus() == GRINDING_OPENING
      )
    {
        CloseGrinding();
    }
    else if( GetGrindingStatus() == GRINDING_CLOSE
            || GetGrindingStatus() == GRINDING_CLOSING
      )
    {
        OpenGrinding();
    }

    return 0;
}


extern U8 led_onoff;
static U8 OnOffLed(void)
{

    led_onoff = !led_onoff;

    return 0;
}


extern U8 dbg_duty;
static U8 SelLedDuty(void)
{

    dbg_duty += 10;

    if( dbg_duty > 100 )
    {
        dbg_duty = 10;
    }

    return 0;
}
