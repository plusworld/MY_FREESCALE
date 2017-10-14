
#include "..\K60_datatype.h"  

unsigned char TxBuf[10] = {0,0,0,0,0,0,0,0,0,0};

void UART_Init(U32 SystemClock, U32 BaudRate, U8 UARTChannel)
{
  register U16 ubd, brfa;
  
  UART_MemMapPtr Channel;
  
  U8 temp;
    
  switch (UARTChannel)
  {
    case UART0:
        /* Enable the UART0_TXD function on PTD6 */
        PORTD_PCR6 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        /* Enable the UART0_RXD function on PTD7 */
        PORTD_PCR7 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
        Channel = UART_0;
      break;
      
    case UART1:
        /* Enable the UART1_TXD function on PTC4 */
        PORTC_PCR4 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        /* Enable the UART1_RXD function on PTC3 */
        PORTC_PCR3 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        Channel = UART_1;
      break;
      
    case UART2:
        /* Enable the UART2_TXD function on PTD3 */
        PORTD_PCR3 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        /* Enable the UART2_RXD function on PTD2 */
        PORTD_PCR2 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;      
        Channel = UART_2;
      break;
      
    case UART3:    
        /* Enable the UART3_TXD function on PTC17 */
        PORTC_PCR17 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        /* Enable the UART3_RXD function on PTC16 */
        PORTC_PCR16 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        
        Channel = UART_3;
      break;
      
    case UART4:
        /* Enable the UART3_TXD function on PTC17 */
        PORTE_PCR25 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        /* Enable the UART3_RXD function on PTC16 */
        PORTE_PCR24 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;      
        Channel = UART_4;
      break;
      
    case UART5:
        /* Enable the UART3_TXD function on PTC17 */
        PORTE_PCR8 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
        /* Enable the UART3_RXD function on PTC16 */
        PORTE_PCR9 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;      

        Channel = UART_5;
      break;
      
    default: 
      break;
  }  

  /* Make sure that the transmitter and receiver are disabled while we
  * change settings.
  */
  UART_C2_REG(Channel) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

  /* Configure the UART for 8-bit mode, no parity */
  /* We need all default settings, so entire register is cleared */
  UART_C1_REG(Channel) = 0;
  if((UART0 == UARTChannel) ||(UART1 == UARTChannel))
  {
    SystemClock +=SystemClock;
  }
  /* Calculate baud settings */
  ubd = (U32)((SystemClock)/(BaudRate * 16));

  /* Save off the current value of the UARTx_BDH except for the SBR */
  temp = UART_BDH_REG(Channel) & ~(UART_BDH_SBR(0x1F));
  UART_BDH_REG(Channel) = temp | UART_BDH_SBR(((ubd & 0x1F00) >> 8));
  UART_BDL_REG(Channel) = (U8)(ubd & UART_BDL_SBR_MASK);

  /* Determine if a fractional divider is needed to get closer to the baud rate */
  brfa = (((SystemClock*32)/(BaudRate * 16)) - (ubd * 32));

  /* Save off the current value of the UARTx_C4 register except for the BRFA */
  temp = UART_C4_REG(Channel) & ~(UART_C4_BRFA(0x1F));
  UART_C4_REG(Channel) = temp | UART_C4_BRFA(brfa);
  
  /* Enable receiver and transmitter */
  UART_C2_REG(Channel) |= (UART_C2_TE_MASK | UART_C2_RE_MASK);

  return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_Printf                                                      */
/* Descrp : 处理来自串口的发送中断                                           */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_Printf(const char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_SendString                                                  */
/* Descrp : 发送字符串                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_SendString(char *pt)
{
    while(*pt)
    {
        Uart_SendByte(UART5,*pt++);
    }
    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_SendByte                                                    */
/* Descrp : 发送数据到串口的发送中断                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_SendByte(U8 UARTChannel, U8 Char)
{
    UART_MemMapPtr Channel;
  
    switch (UARTChannel)
    {
        case UART0:
            Channel = UART_0;
            break;
        case UART1:
            Channel = UART_1;
            break;
        case UART2:
            Channel = UART_2;
            break;
        case UART3:
            Channel = UART_3;
            break;
        case UART4:
            Channel = UART_4;
            break;
        case UART5:
            Channel = UART_5;
            break;
        default: 
            break;
    }  
    
    /* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(Channel) & UART_S1_TDRE_MASK))
    {

    }
    /* Send the character */
    UART_D_REG(Channel) = (S8)Char;

    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_GetString                                                   */
/* Descrp : 获取字符串                                                       */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Uart_GetString(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch(UART1))!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte(UART5,c);
        }
    }
    *string='\0';
    Uart_SendByte(UART5,'\n');
    return;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_GetIntNum                                                   */
/* Descrp : 获取数据                                                         */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
SHORT Uart_GetIntNum(void)
{
    char str[30];
    char *string = str;
    unsigned short base     = 10;
    unsigned short minus    = 0;
    short result   = 0;
    short lastIndex;    
    unsigned short i;
    
    Uart_GetString(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}
/***FUNC+*********************************************************************/
/* Name   : Uart_Getch                                                       */
/* Descrp : 接收串口数据                                                     */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
CHAR Uart_Getch(U8 UARTChannel)
{
    UART_MemMapPtr Channel;
  
    switch (UARTChannel)
    {
        case UART0:
            Channel = UART_0;
            break;
        case UART1:
            Channel = UART_1;
            break;
        case UART2:
            Channel = UART_2;
            break;
        case UART3:
            Channel = UART_3;
            break;
        case UART4:
            Channel = UART_4;
            break;
        case UART5:
            Channel = UART_5;
            break;
        default: 
            break;
    }  
    /* Wait until character has been received */
    while (!(UART_S1_REG(Channel) & UART_S1_RDRF_MASK));
    /* Return the 8-bit data from the receiver */
    return UART_D_REG(Channel);
}
/***FUNC+*********************************************************************/
/* Name   : Delay                                                            */
/* Descrp : 延时                                                             */
/* Input  : None.                                                            */   
/* Output : None.                                                            */
/* Return : None.                                                            */   
/***FUNC-*********************************************************************/
void Delay(SHORT sTime)
{
    for(;sTime>0;sTime--)
    {
    }
    return; 
}

void SCI_sendstring(unsigned char *a,unsigned char stringlen)
{
    unsigned char i;
  for(i=0;i<=stringlen;i++)
        Uart_SendByte(UART5,*(a++));
}

unsigned int CRC_Check(unsigned char *Buf, unsigned char crc_count)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<crc_count; i++)
    {
        CRC_Temp ^= Buf[i];
        for(j=0;j<8;j++)
        {
            if (CRC_Temp & 0x01)
            CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
            CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
 
/******************向上位机发送图像*********************/
void put_shangweiji() 
{
     
       Uart_SendByte(UART5,0xFF);  //图像头   
               for(m=0; m<ROW; m++)
               {
                  for(n=0;n<COLUMN1 ;n++)
                  {
                     if(Buffer4[m][n] == 0xFF) 
                       Buffer4[m][n]--;
                     Uart_SendByte(UART5,Buffer4[m][n]);
                  }
                }
} 

  
/************************************************************************************************* 
** 函数名称: putNum( )
** 功能描述: 给串行口写数字
** 输    入:    无
** 输    出:    无 
** 说明：  
*************************************************************************************************/
void putNum( int num)
{  
	U8 i,n[2]={0};
   
     
     n[0]=(num / 100)%10;
     n[1]=(num / 10)%10;
     n[2]=num % 10;
     for(i=0;i<3;i++)
    {
    	Uart_SendByte(UART5,((uchar)n[i]+'0'));
    } 
   // Uart_SendByte(UART4,0X0A);
   // Uart_SendByte(UART4,0X0D);
}


 
 

void WirelessSerial(float a,float b,float c,float d)
{
  int num;
  unsigned char *data;

#if 1
  num = (int)a;    //Speed
  data = (unsigned char *)&num;
  TxBuf[0] = *data;
  data++;
  TxBuf[1] = *data;

  num = (int)b;      //MotorPWM
  data = (unsigned char *)&num;
  TxBuf[2] = *data;
  data++;
  TxBuf[3] = *data;

  num = (int)c;        //angle
  data = (unsigned char *)&num;
  TxBuf[4] = *data;
  data++;
  TxBuf[5] = *data;

  num = (int)d;
  data = (unsigned char *)&num;
  TxBuf[6] = *data;
  data++;
  TxBuf[7] = *data;
#endif

  num = CRC_Check(TxBuf,8);
  TxBuf[8] = num & 0xFF;
  TxBuf[9] = num >> 8;

  SCI_sendstring(TxBuf,9);
}

#ifdef  __cplusplus
}

#endif       