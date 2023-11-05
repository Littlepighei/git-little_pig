#ifndef BASIC_ACTION
#define BASIC_ACTION

#include "main.h"

#define START 1
#define END 0

typedef struct 
{
  uint8_t  ifstart;
	int      motor_id;
	int      target_loc;  
	void   (*Next_action)(void);//º¯ÊýÖ¸Õë
}action_member;


/**************USER_begin**************/
void Begin_action_one(void);
void Action_one_content(void);
/**************USER_end**************/

#endif
