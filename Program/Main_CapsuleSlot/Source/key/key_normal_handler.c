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

#include "capsule.h"
#include "hal_led.h"




static void DispOnOff(U8 mu8OnOff);

static U8 SelOn(void);
static U8 SelOff(void);
static U8 SelOnOff(void);
static U8 OnOffLed(void);

static U8 SelLedDuty(void);

// step motor
static U8 SelStepDir(void);
static U8 SelStepStart(void);
static U8 SelStepStop(void);

// 일반 모드 리스트
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,        2sec,           3sec,      5sec,           Pop,           TS */
#if CONFIG_TEST_LED_KEY
    { K_ONOFF,        SelLedDuty,      NULL,       NULL,      NULL,           NULL,          NULL },
#else
    //{ K_ONOFF,        SelOff,      NULL,       SelOn,      NULL,           NULL,          NULL },
    { K_ONOFF,        SelStepDir,      NULL,       NULL,      NULL,           SelStepStop,          SelStepStart },
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
    if( GetCapsuleStatus() == CAPSULE_CLOSE
            || GetCapsuleStatus() == CAPSULE_CLOSING
      )
    {
        OpenCapsule();
    }

    return 0;
}

static U8 SelOff(void)
{
    if( GetCapsuleStatus() == CAPSULE_OPEN
            || GetCapsuleStatus() == CAPSULE_OPENING
      )
    {
        CloseCapsule();
    }

    return 0;
}

static U8 SelOnOff(void)
{
    if( GetCapsuleStatus() == CAPSULE_OPEN
            || GetCapsuleStatus() == CAPSULE_OPENING
      )
    {
        CloseCapsule();
    }
    else if( GetCapsuleStatus() == CAPSULE_CLOSE
            || GetCapsuleStatus() == CAPSULE_CLOSING
      )
    {
        OpenCapsule();
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



// step motor
// 한번 길게 누르면 정방향
// 빠르게 두번 누르면 역방향
#define CLEAR_TIME    1000U   //@500ms
U8 count = 2; 
U16 dbg_count = 0;
static U8 SelStepDir(void)
{
    dbg_count++;
    if( IsExpiredTimer( TIMER_ID_TIME_OUT ) == TIMER_EXPIRE )
    {
        count = 2;
    }

    if( count != 0 )
    {
        count--;
    }

    StartTimer( TIMER_ID_TIME_OUT, CLEAR_TIME );

    return 0;
}

static U8 SelStepStart(void)
{
    if( count == 0 )
    {
        OpenCapsule();
    }
    else
    {
        CloseCapsule();
    }

    count = 2;
    return 0;
}

static U8 SelStepStop(void)
{
    InitCapsule();

    return 0;
}
