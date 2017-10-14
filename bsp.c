 #include "K60_datatype.h"

void BSP_init()
{
  
 
   WatchdogDisable();  //看门狗

   
   VECTableInit();

   SIM_SCGC5 |= (      SIM_SCGC5_PORTA_MASK     //PORTA，B，C，D，E口时钟开启
                      | SIM_SCGC5_PORTB_MASK
                      | SIM_SCGC5_PORTC_MASK
                      | SIM_SCGC5_PORTD_MASK
                      | SIM_SCGC5_PORTE_MASK 
                        );
    
    //系统时钟初始化，主频125MHZ
    PLL_Init();
    
    IO_init();
    //OLED模块初始化
    OLED_Init();
    
    boma_init();
    /*********中断优先级的设定**********/
   
    set_irq_priority (89, 0); //PORTE中断行中断
    set_irq_priority (1, 1);  //DMA中断
    set_irq_priority (68, 2); //PIT中断
  
   
    
    //FTM模块PWM输出初始化
    //FTM0 f(bus)=62.5M/4/250=62.5KHz
    PWM_Init(PWM_PIN_PTC1,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);
    PWM_Init(PWM_PIN_PTC2,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);
    PWM_Init(PWM_PIN_PTC3,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);
    PWM_Init(PWM_PIN_PTC4,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);//2
    //FTM2 f(bus)=62.5M/32/9764 (MOD)=100Hz
    PWM_Init(PWM_PIN_PTA10, PWM_FRE_DIV_FACTOR_32,PWM_MODE_HIGH_ACTIVE);
    
    

    //摄像头数据传输的引脚
    Gpio_init(PORT_E, 0, 0, 0);
    Gpio_init(PORT_E, 1, 0, 0);
    Gpio_init(PORT_E, 2, 0, 0);
    Gpio_init(PORT_E, 3, 0, 0);
    Gpio_init(PORT_E, 4, 0, 0);
    Gpio_init(PORT_E, 5, 0, 0);
    Gpio_init(PORT_E, 6, 0, 0);
    Gpio_init(PORT_E, 7, 0, 0);
    
    //PTC8像素中断,行中断，场中断设置为输入状态
    Gpio_init(PORT_C,16,0,0);  //像素中断
    Gpio_init(PORT_C,18,0,0);
    Gpio_init(PORT_C,19,0,0);
   
    PORTC_PCR16|=PORT_PCR_IRQC(1);//DMA请求 
    
    
    DMA_transmit_init((void *)&GPIOE_PDIR, Buffer,280,51);
    DMA_count_Init(DMA_CH4, PTB3 , 2000, DMA_rising);
    DMA_count_Init(DMA_CH5, PTA8, 2000, DMA_rising);
    
    //外部中断初始化
    exti_init();
    
    //核心板上串口UART4初始化
    UART_Init(PERIPH_SYS_CLK,115200,UART4);
    
    //AD采集模块初始化
    ADC0_Init(ADC_REF_VREF, ADC_HARD_AVER_16, ADC_MODE_12BIT);
    
    //主板上串口UART5初始化
    UART_Init(PERIPH_SYS_CLK,115200,UART5);
    
    Timer0_Init();
    
   // DMA_EN(DMA_CH1);//打开DMA中断
    
    EnableTimer0();//PIT中断使能
    
    EnableInterrupts;
  
}