#ifndef __MOVING_FAUCET_H__
#define __MOVING_FAUCET_H__

#include "prj_type.h"

#define  FAUCET_UP_DOWN_END_STEP_VAL     (2100)
#define  FAUCET_FORWARD_END_STEP_VAL     (1100)
typedef enum
{
    MOVING_FAUCET_INITING,    
    MOVING_FAUCET_GOING_READY,
    MOVING_FAUCET_READY,      
    MOVING_FAUCET_GOING_MOVE, 
    MOVING_FAUCET_MOVE,       
    MOVING_FAUCET_MANUAL,     
} FaucetStatus_T;

typedef struct _moving_faucet_
{
    /* STATUS */
    FaucetStatus_T Status; 
    
    /* MODE */
    U8 Mode;         

    /* STEP */
    U8 InitStep;
    U8 ReadyStep;
    U8 MoveStep;

    U8 PowerOnTime; // SMPS ON WAIT TIMEER @100ms
    U8 ReadyTime; // @100ms
    U16 ManualTime; // @100ms
} MovingFaucet_T;

void InitMovingFaucet(void);

void GetMovingFaucetData(MovingFaucet_T *pData);

FaucetStatus_T GetFaucetStatus(void);
void SetFaucetStatus(FaucetStatus_T mu8Status);


/* MODE */
#define  MOVING_FAUCET_MODE_NONE           0x00
#define  MOVING_FAUCET_MODE_INIT           0x01
#define  MOVING_FAUCET_MODE_READY          0x02
#define  MOVING_FAUCET_MODE_MOVE           0x04
U8 GetFaucetMode(void);

void ResetFaucetManualTime(void);
void MovingFaucetInit(void);
void MovingFaucetReady(void);
void MovingFaucetMove(void);
void MovingFaucetStop(void);

void ControlMovingFaucet(void);
void ProcessMovingFaucet(void);

#endif /* __MOVING_FAUCET_H__ */
