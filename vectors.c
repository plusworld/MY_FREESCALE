



#include "..\K60_datatype.h"    


/***GBL+**********************************************************************/
/* Vectors table address.                                                    */
/***GBL-**********************************************************************/
extern unsigned long __BOOT_STACK_ADDRESS[];

/***FUNC+*********************************************************************/
/* Name   : WatchdogDisable                                                  */
/* Descrp : watchdog disable                                                 */
/* Input  : None                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void WatchdogDisable(void)
{
    WDOG_UNLOCK = 0xC520;
    WDOG_UNLOCK = 0xD928;
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK; 
}
/***FUNC+*********************************************************************/
/* Name   : VECTableInit                                                     */
/* Descrp : Copy the vector table to RAM                                     */
/* Input  : None                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam" 

void VECTableInit(void)
{
 
    /* Declare a counter we'll use in all of the copy loops */
    ULONG n;
 
 
    /* Addresses for VECTOR_TABLE and VECTOR_RAM come from the linker file */  
    extern ULONG __VECTOR_TABLE[];
    extern ULONG __VECTOR_RAM[];

    /* Copy the vector table to RAM */
    if (__VECTOR_RAM != __VECTOR_TABLE)
    {
        for (n = 0; n < 0x410; n++)
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }
    /* Point the VTOR to the new copy of the vector table */
    //write_vtor((uint32)__VECTOR_RAM);  
    
    SCB_VTOR = (ULONG)__VECTOR_RAM;
    
    /* Get the addresses for the .data section (initialized data section) */
    U8* data_ram = __section_begin(".data");
    U8* data_rom = __section_begin(".data_init");
    U8* data_rom_end = __section_end(".data_init");
    
    /* Copy initialized data from ROM to RAM */
    n = data_rom_end - data_rom;
    while (n--)
      *data_ram++ = *data_rom++;
 
 
    /* Get the addresses for the .bss section (zero-initialized data) */
    U8* bss_start = __section_begin(".bss");
    U8* bss_end = __section_end(".bss");
    
    /* Clear the zero-initialized data section */
    n = bss_end - bss_start;
    while(n--)
      *bss_start++ = 0;    
    
    /* Get addresses for any code sections that need to be copied from ROM to RAM.
     * The IAR tools have a predefined keyword that can be used to mark individual
     * functions for execution from RAM. Add "__ramfunc" before the return type in
     * the function prototype for any routines you need to execute from RAM instead 
     * of ROM. ex: __ramfunc void foo(void);
     */
    U8* code_relocate_ram = __section_begin("CodeRelocateRam");
    U8* code_relocate = __section_begin("CodeRelocate");
    U8* code_relocate_end = __section_end("CodeRelocate");
    
    /* Copy functions from ROM to RAM */
    n = code_relocate_end - code_relocate;
    while (n--)
      *code_relocate_ram++ = *code_relocate++;
    
}
/***FUNC+*********************************************************************/
/* Name   : enable_irq                                                       */
/* Descrp : Initialize the NVIC to enable the specified IRQ.                 */
/* Input  : irq    irq number to be enabled (the irq number NOT the          */
/*          vector number).                                                  */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void enable_irq (INT irq)
{
    INT div;
    
    /* Make sure that the IRQ is an allowable number. Right now up to 91 is 
     * used.
     */
    if (irq > MAX_IRQ_NUM)
    {
        printf("\nERR! Invalid IRQ value passed to enable irq function!\n");
    }
    
    /* Determine which of the NVICISERs corresponds to the irq */
    div = irq/32;
    
    switch (div)
    {
        case 0x0:
              NVICICPR0 |= 1 << (irq%32);
              NVICISER0 |= 1 << (irq%32);
              break;
        case 0x1:
              NVICICPR1 |= 1 << (irq%32);
              NVICISER1 |= 1 << (irq%32);
              break;
        case 0x2:
              NVICICPR2 |= 1 << (irq%32);
              NVICISER2 |= 1 << (irq%32);
              break;
    }              
}

/***FUNC+*********************************************************************/
/* Name   : disable_irq                                                      */
/* Descrp : Initialize the NVIC to disable the specified IRQ.                */
/* Input  : irq    irq number to be disable (the irq number NOT the          */
/*          vector number).                                                  */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void disable_irq (INT irq)
{
    INT div;
    
    /* Make sure that the IRQ is an allowable number. Right now up to 91 is 
     * used.
     */
    if (irq > MAX_IRQ_NUM)
    {
        printf("\nERR! Invalid IRQ value passed to disable irq function!\n");
    }
    
    /* Determine which of the NVICICERs corresponds to the irq */
    div = irq/32;
    
    switch (div)
    {
        case 0x0:
               NVICICER0 |= 1 << (irq%32);
              break;
        case 0x1:
              NVICICER1 |= 1 << (irq%32);
              break;
        case 0x2:
              NVICICER2 |= 1 << (irq%32);
              break;
    }              
}
#if 1 
/***FUNC+*********************************************************************/
/* Name   : set_irq_priority                                                 */
/* Descrp : Initialize the NVIC to set specified IRQ priority.               */
/* Input  : irq    irq number to be enabled (the irq number NOT the          */
/*          vector number).                                                  */
/*          prio   irq priority. 0-15 levels. 0 max priority                 */
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void set_irq_priority (INT irq, INT prio)
{
    /*irq priority pointer*/
    uint8 *prio_reg;
    
    /* Make sure that the IRQ is an allowable number. Right now up to 91 is 
     * used.
     */
    if (irq > MAX_IRQ_NUM)
    {
        printf("\nERR! Invalid IRQ value passed to priority irq function!\n");
    }

    if (prio > 15)
    {
        printf("\nERR! Invalid priority value passed to priority irq function!\n");
    }
    
    /* Determine which of the NVICIPx corresponds to the irq */
    prio_reg = (uint8 *)(((uint32)&NVICIP0) + irq);

    /* Assign priority to IRQ */
    *prio_reg = ( (prio&0xF) << (8 - 4) );             
}
#endif
/***FUNC+*********************************************************************/
/* Name   : Vector Table                                                     */
/* Descrp : struct Vector Table                                              */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
typedef void (*vector_entry)(void);

#pragma location = ".intvec"

const vector_entry  __vector_table[] = //@ ".intvec" =
{
   VECTOR_000,           /* Initial SP           */
   VECTOR_001,           /* Initial PC           */
   VECTOR_002,
   VECTOR_003,
   VECTOR_004,
   VECTOR_005,
   VECTOR_006,
   VECTOR_007,
   VECTOR_008,
   VECTOR_009,
   VECTOR_010,
   VECTOR_011,
   VECTOR_012,
   VECTOR_013,
   VECTOR_014,
   VECTOR_015,
   VECTOR_016,
   VECTOR_017,
   VECTOR_018,
   VECTOR_019,
   VECTOR_020,
   VECTOR_021,
   VECTOR_022,
   VECTOR_023,
   VECTOR_024,
   VECTOR_025,
   VECTOR_026,
   VECTOR_027,
   VECTOR_028,
   VECTOR_029,
   VECTOR_030,
   VECTOR_031,
   VECTOR_032,
   VECTOR_033,
   VECTOR_034,
   VECTOR_035,
   VECTOR_036,
   VECTOR_037,
   VECTOR_038,
   VECTOR_039,
   VECTOR_040,
   VECTOR_041,
   VECTOR_042,
   VECTOR_043,
   VECTOR_044,
   VECTOR_045,
   VECTOR_046,
   VECTOR_047,
   VECTOR_048,
   VECTOR_049,
   VECTOR_050,
   VECTOR_051,
   VECTOR_052,
   VECTOR_053,
   VECTOR_054,
   VECTOR_055,
   VECTOR_056,
   VECTOR_057,
   VECTOR_058,
   VECTOR_059,
   VECTOR_060,
   VECTOR_061,
   VECTOR_062,
   VECTOR_063,
   VECTOR_064,
   VECTOR_065,
   VECTOR_066,
   VECTOR_067,
   VECTOR_068,
   VECTOR_069,
   VECTOR_070,
   VECTOR_071,
   VECTOR_072,
   VECTOR_073,
   VECTOR_074,
   VECTOR_075,
   VECTOR_076,
   VECTOR_077,
   VECTOR_078,
   VECTOR_079,
   VECTOR_080,
   VECTOR_081,
   VECTOR_082,
   VECTOR_083,
   VECTOR_084,
   VECTOR_085,
   VECTOR_086,
   VECTOR_087,
   VECTOR_088,
   VECTOR_089,
   VECTOR_090,
   VECTOR_091,
   VECTOR_092,
   VECTOR_093,
   VECTOR_094,
   VECTOR_095,
   VECTOR_096,
   VECTOR_097,
   VECTOR_098,
   VECTOR_099,
   VECTOR_100,
   VECTOR_101,
   VECTOR_102,
   VECTOR_103,
   VECTOR_104,
   VECTOR_105,
   VECTOR_106,
   VECTOR_107,
   VECTOR_108,
   VECTOR_109,
   VECTOR_110,
   VECTOR_111,
   VECTOR_112,
   VECTOR_113,
   VECTOR_114,
   VECTOR_115,
   VECTOR_116,
   VECTOR_117,
   VECTOR_118,
   VECTOR_119,
   VECTOR_120,
   VECTOR_121,
   VECTOR_122,
   VECTOR_123,
   VECTOR_124,
   VECTOR_125,
   VECTOR_126,
   VECTOR_127,
   VECTOR_128,
   VECTOR_129,
   VECTOR_130,
   VECTOR_131,
   VECTOR_132,
   VECTOR_133,
   VECTOR_134,
   VECTOR_135,
   VECTOR_136,
   VECTOR_137,
   VECTOR_138,
   VECTOR_139,
   VECTOR_140,
   VECTOR_141,
   VECTOR_142,
   VECTOR_143,
   VECTOR_144,
   VECTOR_145,
   VECTOR_146,
   VECTOR_147,
   VECTOR_148,
   VECTOR_149,
   VECTOR_150,
   VECTOR_151,
   VECTOR_152,
   VECTOR_153,
   VECTOR_154,
   VECTOR_155,
   VECTOR_156,
   VECTOR_157,
   VECTOR_158,
   VECTOR_159,
   VECTOR_160,
   VECTOR_161,
   VECTOR_162,
   VECTOR_163,
   VECTOR_164,
   VECTOR_165,
   VECTOR_166,
   VECTOR_167,
   VECTOR_168,
   VECTOR_169,
   VECTOR_170,
   VECTOR_171,
   VECTOR_172,
   VECTOR_173,
   VECTOR_174,
   VECTOR_175,
   VECTOR_176,
   VECTOR_177,
   VECTOR_178,
   VECTOR_179,
   VECTOR_180,
   VECTOR_181,
   VECTOR_182,
   VECTOR_183,
   VECTOR_184,
   VECTOR_185,
   VECTOR_186,
   VECTOR_187,
   VECTOR_188,
   VECTOR_189,
   VECTOR_190,
   VECTOR_191,
   VECTOR_192,
   VECTOR_193,
   VECTOR_194,
   VECTOR_195,
   VECTOR_196,
   VECTOR_197,
   VECTOR_198,
   VECTOR_199,
   VECTOR_200,
   VECTOR_201,
   VECTOR_202,
   VECTOR_203,
   VECTOR_204,
   VECTOR_205,
   VECTOR_206,
   VECTOR_207,
   VECTOR_208,
   VECTOR_209,
   VECTOR_210,
   VECTOR_211,
   VECTOR_212,
   VECTOR_213,
   VECTOR_214,
   VECTOR_215,
   VECTOR_216,
   VECTOR_217,
   VECTOR_218,
   VECTOR_219,
   VECTOR_220,
   VECTOR_221,
   VECTOR_222,
   VECTOR_223,
   VECTOR_224,
   VECTOR_225,
   VECTOR_226,
   VECTOR_227,
   VECTOR_228,
   VECTOR_229,
   VECTOR_230,
   VECTOR_231,
   VECTOR_232,
   VECTOR_233,
   VECTOR_234,
   VECTOR_235,
   VECTOR_236,
   VECTOR_237,
   VECTOR_238,
   VECTOR_239,
   VECTOR_240,
   VECTOR_241,
   VECTOR_242,
   VECTOR_243,
   VECTOR_244,
   VECTOR_245,
   VECTOR_246,
   VECTOR_247,
   VECTOR_248,
   VECTOR_249,
   VECTOR_250,
   VECTOR_251,
   VECTOR_252,
   VECTOR_253,
   VECTOR_254,
   VECTOR_255,
   CONFIG_1,
   CONFIG_2,
   CONFIG_3,
   CONFIG_4,
};


/***FUNC+*********************************************************************/
/* Name   : default_isr                                                      */
/* Descrp : Default ISR definition                                           */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void default_isr(void) @ ".vec2nd"
{
   DummyISR();   
}
/***FUNC+*********************************************************************/
/* Name   : DummyISR                                                         */
/* Descrp : Default ISR definition                                           */
/* Input  : num.                                                             */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void DummyISR(void)
{
    return;   
}

void DMA_CH1_Handler()
{ 
  DMA_IRQ_CLEAN(DMA_CH1);  //清除通道传输中断标志位(这样才能再次进入中断)
       
    DMA_DIS(DMA_CH1);        //采集完H个数据后进入这个DMA中断，停止DMA传输。行中断中打开DMA传输 
     
    if(V_Cnt >= 240)         //串口调试用到
    {
    
      Is_SendPhoto = 1;
    
   
    }
}

void PORTC_Interrupt()
{
  //  DisableInterrupts; 
   if(PORTC_ISFR & (1 << 18))//场中断 //PTC18触发中断
   {
      PORTC_ISFR |= (1 << 18);//写1清中断标志位 
      
      Is_SendPhoto = 0;  //发送图像标志，串口调试要用
 
      DMA_TCD1_DADDR = (u32)Buffer;            
          
      V_Cnt=0;    //行采集计数清零   
       hang_flag=0;
         
    Is_SendPhoto = 0; //发送图像标志  
  
    }
   
    if(PORTC_ISFR & (1 << 19))//PTC19触发中断//行中断
    {
      PORTC_ISFR |= (1 << 19);//写1清中断标志位 
      
      V_Cnt++;
   
      if(V_Cnt%4==0)
     
      
      {
      
        DMA_EN(DMA_CH1);  //使能通道CHn 硬件请求
        hang_flag++;  
      }
    } 
 //  EnableInterrupts;
}  
  





//PIT中断服务函数                 尝试关中断
void Timer0_isr()
{
  static unsigned char TimerCntms1=0;
  
  TimerCntms1++;
     
  //每12ms进行一次CCD采样，控制电机与舵机  
  if(TimerCntms1 == 12) 
  {
     TimerCntms1 = 0;
     DisableInterrupts; 
     if(Lostright==1&&Lostleft==1&&fabss(error_duoj)>30)       //圆环情况
        MotorControl(zhankongbiL+(7*error_duoj),zhankongbiR-(7*error_duoj));  //圆环差速
     else                                                      //普通情况
        MotorControl(zhankongbiL+(4*error_duoj),zhankongbiR-(4*error_duoj));  //普通路径差速
     PWM_Output(PWM_PIN_PTA10,SteeringPWM);  //舵机控制
     EnableInterrupts;
    
  }
   if(TimerCntms1==6)
   {

    SpeedL=DMA_count_get(DMA_CH4);           //采集左轮速度
    DMA_count_reset(DMA_CH4);                //计数清零
    
    SpeedR=DMA_count_get(DMA_CH5);           //采集右轮速度 
    DMA_count_reset(DMA_CH5);                //计数清零
   }
  CLR_PIT0;
   
  PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; //计时器使能
}





     #ifdef  __cplusplus
}



#endif

   



 
              