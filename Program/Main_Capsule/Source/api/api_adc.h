/**
 * File : api_adc.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_ADC_H__
#define __API_ADC_H__

#include "prj_type.h"

typedef enum _adc_id_
{
    /* TEMPERATURE SENSOR FEEDBACK */
    ADC_ID_TEM_VAD,         // 0 : TEM Feedbac Voltage

    ADC_ID_MAX,
} EAdcId_T;

void InitAdc(void);
void SetAdcData(U8 xUid, U16 xUadc );
U16 GetAdcData( U8 xUid );
void TurnOnSensingAdc(U8 xUid );
void TurnOffSensingAdc( U8 xUid );
I8 IsOnOffSensingAdc( U8 xUid );

void ProcessAdc(void);

#endif
