#include "main.h"
#include "fixed.h"
#include "usart.h"
#include "dji_motor.h"

/*任务内容：机械臂末端，选定特殊点？
  打三个固定点: (300,0,0),(200,50,150),(-150,150,50)*/

int k=0;
int m=1;
int n=2;//motor_id
int loc1=0,loc2=0,loc3=0;
const float PI=3.1415926;
const float RATIO=3591/187;

/*
函数功能：计算打印目标点各个电机需要计算的角度
入参：串口输入目标点的坐标x,y,z
*/
void Draw_target_loc(int x,int y,int z)
{
    int angle1=0,angle2=0,angle3=0;
    //缺少逆解代码
    loc1=RATIO*angle1*8191/(2*PI);
    loc2=RATIO*angle2*8191/(2*PI);
    loc3=RATIO*angle3*8191/(2*PI);
    USART_printf("%d,%d,%d",loc1,loc2,loc3);
}

/*
函数功能：转动电机完成打固定点动作
入参：各个电机需转动到的角度位置
*/
void Touch_fixed_point(int loc1,int loc2,int loc3)
{
    Change_dji_loc(k,loc1);
    Change_dji_loc(m,loc2);
    Change_dji_loc(n,loc3);
}