#ifndef __K60_ADC_H__
#define __K60_ADC_H__



/***DEF+**********************************************************************/
/* define ADC .                                                              */
/***DEF-**********************************************************************/


#define ADC_REF_VREF                 0    // Default voltage reference pin pair (External pins V REFH and V REFL )
#define ADC_REF_VALT                 1    // Alternate reference pair (V ALTH and V ALTL ). This pair may be additional external pins or internal
                                          // sources depending on MCU configuration. Consult the module introduction for information on the
                                          // Voltage Reference specific to this MCU.
#define ADC_HARD_AVER_4              0    // 4 samples averaged.
#define ADC_HARD_AVER_8              1    // 8 samples averaged.
#define ADC_HARD_AVER_16             2    // 16 samples averaged.
#define ADC_HARD_AVER_32             3    // 32 samples averaged.
#define ADC_HARD_AVER_0              4    // disable hardware average

#define ADC_MODE_8BIT                0    // When DIFF=0: It is single-ended 8-bit conversion; when DIFF=1, it is differential 9-bit conversion with 2's complement output.
#define ADC_MODE_12BIT               1    // When DIFF=0: It is single-ended 12-bit conversion; when DIFF=1, it is differential 13-bit conversion with 2's complement output.
#define ADC_MODE_10BIT               2    // When DIFF=0: It is single-ended 10-bit conversion; when DIFF=1, it is differential 11-bit conversion with 2's complement output.
#define ADC_MODE_16BIT               3    // When DIFF=0: It is single-ended 16-bit conversion; when DIFF=1, it is differential 16-bit conversion with 2's complement output.

#define ADC_SINGLE                   0
#define ADC_DIFF                     1
                                              //ADC0               ADC1
#define ADC_SEL_DADP0                0x00
#define ADC_SEL_DADP1                0x01
#define ADC_SEL_DADP2                0x02
#define ADC_SEL_DADP3                0x03

#define ADC_SEL_DAD0                 0x00
#define ADC_SEL_DAD1                 0x01
#define ADC_SEL_DAD2                 0x02
#define ADC_SEL_DAD3                 0x03

#define ADC_SEL_AD4                  0x04//                     ADC1_SE4a - PTE0   
#define ADC_SEL_AD5                  0x05//                     ADC1_SE5a - PTE1
#define ADC_SEL_AD6                  0x06//                     ADC1_SE6a - PTE2
#define ADC_SEL_AD7                  0x07//                     ADC1_SE7a - PTE3
#define ADC_SEL_AD8                  0x08// ADC0_SE8 - PTB0     ADC1_SE8 - PTB0
#define ADC_SEL_AD9                  0x09// ADC0_SE9 - PTB1     ADC1_SE9 - PTB1
#define ADC_SEL_AD10                 0x0A// ADC0_SE10 - PTB7    ADC1_SE10 - PTB4
#define ADC_SEL_AD11                 0x0B// ADC0_SE11 - PTB8    ADC1_SE11 - PTB5
#define ADC_SEL_AD12                 0x0C// ADC0_SE12 - PTB2    ADC1_SE12 - PTB6
#define ADC_SEL_AD13                 0x0D// ADC0_SE13 - PTB3    ADC1_SE13 - PTB7
#define ADC_SEL_AD14                 0x0E// ADC0_SE14 - PTC0    ADC1_SE14 - PTB10
#define ADC_SEL_AD15                 0x0F// ADC0_SE15 - PTC1    ADC1_SE15 - PTB11
#define ADC_SEL_AD16                 0x10
#define ADC_SEL_AD17                 0x11// ADC0_SE17 - PTE24   ADC1_SE17 - PTA17
#define ADC_SEL_AD18                 0x12// ADC0_SE18 - PT25    ADC1_SE18 - PTB0
#define ADC_SEL_AD19                 0x13
#define ADC_SEL_AD20                 0x14
#define ADC_SEL_AD21                 0x15
#define ADC_SEL_AD22                 0x16
#define ADC_SEL_AD23                 0x17

#define ADC_SEL_TEMP_SENSOR_SINGLE   0x1A
#define ADC_SEL_TEMP_SENSOR_DIFF     0x1A

#define ADC_SEL_BANDGAP_SINGLE       0x1B
#define ADC_SEL_BANDGAP_DIFF         0x1B

#define ADC_SEL_VREFSH_SINGLE        0x1D
#define ADC_SEL_VREFSH_DIFF          0x1D

#define ADC_SEL_VREFSL               0x1E

#define ADC_DIS                      0x1F

#define TIME_OUT_CNT                 (0xFFFFFFFF)

#define ADC_MUXSEL_ADXXA             0x00
#define ADC_MUXSEL_ADXXB             0x01


typedef enum ADC_INDEX
{
    ADC0_INDEX = 0,
    ADC1_INDEX = 1
    
}eADCChannel;



/*****************************************************************************/
/*                         Function Declare                                  */

/*****************************************************************************/



void ADC0_Init(U8 ucRef, U8 ucAver, U8 ucMode);
void ADC1_Init(U8 ucRef, U8 ucAver, U8 ucMode);
U16  ADC_GetData(U8 ucADCCh, U8 ucDiff, U8 ucInputCh,U8 ucADCMuxSel);

static ADC_MemMapPtr ADC_GetBaseAddr(U8 ucADCCh);
#endif 
