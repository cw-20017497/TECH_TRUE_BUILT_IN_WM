#ifndef __CAPSULE_H__
#define __CAPSULE_H__

#include "prj_type.h"

void InitCapsule(void);

#define CAPSULE_OPEN    0   // FULL OPENED
#define CAPSULE_OPENING 1   // OPENING..
#define CAPSULE_CLOSE   2   // FULL CLOSED
#define CAPSULE_CLOSING 3   // CLOSING...
U8 GetCapsuleStatus(void);

// 캡슐 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenCapsule(void);

void OpenCapsule(void);
void CloseCapsule(void);

void ControlCapsule(void);

#endif /* __CAPSULE_H__ */