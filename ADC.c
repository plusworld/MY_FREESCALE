#include "K60_datatype.h"

/******************************************************************************/
/* Name   : ADC0_Init                                                         */
/* Descrp : initialize ADC0                                                   */
/* Input  : ADC voltage reference,Hardware average select,                    */
/*          Conversion mode selection                                         */
/* Output : None.                                                             */
/* Return : None.                                                             */
/******************************************************************************/

void ADC0_Init(U8 ucRef, U8 ucAver, U8 ucMode)
{

    ucRef  &= 0x03;
    ucMode &= 0x03;

    /*config Voltage Reference enable*/
    VREF_SC = VREF_SC_VREFEN_MASK |
                VREF_SC_MODE_LV(1);           // VREFEN REGEN X X X VREFST MODE_LV
                                              //  1      0                  0  1
                                              //VREFEN:1 Internal Voltage Reference enable 
                                              //MODE_LV:01 Low-power buffer enabled
    while (!(VREF_SC & VREF_SC_VREFST_MASK))
    {
        ;
    }

    /* ADC0 clock enable */
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;

    ADC0_CFG1 = (ADC_CFG1_ADIV(2)            // ADIV  =01:clock/2
                 |ADC_CFG1_ADLSMP_MASK        // ADLSMP= 1:Long sample time
                 |ADC_CFG1_MODE(ucMode)       // MODE  =ucMode:Conversion mode selection
                 |ADC_CFG1_ADICLK(0));        // ADICLK=00:Bus clock

    ADC0_CFG2 = (ADC_CFG2_ADHSC_MASK         // ADHSC = 1:High speed conversion sequence selected
                 |ADC_CFG2_ADLSTS(0));        // ADLSTS=00:Default longest sample time

    ADC0_SC2 =   ADC_SC2_REFSEL(ucRef);        // REFSEL bits select the voltage reference source used for conversions        

    if (ucAver < ADC_HARD_AVER_0)
    {
        ADC0_SC3 = (ADC_SC3_AVGS(ucAver)     // AVGS determines how many ADC conversions will be averaged to create the ADC average result
                    |ADC_SC3_AVGE_MASK);      // AVGE = 1:Hardware average function enabled
    }

}


/******************************************************************************/
/* Name   : ADC1_Init                                                         */
/* Descrp : initialize ADC1                                                   */
/* Input  : ADC voltage reference,Hardware average select,                    */
/*          Conversion mode selection                                         */
/* Output : None.                                                             */
/* Return : None.                                                             */
/******************************************************************************/


void ADC1_Init(U8 ucRef, U8 ucAver, U8 ucMode)
{

    ucRef  &= 0x03;
    ucMode &= 0x03;

    // ADC1 clock enable:
    SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;

    ADC1_CFG1 = (ADC_CFG1_ADIV(2)            // ADIV  =01:clock/2
                 |ADC_CFG1_ADLSMP_MASK        // ADLSMP= 1:Long sample time
                 |ADC_CFG1_MODE(ucMode)       // MODE  =ucMode:Conversion mode selection
                 |ADC_CFG1_ADICLK(0));        // ADICLK=00:Bus clock

    ADC1_CFG2 = (ADC_CFG2_ADHSC_MASK         // ADHSC = 1:High speed conversion sequence selected
                 |ADC_CFG2_ADLSTS(0));        // ADLSTS=00:Default longest sample time
                 
    ADC1_SC2 = ADC_SC2_REFSEL(ucRef);        // REFSEL bits select the voltage reference source used for conversions        

    if (ucAver < ADC_HARD_AVER_0)
    {
        ADC1_SC3 = (ADC_SC3_AVGS(ucAver)     // AVGS determines how many ADC conversions will be averaged to create the ADC average result
                    |ADC_SC3_AVGE_MASK);      // AVGE = 1:Hardware average function enabled
    }

}

/******************************************************************************/
/* Name   : ADC_GetData                                                       */
/* Descrp : get result from ADC0 ADC1 channel                                 */
/* Input  : ADC register base address,Differential or                         */
/*          single-ended mode,Input channel                                   */
/* Output : None.                                                             */
/* Return : None.                                                             */
/******************************************************************************/


U16 ADC_GetData(U8 ucADCCh, U8 ucDiff, U8 ucInputCh,U8 ucADCMuxSel)
{

    U32    ulTemp;

    U16    usResult;

    U32   uiCnt;

    ADC_MemMapPtr pstADC = ADC_GetBaseAddr(ucADCCh);


    if (ADC_MUXSEL_ADXXA == ucADCMuxSel)
    {
        pstADC->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
    }
    else if (ADC_MUXSEL_ADXXB == ucADCMuxSel)
    {
        pstADC->CFG2 |= ADC_CFG2_MUXSEL_MASK;  
    }

    ucInputCh &= 0x1F;

    ulTemp  = pstADC->SC1[0];
    ulTemp &= 0xFFFFFFC0;

    ulTemp |= ADC_SC1_ADCH(ucInputCh);

    if (ADC_DIFF == ucDiff)
    {
        ulTemp |= ADC_SC1_DIFF_MASK;
    }

    pstADC->SC1[0] = ulTemp;

    uiCnt = TIME_OUT_CNT;
    while (!(pstADC->SC1[0] & ADC_SC1_COCO_MASK))
    {
        if (0 == --uiCnt)
        {
            return 0;
        }
    }

    usResult = pstADC->R[0];

    return usResult;

}

/******************************************************************************/
/* Name   : ADC_GetBaseAddr                                                   */
/* Descrp : get base address of ADC registers                                 */
/* Input  : ADC channel                                                       */
/* Output : None.                                                             */
/* Return : None.                                                             */
/******************************************************************************/


static ADC_MemMapPtr ADC_GetBaseAddr(U8 ucADCCh)
{

    switch (ucADCCh)
    {
        case ADC0_INDEX:
        return ADC0_BASE_PTR;

        case ADC1_INDEX:
        return ADC1_BASE_PTR;

        default:
        return ADC0_BASE_PTR;
    }  

}


