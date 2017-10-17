#include "K60_datatype.h"

/*************************************************************************
*  函数名称:gabs_char
*  功能说明:取绝对值
*  参数说明:
*  函数返回:
*  修改时间:
*  备    注:
*************************************************************************/
unsigned int gabs_char(int ek)  
{
   if(ek<0)ek=-ek; 
   return ek;
}

/*************************************************************************
*  函数名称:duoji_control
*  功能说明:舵机PID算法
*  参数说明:
*  函数返回:
*  修改时间:
*  备    注:
*************************************************************************/

void duoji_control()
{
    int p=10,d=2;                               //PD控制
    int x1,x2,x0;
    x0=(int)(Middle_line[25]+Middle_line[26]+Middle_line[27])/3;        
    x1=(int)(Middle_line[28]+Middle_line[29]+Middle_line[30])/3;
    x2=(int)(Middle_line[31]+Middle_line[32]+Middle_line[33])/3;
   
    //求取三行中线的平均值
    
    error_duoj0=x0-69;                          //计算中线与中值的偏差  
    error_duoj1=x1-69;
    error_duoj2=x2-69;
    error_duoj=(int)(error_duoj1*0.2+error_duoj2*0.3+error_duoj0*0.5);     //偏差乘各段比例系数

    error[1]=error[0];
    error[0]=error_duoj;                        //记录偏差
    
    dj_p=(error_duoj*error_duoj)/p;  
    dj_d=(error[0]-error[1])*d;
    
    now_turn=(int)(dj_p+dj_d);
    
    JD=Douji_Mid+now_turn;   
    
    if (error_duoj<0)
    JD=Douji_Mid-now_turn;
    if (error_duoj==0)
    JD=Douji_Mid;
    if (error_duoj>0)
    JD=Douji_Mid+now_turn;
         
    if(JD>Douji_Max)JD=Douji_Max ;      //转角保护
    if(JD<Douji_Min)JD=Douji_Min;

    SteeringPWM =JD ;
                                        //最终输出在中断函数实现，函数在/vector.c下

}
