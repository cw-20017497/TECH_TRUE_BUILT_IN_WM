#include "hw.h"
#include "hal_key.h"


#define  HAL_KEY_PUSHED     LOW
#define  HAL_LEVER_PUSHED   LOW

#define  DEFAULT_COUNT      50
U32 gu32Key     = 0UL;       // 터치 버튼 
U32 gu32KeyOut  = 0UL;       // 추출 버튼

U32 HAL_GetKeyVal(void)
{
    U32 mu32Val = 0UL;

    mu32Val = gu32Key | gu32KeyOut;
    return mu32Val;
}

static void ScanKey(void)
{
    static U16 mu16Count = 0;
    static U32 mu32PrevVal = 0;
    U32 mu32Val = 0;


    // SCAN KEY
    if( P_KEY_1 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_1;
    }

    // RESET KEY COUNT
    if( mu32PrevVal != mu32Val )
    {
        mu32PrevVal = mu32Val;
        mu16Count = DEFAULT_COUNT;

        return ;
    }

    if( mu16Count != 0 )
    {
        mu16Count--;
        return ;
    }

    // UPDATE NEW KEY
    gu32Key = mu32Val;
}

static void ScanKeyOut(void)
{
    static U32 mu32Count = 0;
    static U32 mu32PrevVal = 0;
    U32 mu32Val = 0;


    // SCAN KEY
    //if( P_KEY_19 == HAL_KEY_PUSHED )
    //{
    //    mu32Val |= HAL_KEY_19;
    //}

    //if( P_KEY_20 == HAL_KEY_PUSHED )
    //{
    //    mu32Val |= HAL_KEY_20;
    //}


    // RESET KEY COUNT
    if( mu32PrevVal != mu32Val )
    {
        mu32PrevVal = mu32Val;
        mu32Count = DEFAULT_COUNT;

        return ;
    }

    if( mu32Count != 0 )
    {
        mu32Count--;
        return ;
    }

    // UPDATE NEW KEY
    gu32KeyOut = mu32Val;
}

void HAL_ScanKey(void)
{
    ScanKey();
    //ScanKeyOut();
}
