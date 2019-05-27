#include "gps.h"

extern uint8_t flag_reinit_GPS;
extern uint8_t flag_new_data_GPS;

void initGPS(UART_HandleTypeDef *huart)
{
	HAL_UART_Transmit(huart,(uint8_t*)"\n***Init-GPS***\n",17,10);
	HAL_GPIO_WritePin(GPIOE,GPS_RST, GPIO_PIN_SET);//RST=1
	
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	
	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOE, GPS_RST, GPIO_PIN_RESET);//RST=0
	
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
		
	/*Wake up the module*/
	HAL_GPIO_WritePin(GPIOB, GPS_WAKE_UP, GPIO_PIN_RESET);
  HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPS_WAKE_UP, GPIO_PIN_SET);
	
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOB, GPS_WAKE_UP, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_UART_Transmit(huart,(uint8_t*)"done\n",6,10);
	
	flag_reinit_GPS = 0;
}

void parse_trame(UART_HandleTypeDef *huart, char* trameGlobale, char* latitude, char* longitude, uint8_t *gpsFail, uint8_t *premierPost)
{
	int i = 0;

	for(i=20;i<31;i++)
		latitude[i-20]=trameGlobale[i];
	for(i=32;i<44;i++)
		longitude[i-32]=trameGlobale[i];

	HAL_UART_Transmit(huart,(uint8_t *)latitude,11,100);
	uartEndLine(huart);
	HAL_UART_Transmit(huart,(uint8_t *)longitude,12,100);
	uartEndLine(huart);
	uartEndLine(huart);
	
	flag_new_data_GPS = 1;
	if(longitude[11] != 'W'){
		flag_new_data_GPS = 0;
		*gpsFail += 1;
	}
	else if (!premierPost){
		*premierPost = 1;
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
	if (*gpsFail > GPS_TEST_TIMEOUT && *premierPost == 0){
		flag_reinit_GPS = 1;
		*gpsFail = 0;
	}
	
}
