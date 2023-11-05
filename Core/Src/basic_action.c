#include "basic_action.h"
#include "main.h"
#include "dji_motor.h"
#include "basic.h"
#include "usart.h"

/**************内部宏定义与重命名begin**************/

/**************内部宏定义与重命名end**************/

/**************内部变量与函数begin**************/

static uint8_t Arrive_motor_target_loc(int motor_id,int target_angle);
static void change_next(void);
const int equal_allow_err=100;
static action_member action_one_s[]={
	/*      开始标志   电机ID      目标转程   下个动作函数*/
	/*1*/{		 END,        0,        8192    ,NULL},
	/*2*/{		 END,        1,        8192*2  ,NULL},
	/*3*/{		 END,        5,        8192*3  ,NULL},
	/*1*/{	     END,        0,        0       ,NULL},
	/*2*/{		 END,        1,        0       ,NULL},
	/*3*/{		 END,        5,        0       ,change_next},
	/*3*/{		 END,        5,        0       ,change_next},
};
static int action_one_state_num=sizeof(action_one_s) / sizeof(action_one_s[0]);//状态总数
/**************内部变量与函数end**************/


/**************外部接口begin**************/
void Begin_action_one(void);
void Action_one_content(void);
/**************外部接口end**************/


static uint8_t Arrive_motor_target_loc(int motor_id,int target_angle){
	if (Basic_int_abs(Get_dji_information(motor_id).total_angle-target_angle)<equal_allow_err){
		return 1;
	}
	else{
		return 0;
	}
}

static void change_next(void){
	USART_printf("over\n");
}

/*
1.函数功能：
2.入参：
3.返回值：
4.用法及调用要求：
5.其它：
*/
void Begin_action_one(void){
	action_one_s[0].ifstart=START;
}
/*
1.函数功能：
2.入参：
3.返回值：
4.用法及调用要求：
5.其它：
*/
void Action_one_content(void){
	static int state_cnt=0;
	if(action_one_s[state_cnt].ifstart==START){
		Change_dji_loc(action_one_s[state_cnt].motor_id,action_one_s[state_cnt].target_loc);
		while(state_cnt<=action_one_state_num){
			if(Arrive_motor_target_loc(action_one_s[state_cnt].motor_id,action_one_s[state_cnt].target_loc)){
				action_one_s[state_cnt].ifstart=END;
				if(action_one_s[state_cnt].Next_action!=NULL){
					action_one_s[state_cnt].Next_action();
				}
				if(state_cnt<action_one_state_num-1){
					action_one_s[++state_cnt].ifstart=START;
				}
				else{//所有状态结束
					state_cnt=0;
				}
			}
			else{
				break;
			}
		}
	}
}

