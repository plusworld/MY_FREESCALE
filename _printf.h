

/*    .h文件的固定格式：

#ifndef __K60_PRINTF_H__
#define __K60_PRINTF_H__

加上声明的部分；

#endif
*/


#ifndef __K60_PRINTF_H__
#define __K60_PRINTF_H__

#define UART_0              (UART0_BASE_PTR)
#define UART_1              (UART1_BASE_PTR)
#define UART_2              (UART2_BASE_PTR)
#define UART_3              (UART3_BASE_PTR)
#define UART_4              (UART4_BASE_PTR)
#define UART_5              (UART5_BASE_PTR)


#define  BAUD_RATE_115200          (115200)
#define  BAUD_RATE_57600           (57600)
#define  BAUD_RATE_38400           (38400)
#define  BAUD_RATE_28800           (28800)
#define  BAUD_RATE_19200           (19200)
#define  BAUD_RATE_14400           (14400)
#define  BAUD_RATE_9600            (9600)

typedef enum
{
    UART0 = 0,
    UART1 = 1,
    UART2 = 2,
    UART3 = 3,
    UART4 = 4,
    UART5 = 5
}eUARTChannel;

void UART_Init(U32 SystemClock, U32 BaudRate, U8 UARTChannel);
void Uart_Printf(const char *fmt,...);
void Uart_SendString(char *pt);

void Uart_SendByte(U8 UARTChannel, U8 Char);

void Uart_GetString(char *string);
SHORT Uart_GetIntNum(void);
void Delay(short sTime);
CHAR Uart_Getch(U8 UARTChannel);
void WirelessSerial(float a,float b,float c,float d);
void putNum( int num);
void put_shangweiji(); 

#endif