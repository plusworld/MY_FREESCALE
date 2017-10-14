 #include "K60_datatype.h"

void BSP_init()
{
  
 
   WatchdogDisable();  //���Ź�

   
   VECTableInit();

   SIM_SCGC5 |= (      SIM_SCGC5_PORTA_MASK     //PORTA��B��C��D��E��ʱ�ӿ���
                      | SIM_SCGC5_PORTB_MASK
                      | SIM_SCGC5_PORTC_MASK
                      | SIM_SCGC5_PORTD_MASK
                      | SIM_SCGC5_PORTE_MASK 
                        );
    
    //ϵͳʱ�ӳ�ʼ������Ƶ125MHZ
    PLL_Init();
    
    IO_init();
    //OLEDģ���ʼ��
    OLED_Init();
    
    boma_init();
    /*********�ж����ȼ����趨**********/
   
    set_irq_priority (89, 0); //PORTE�ж����ж�
    set_irq_priority (1, 1);  //DMA�ж�
    set_irq_priority (68, 2); //PIT�ж�
  
   
    
    //FTMģ��PWM�����ʼ��
    //FTM0 f(bus)=62.5M/4/250=62.5KHz
    PWM_Init(PWM_PIN_PTC1,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);
    PWM_Init(PWM_PIN_PTC2,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);
    PWM_Init(PWM_PIN_PTC3,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);
    PWM_Init(PWM_PIN_PTC4,PWM_FRE_DIV_FACTOR_2,PWM_MODE_HIGH_ACTIVE);//2
    //FTM2 f(bus)=62.5M/32/9764 (MOD)=100Hz
    PWM_Init(PWM_PIN_PTA10, PWM_FRE_DIV_FACTOR_32,PWM_MODE_HIGH_ACTIVE);
    
    

    //����ͷ���ݴ��������
    Gpio_init(PORT_E, 0, 0, 0);
    Gpio_init(PORT_E, 1, 0, 0);
    Gpio_init(PORT_E, 2, 0, 0);
    Gpio_init(PORT_E, 3, 0, 0);
    Gpio_init(PORT_E, 4, 0, 0);
    Gpio_init(PORT_E, 5, 0, 0);
    Gpio_init(PORT_E, 6, 0, 0);
    Gpio_init(PORT_E, 7, 0, 0);
    
    //PTC8�����ж�,���жϣ����ж�����Ϊ����״̬
    Gpio_init(PORT_C,16,0,0);  //�����ж�
    Gpio_init(PORT_C,18,0,0);
    Gpio_init(PORT_C,19,0,0);
   
    PORTC_PCR16|=PORT_PCR_IRQC(1);//DMA���� 
    
    
    DMA_transmit_init((void *)&GPIOE_PDIR, Buffer,280,51);
    DMA_count_Init(DMA_CH4, PTB3 , 2000, DMA_rising);
    DMA_count_Init(DMA_CH5, PTA8, 2000, DMA_rising);
    
    //�ⲿ�жϳ�ʼ��
    exti_init();
    
    //���İ��ϴ���UART4��ʼ��
    UART_Init(PERIPH_SYS_CLK,115200,UART4);
    
    //AD�ɼ�ģ���ʼ��
    ADC0_Init(ADC_REF_VREF, ADC_HARD_AVER_16, ADC_MODE_12BIT);
    
    //�����ϴ���UART5��ʼ��
    UART_Init(PERIPH_SYS_CLK,115200,UART5);
    
    Timer0_Init();
    
   // DMA_EN(DMA_CH1);//��DMA�ж�
    
    EnableTimer0();//PIT�ж�ʹ��
    
    EnableInterrupts;
  
}