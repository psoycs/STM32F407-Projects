#include "main.h"
#include "lcd_16x2.h"
#include "peripheralInit.h"
#include "task.h"
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;
enum state {start, enterPassword, check, alarm, open, resetAlarm} myState;
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init(50);
  MX_I2C1_Init();
  HAL_TIM_Base_Start_IT(&htim2);
  lcd_init();
  myState = start;
  while (1)
  {

  }
}





