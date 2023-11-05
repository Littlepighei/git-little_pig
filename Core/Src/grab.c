#include "main.h"
#include "grab.h"
#include "usart.h"
#include "dji_motor.h"
#include "fixed.h"
#include "basic.h"

/*任务内容：将位于xy坐标为（0，300）的置于地面的球（直径150）
  放至xy坐标为（0，-300）的球桶里，球桶内径160，高100*/

int flag=0;
const int allow_err=50;//约2.2度

/*
函数功能：完成定点抓取球并转移至固定点释放
入参：球的位置坐标和桶的位置坐标
*/
void Transfer_the_ball(int x,int y,int z,int x1,int y1,int z1)
{
    if(flag==0)
    {
        Draw_target_loc(x,y,z);
        Touch_fixed_point(loc1,loc2,loc3);
//       USART_printf("%d,%d,%d",Get_dji_information(k).angle,
//                    Get_dji_information(m).angle,Get_dji_information(n).angle);
    }
    if(Basic_int_abs(Get_dji_information(k).angle-loc1)<allow_err&&
       Basic_int_abs(Get_dji_information(m).angle-loc2)<allow_err&&
       Basic_int_abs(Get_dji_information(n).angle-loc3)<allow_err)
      {
          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
          Draw_target_loc(x1,y1,z1);
          Touch_fixed_point(loc1,loc2,loc3);
          flag=1;
          if(Basic_int_abs(Get_dji_information(k).angle-loc1)<allow_err&&
             Basic_int_abs(Get_dji_information(m).angle-loc2)<allow_err&&
             Basic_int_abs(Get_dji_information(n).angle-loc3)<allow_err)
            {
                HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
                flag=2;
            }
      }
}

