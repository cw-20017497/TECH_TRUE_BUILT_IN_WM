#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#define  HAL_KEY_NONE   0x00000000UL
#define  HAL_KEY_1      0x00000001UL

#define  HAL_KEY_ALL    0x000FFFFFUL
U32 HAL_GetKeyVal(void);


void HAL_ScanKey(void);
#endif /* __HAL_KEY_H__ */
