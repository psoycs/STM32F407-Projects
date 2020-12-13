/*
 * task.c
 *
 *  Created on: Dec 12, 2020
 *      Author: psoycs
 */
#include "task.h"
#include "main.h"

enum state {start, enterPassword, check, alarm, open, resetAlarm} myState;
int timeout = 0;
int count = 0;
int pressed = 0;
int failCount = 0;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;
unsigned char password[4] = {0,0,0,0};
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  task();
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}
uint8_t b1Read(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,B1_Pin)==GPIO_PIN_SET) {
	if(HAL_GPIO_ReadPin(GPIOD,B1_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,B1_Pin));
		flag=1;
		}
	}
  return flag;
}
uint8_t b2Read(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,B2_Pin)==GPIO_PIN_SET) {
	if(HAL_GPIO_ReadPin(GPIOD,B2_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,B2_Pin));
		flag=1;
		}
	}
  return flag;
}
uint8_t b3Read(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,B3_Pin)==GPIO_PIN_SET) {
	if(HAL_GPIO_ReadPin(GPIOD,B3_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,B3_Pin));
		flag=1;
		}
	}
  return flag;
}
uint8_t b4Read(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,B4_Pin)==GPIO_PIN_SET) {
	if(HAL_GPIO_ReadPin(GPIOD,B4_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,B4_Pin));
		flag=1;
		}
	}
  return flag;
}
void task(void){
	/* USER CODE END WHILE */
  switch (myState) {
	case start:
		lcd_put_cur(0,0);
		lcd_send_string ("Smartlock System");
		lcd_put_cur(1, 3);
		lcd_send_string("PENS 2020");
		if(++timeout == 100){
			lcd_clear();
			myState = enterPassword;
		}
		break;
	case enterPassword:
		lcd_put_cur(0,0);
		lcd_send_string ("Enter Password :");
		if(b1Read()){
			pressed = 1;
			password[count] = 1;
			count++;
			timeout = 0;
		}
		if(b2Read()){
			pressed = 1;
			password[count] = 2;
			count++;
			timeout = 0;
		}
		if(b3Read()){
			pressed = 1;
			password[count] = 3;
			count++;
			timeout = 0;
		}
		if(b4Read()){
			pressed = 1;
			password[count] = 4;
			count++;
			timeout = 0;
		}
		if(pressed){
			for(int i =0; i<count && i<=3 ;i++){
				lcd_put_cur(1, i);
				lcd_send_string ("*");
			}
		}
		if(count <= 3){
			lcd_put_cur(1, count);
			lcd_send_string ("_");
		}
		if(++timeout>40 && count > 0){
			timeout = 0;
			myState = alarm;
			lcd_clear();
			//lcd_put_cur(0, 3);
			//lcd_send_string("TIME OUT !!");
		}
		if(count==4){
			lcd_clear();
			count=0;
			timeout = 0;
			myState = check;
		}
		break;
	case check:
		if(password[0]==1&&password[1]==2&&password[2]==3&&password[3]==4){
			myState = open;
			failCount = 0;
		}
		else{
			if(++failCount>2){
				myState = alarm;
				lcd_clear();
			}
			else{
				//lcd_put_cur(0, 3);
				//lcd_send_string("TRY AGAIN!");
				myState = enterPassword;
				lcd_clear();
			}
		}
		break;
	case alarm:
		//lcd_put_cur(0, 1);
		//lcd_send_string("<Alarm System>");
		//lcd_put_cur(1, 1);
		//lcd_send_string("Access Denied!");
		HAL_GPIO_WritePin(GPIOA, alarm_Pin, GPIO_PIN_SET);
		lcd_clear();
		myState = resetAlarm;
		break;
	case open:
		//lcd_put_cur(0, 1);
		//lcd_send_string("<Alarm System>");
		//lcd_put_cur(1, 1);
		//lcd_send_string("Access Granted");
		HAL_GPIO_TogglePin(GPIOD, doorRelay_Pin);
		myState = enterPassword;
		lcd_clear();
		break;
	case resetAlarm:
		lcd_put_cur(0, 0);
		lcd_send_string("Enter ResetCode");
		if(b1Read()){
			pressed = 1;
			password[count] = 1;
			count++;
			timeout = 0;
		}
		if(b2Read()){
			pressed = 1;
			password[count] = 2;
			count++;
			timeout = 0;
		}
		if(b3Read()){
			pressed = 1;
			password[count] = 3;
			count++;
			timeout = 0;
		}
		if(b4Read()){
			pressed = 1;
			password[count] = 4;
			count++;
			timeout = 0;
		}
		if(pressed){
			for(int i =0; i<count && i<=3 ;i++){
				lcd_put_cur(1, i+6);
				lcd_send_string ("*");
			}
		}
		if(count <= 3){
			lcd_put_cur(1, count+6);
			lcd_send_string ("_");
		}
		if(count==4){
			lcd_clear();
			count=0;
			if(password[0]==1&&password[1]==2&&password[2]==3&&password[3]==4){
				HAL_GPIO_WritePin(GPIOA, alarm_Pin, GPIO_PIN_RESET);
				lcd_clear();
				myState = enterPassword;
				failCount = 0;
			}
		}
		break;
	default:
		break;
  }
}
void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
