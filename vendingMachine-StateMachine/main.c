#include "main.h"

enum state {s0,s1,s2,s3,b500,b1000,b500k,b1000k} stateku;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/*input section */
uint8_t c500Detect(void);
uint8_t c1000Detect(void);
uint8_t proses(void);
uint8_t batal(void);
/*output section*/
void drop(void);
void kembali500(void);
void kembali1000(void);
void sensorRead(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  stateku=s0;
  while (1)
  {
	  switch (stateku) {
		case s0:
			if(c500Detect()){
				stateku=s1;
			}
			else if(c1000Detect()){
				stateku=s2;
			}
			break;
		case s1:
			if(c500Detect()){
				stateku=s2;
			}
			else if(batal()){
				stateku=b500k;
			}
			break;
		case s2:
			if (proses()) {
				stateku=s3;
			}
			else if(batal()){
				stateku=b1000k;
			}
			else if(c500Detect()){
				stateku=b500;
			}
			else if(c1000Detect()){
				stateku=b1000;
			}
			break;
		case s3:
			drop();
			stateku=s0;
			break;
		case b500:
			kembali500();
			stateku=s2;
			break;
		case b1000:
			kembali1000();
			stateku=s2;
			break;
		case b500k:
			kembali500();
			stateku=s0;
			break;
		case b1000k:
			kembali1000();
			stateku=s0;
			break;
		default:
			break;
	}
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = c500_Pin|c1000_Pin|process_Pin|batal_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
uint8_t c1000Detect(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,c1000_Pin)==GPIO_PIN_SET) {
	  HAL_Delay(20);
	if(HAL_GPIO_ReadPin(GPIOD,c1000_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,c1000_Pin));
		flag=1;
		}
	}
  return flag;
}
uint8_t c500Detect(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,c500_Pin)==GPIO_PIN_SET) {
	  HAL_Delay(20);
	if(HAL_GPIO_ReadPin(GPIOD,c500_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,c500_Pin));
		flag=1;
		}
	}
  return flag;
}
uint8_t proses(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,process_Pin)==GPIO_PIN_SET) {
	  HAL_Delay(20);
	if(HAL_GPIO_ReadPin(GPIOD,process_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,process_Pin));
		flag=1;
		}
	}
  return flag;
}
uint8_t batal(void){
int flag=0;
  if(HAL_GPIO_ReadPin(GPIOD,batal_Pin)==GPIO_PIN_SET) {
	  HAL_Delay(20);
	if(HAL_GPIO_ReadPin(GPIOD,batal_Pin)) {
		while(HAL_GPIO_ReadPin(GPIOD,batal_Pin));
		flag=1;
		}
	}
  return flag;
}
void sensorRead(void){
	c500Detect();
	c1000Detect();
	batal();
	proses();
}
void kembali500(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
}
void kembali1000(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
}
void drop(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
}
void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif

