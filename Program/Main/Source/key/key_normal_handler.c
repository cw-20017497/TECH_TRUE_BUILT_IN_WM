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




static void DispOnOff(U8 mu8OnOff);

static U8 SelOnOff(void);
static U8 OnOffLed(void);


// 일반 모드 리스트
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,        2sec,           3sec,      5sec,           Pop,           TS */
    { K_ONOFF,        SelOnOff,      OnOffLed,       NULL,      NULL,           NULL,          NULL },
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
