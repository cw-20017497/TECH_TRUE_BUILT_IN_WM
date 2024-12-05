#ifndef __GRINDING_H__
#define __GRINDING_H__

#include "prj_type.h"



void InitGrinding(void);


#define  GRINDING_OPEN     0     // FULL OPEND
#define  GRINDING_OPENING  1     // OPEING..
#define  GRINDING_CLOSE    2     // FULL CLOSED
#define  GRINDING_CLOSING  3     // CLSOING...
U8 GetGrindingStatus(void);

// 아이스 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenGrinding(void);

void OpenGrinding(void);
void CloseGrinding(void);

void ControlGrinding(void);

#endif /* __GRINDING_H__ */
