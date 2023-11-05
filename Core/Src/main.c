/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can_database.h"
#include "com_chassis.h"
#include "pid.h"
#include "dji_motor.h"
#include "basic_action.h"
#include "fixed.h"
#include "grab.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t time[4]={0};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_UART7_Init();
  MX_CAN2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  Can_start_work();
  Pid_parameter_init();
  HAL_UART_Receive_IT(&huart1,&recv_data,1);
  HAL_UART_Receive_IT(&huart7,&recv_data,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  USART_printf("ok\n");
  while (1)
  {
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      Change_dji_speed(1,50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 90;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    static int x=0,y=0,z=0,x1=0,y1=0,z1=0;//前后分别描述点/球的位置、桶的位置
    static int i=0;
    static int mark=0;//用于判断执行3or4
    static char ball[13]={'0'};
    //视觉串口返回球/桶的数据存入数组;约定视觉通过串口发送球数据格式为#数数数#数数数#数数数#,桶数据格式为!数数数!数数数!数数数!
    if(huart->Instance==UART7){
      //任务1部分
      if(recv_data=='a')
      {
//          x=300;y=0;z=0;
//          Draw_target_loc(x,y,z);
          Touch_fixed_point(8191,8191*2,8191*3);
      }
      else if(recv_data=='b')
      {
          x=200;y=50;z=150;
          Draw_target_loc(x,y,z);
          Touch_fixed_point(loc1,loc2,loc3);
      }
      else if(recv_data=='c')
      {
          x=-150;y=-150;z=50;
          Draw_target_loc(x,y,z);
          Touch_fixed_point(loc1,loc2,loc3);
      }
      //任务2部分
      else if(recv_data=='s')
      {
          while(flag==0||flag==1)
          {
                x=0;y=300;z=75;
                x1=0;y1=-300;z1=175;
                Transfer_the_ball(x,y,z,x1,y1,z1);
          }
          flag=0;
      }
      HAL_UART_Receive_IT(&huart7,&recv_data,1);
    }
    else if(huart->Instance==USART1){
      //任务3部分
      if(((recv_data>'0'&&recv_data<'9')||recv_data=='#')&&mark==0)
      {
          if(i<13)
          {
                ball[i]=recv_data;
                i++;
          }
          if(ball[12]=='#')
          {
              x=(ball[1]-'0')*100+(ball[2]-'0')*10+(ball[3]-'0')*1;
              y=(ball[5]-'0')*100+(ball[6]-'0')*10+(ball[7]-'0')*1;
              z=(ball[9]-'0')*100+(ball[10]-'0')*10+(ball[11]-'0')*1;
              USART_printf("x=%d,y=%d,z=%d",x,y,z);
              x1=0;y1=-300;z1=175;
              while(flag==0||flag==1)
              {
                  Transfer_the_ball(x,y,z,x1,y1,z1);
              }
              flag=0;
              i=0;
              ball[12]='0';
              mark=1;
          }
      }
      //任务4部分
      else if((recv_data=='#'||recv_data=='!'||(recv_data<'9'&&recv_data>'0'))&&mark==1)
      {
          if(i<13)
          {
              ball[i]=recv_data;
              i++;
          }
          if(i==13&&ball[12]=='#')
          {
              x=(ball[1]-'0')*100+(ball[2]-'0')*10+(ball[3]-'0')*1;
              y=(ball[5]-'0')*100+(ball[6]-'0')*10+(ball[7]-'0')*1;
              z=(ball[9]-'0')*100+(ball[10]-'0')*10+(ball[11]-'0')*1;
              //缺少坐标转换
              USART_printf("ball:%d,%d,%d",x,y,z);
              i=0;
              ball[12]='0';
          }
          else if(i==13&&ball[12]=='!')
          {
              x1=(ball[1]-'0')*100+(ball[2]-'0')*10+(ball[3]-'0')*1;
              y1=(ball[5]-'0')*100+(ball[6]-'0')*10+(ball[7]-'0')*1;
              z1=(ball[9]-'0')*100+(ball[10]-'0')*10+(ball[11]-'0')*1;
              //缺少坐标转换
              USART_printf("pail:%d,%d,%d",x1,y1,z1);
              while(flag==0||flag==1)
              {
                  Transfer_the_ball(x,y,z,x1,y1,z1);
              }
              flag=0;
          }
      }
      HAL_UART_Receive_IT(&huart1,&recv_data,1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
