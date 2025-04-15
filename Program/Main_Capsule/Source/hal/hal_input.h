#ifndef __HAL_INPUT_H__
#define __HAL_INPUT_H__


/* INPUT ID */
enum
{
    IN_DOOR_OPEN,
    IN_DOOR_CLOSE,

    IN_CAPSULE_OUT,
    IN_CAPSULE_IN,

    IN_MAX_NUM
};

#define INPUT_DELAY_30MS   3U        /* 30ms @10ms */
#define INPUT_DELAY_100MS  10U       /* 100ms @10ms */
#define INPUT_DELAY_300MS  30U       /* 300ms @10ms */
#define INPUT_DELAY_1SEC   100U      /* 1sec @10ms */

void InitInputConfDetectTime(void);

void SetInputConfDetectTime(U8 mu8Id, U8 mu16DetectTime);

U8 HAL_GetInputValue(U8  mu8Id );

void HAL_ProcessInput(void);

#endif /* __HAL_INPUT_H__ */
