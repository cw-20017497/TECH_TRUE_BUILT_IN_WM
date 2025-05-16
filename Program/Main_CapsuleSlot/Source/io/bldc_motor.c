#pragma interrupt INTTM01 r_tau0_channel1_interrupt

#include "hw.h"
#include "bldc_motor.h"

/***************************************************************************
 * BLDC MOTOR
 *
 * PWM TIMER :
 *  - frequnecy : 50us, duty value : 20%
 *  
 * FEEDB BACK TIMER  : 800us ( sampling time )
 *  - interrupt priority : high
 *
 */

// max feedback 600hz
// sampling freq 1200hz = 0.83ms 
 
typedef struct _bldc_motor_
{
    U8 PrevStart;
    U8 Start;

    U8 Break;

    U8 Direction;
    F32 Output;     // voltage 0v ~ 12v
    U16 CurrentRpm;

    U16 FeedbackCount;
    U16 Hz;
} BldcMotor_T;
BldcMotor_T bldc;

static void (*pFunUserISR)(U16 val) = NULL ;   // 사용자 등록 ISR 함수

void InitBldcMotor(void)
{
    bldc.PrevStart = FALSE;
    bldc.Start = FALSE;
    bldc.Break = FALSE;
    bldc.Direction = MOTOR_DIR_CW;
    bldc.Output = 0.0f;
    bldc.CurrentRpm = 0;

    bldc.FeedbackCount = 0;
    bldc.Hz = 0;
}

void RegisterBldcMotorFeedback( void (*pUserISR)(U16 val) )
{
    pFunUserISR = pUserISR;
}

void StartBldcMotor(U8 dir, F32 output_volt )
{
    bldc.Start = TRUE;

    bldc.Direction = dir;
    bldc.Output = output_volt;
}


void StopBldcMotor(U8 use_brk)
{
    bldc.Start = FALSE;

    bldc.Output = 0.0f;
    if( use_brk == TRUE )
    {
        bldc.Break = TRUE;
    }
}

void BreakBldcMotor( U8 bldc_break )
{
    bldc.Break = bldc_break;
}

U16 GetBldcCurrentRpm(void)
{

    return bldc.CurrentRpm;
}

// 1600 Register Value : 24V 
// (1600/24) * xV = target register value
#define MAX_TDR         1600U  // 0x0640
static U16 ConvVolt2Reg(U16 mu16MaxTDR, F32 mf32Volt)
{
    F32 mfCalcVal = 0.0f;
    U16 mu16TDR = 0U;

    // TDR 레지스터 계산.
    if( mf32Volt <= 0.0f )
    {
        return 0;   // 0V
    }
    else
    {
        mfCalcVal = ((F32)mu16MaxTDR / MAX_BLDC_OUTPUT_VOLT) * mf32Volt;
        mu16TDR = (U16)mfCalcVal;
    }
    
    if( mu16TDR > mu16MaxTDR )
    {
        mu16TDR = mu16MaxTDR;
    }

    return mu16TDR;
}

// Circulate Pump TIMER 

#define TDR_OUTPUT_RATE_10  0x00A0U
#define TDR_OUTPUT_RATE_20  0x0140U
#define TDR_OUTPUT_RATE_30  0x01E0U
#define TDR_OUTPUT_RATE_40  0x0280U
#define TDR_OUTPUT_RATE_50  0x0320U
#define TDR_OUTPUT_RATE_60  0x03C0U
#define TDR_OUTPUT_RATE_70  0x0460U
#define TDR_OUTPUT_RATE_80  0x0500U
#define TDR_OUTPUT_RATE_90  0x05A0U
#define TDR_OUTPUT_RATE_100 0x0640U
#define TDR_STARTING_INIT_VALUE     TDR_OUTPUT_RATE_20


#define  BLDC_MOTOR_TDR_REG         TDR01

// 배수 펌프 START PWM TIMER
static void Timer_TurnOnBldcMotor(void)
{
    // PWM
    TOE0 |= _0080_TAU_CH7_OUTPUT_ENABLE;
    TS0 |= _0040_TAU_CH6_START_TRG_ON | _0080_TAU_CH7_START_TRG_ON;

    // Feedback
    R_TAU0_Channel1_Start();
}

// 배수 펌프 STOP PWM TIMER
static void Timer_TurnOffBldcMotor(void)
{
    // PWM
    TOE0 &= ~_0080_TAU_CH7_OUTPUT_ENABLE;
    TT0 |= _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON;
    TO0 |= _0080_TAU_CH7_OUTPUT_VALUE_1;

    // Feedback
    R_TAU0_Channel1_Stop();
}

U8 HAL_IsTurnOnBldcMotor(void)
{
    if( (TOE0 & _0080_TAU_CH7_OUTPUT_ENABLE ) != 0 )
    {
        return TRUE;
    }

    return FALSE;
}

// sampling feeback frequency 
#define INTS_BYTE_BLDC_MOTOR()     unsigned char _ints_byte
#define MASK_BLDC_MOTOR                       TMMK01
#define DISABLE_INT_MASK_BLDC_MOTOR()          do { MASK_BLDC_MOTOR = 1; }while(0)
#define ENABLE_INT_MASK_BLDC_MOTOR()           do { MASK_BLDC_MOTOR = 0; }while(0)

#define ENTER_CRITICAL_SECTION_BLDC_MOTOR()  \
    do{ \
        _ints_byte = MASK_BLDC_MOTOR; \
        DISABLE_INT_MASK_BLDC_MOTOR(); \
    }while(0)

#define EXIT_CRITICAL_SECTION_BLDC_MOTOR()   \
    do{ \
        MASK_BLDC_MOTOR = _ints_byte; \
    } while(0)
__interrupt static void r_tau0_channel1_interrupt(void)
{
    static U8 prev = 0;
    static U8 current = 0;


    current = GET_BLDC_MOTOR_FG();
    if( prev != current )
    {
        prev = current;
        if( current == HIGH )
        {
            bldc.FeedbackCount++;
        }
    }
}

static void UpdateFeedbackFreq(void)
{
    static U16 SecCount = 100;  //interval 1sec @ 10ms
    U16 Count;

    SecCount--;
    if( SecCount == 0 )
    {
        INTS_BYTE_BLDC_MOTOR();
        SecCount = 100;


        ENTER_CRITICAL_SECTION_BLDC_MOTOR();

        if( pFunUserISR != NULL )
        {
            pFunUserISR( bldc.FeedbackCount );
        }

        bldc.Hz = bldc.FeedbackCount;
        bldc.FeedbackCount = 0;

        EXIT_CRITICAL_SECTION_BLDC_MOTOR();


        bldc.CurrentRpm = bldc.Hz * 60U; // 1min
    }
}


#define WAIT_TIME   10
void ProcessBldcMotor(void)
{
    static U16 wait = WAIT_TIME;  //interval 1sec @ 10ms

    if( bldc.PrevStart != bldc.Start )
    {
        bldc.PrevStart = bldc.Start;
        // OFF -> ON
        if( bldc.Start == TRUE )
        {
            // 1. direction 
            if( bldc.Direction == MOTOR_DIR_CW )
            {
                TURN_ON_BLDC_MOTOR_FR();
            }
            else
            {
                TURN_OFF_BLDC_MOTOR_FR();
            }

            // 2. RELEASE BREAK
            bldc.Break = FALSE;
            TURN_OFF_BLDC_MOTOR_BRK();

            // 3. START PWM
            Timer_TurnOnBldcMotor();

            // 4. SETTING TDR INIT VALUE
            //BLDC_MOTOR_TDR_REG = TDR_STARTING_INIT_VALUE;
            BLDC_MOTOR_TDR_REG = ConvVolt2Reg( MAX_TDR, bldc.Output );
        }
        else
        {
            wait = WAIT_TIME; // @1sec

            // 1. STOP PWM
            Timer_TurnOffBldcMotor();
            //BLDC_MOTOR_PWM = 1;

            //            // 2. BREAK
            //            if( bldc.Break == TRUE )
            //            {
            //                TURN_ON_BLDC_MOTOR_BRK();
            //            }
        }
    }

    if( bldc.Break == FALSE )
    {
        TURN_OFF_BLDC_MOTOR_BRK();
    }
    else
    {
        TURN_ON_BLDC_MOTOR_BRK();
    }

    UpdateFeedbackFreq();

    // DIRECTION 
    if( bldc.Start == TRUE )
    {
        if( bldc.Direction == MOTOR_DIR_CW )
        {
            TURN_ON_BLDC_MOTOR_FR();
        }
        else
        {
            TURN_OFF_BLDC_MOTOR_FR();
        }

        if( wait != 0 )
        {
            wait--;
        }
        else
        {
            BLDC_MOTOR_TDR_REG = ConvVolt2Reg( MAX_TDR, bldc.Output );
        }

    }
}
