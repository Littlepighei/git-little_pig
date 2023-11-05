#include "main.h"
#include "grab.h"
#include "usart.h"
#include "dji_motor.h"
#include "fixed.h"
#include "basic.h"

/*�������ݣ���λ��xy����Ϊ��0��300�������ڵ������ֱ��150��
  ����xy����Ϊ��0��-300������Ͱ���Ͱ�ھ�160����100*/

int flag=0;
const int allow_err=50;//Լ2.2��

/*
�������ܣ���ɶ���ץȡ��ת�����̶����ͷ�
��Σ����λ�������Ͱ��λ������
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

