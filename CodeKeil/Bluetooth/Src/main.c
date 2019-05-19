/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../MDK-ARM/nrf.h"
#include "../MDK-ARM/AT_command.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TAILLE_REPONSE 200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
AT_command currentAT;
StatusAT statusAT = EN_COURS;
char rxBuffer[1];

struct nrf_tx tx;
static uint8_t valueBluetooth = NRF_DATA_DEFAULT;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
static void GPIO_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	volatile int setupStatus;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	__HAL_RCC_SPI1_CLK_ENABLE();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	GPIO_Init();

	/* init module 4g */
	initLARA(&huart6);
	initConnection(&huart6);
	
	
	/* precedure de reset */ 
	HAL_GPIO_WritePin(GPIOE, NRF_PIN_RST, GPIO_PIN_RESET);
  HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOE, NRF_PIN_RST, GPIO_PIN_SET); 
	HAL_Delay(100);

	setupStatus = nrf_setup();
	if (setupStatus < 0){
		HAL_UART_Transmit(&huart2,(uint8_t *) "ERROR\n\r",8, 10);
		Error_Handler();
	}
	HAL_UART_Transmit(&huart2,(uint8_t *) "OK\n\r",5, 10); 
	
	memset(&tx, 0, sizeof(tx));
	
	struct nrf_rx rx;
	memset(&rx, 0, sizeof(rx));
	
	nrf_advertise();
	for(unsigned char i = 0; i < 3; i++){
		nrf_receive(&rx);
		nrf_parse(&rx);
		memset(&rx, 0, sizeof(rx));
		HAL_Delay(40);
	}	
	
	nrf_manage_tx(NRF_DATA_DEFAULT);
	HAL_Delay(100);
	nrf_manage_tx(NRF_DATA_DEFAULT);
	HAL_Delay(1000);
	nrf_manage_tx(NRF_DATA_DEFAULT);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		/* Partie Bluetooth */
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET){
			valueBluetooth = 0x55;
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET);
		}
		
		/* Partie 4G */
		if(valueBluetooth != NRF_DATA_DEFAULT){
			nrf_manage_tx(valueBluetooth);
			valueBluetooth = NRF_DATA_DEFAULT;
		}
		
		HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
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

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE12 PE14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart6.Instance){
		static char staking[RX_BUFFER_SIZE];
		static unsigned short index = 0;
		char flag_reset_index = 'F';
		char flag_end_responce = 0;
		
		staking[index] = rxBuffer[0];
		
		/*test fin des reponses*/
		if (currentAT.type == AT_C_UHTTPC){
			if(staking[index-6] == ':'){
				flag_end_responce = 1;
			}
		}
		else if(currentAT.type == AT_C_UDWNFILE){
				if (staking[index] == '>')
					flag_end_responce = 1;
		}
		else if((staking[index] == 'K' && staking[index-1] == 'O') || (staking[index] == 'R' && staking[index-1] == 'O'))
			flag_end_responce = 1;
		/*-------*/
		
		if (flag_end_responce){
			/*Actions grâce aux réponses*/
			flag_end_responce = 0;
			static char reponses[5][TAILLE_REPONSE];
			/* Reset de reponse*/
			for(short index_tab = 0; index_tab < 5; index_tab++){ //memset ?
				for(short new_index = 0; new_index < TAILLE_REPONSE ; new_index++){
					reponses[index_tab][new_index] = 0x00;
				}
			}
			//volatile unsigned int nb_reponse = currentAT.nombre_reponses;
		
			/* récupération des réponses dans des buffers spécifiques */
			uint8_t cptBuffer=0, cpt = 0;
			for(unsigned short i=0 ; i<index+1 ; i++){
				if(staking[i] == '\r' || staking[i] == '\n' || staking[i] == '\0' || staking[i] == 0x0D){
					if(cpt != 0){
						reponses[cptBuffer][cpt+1] = '\0';
						cptBuffer+=1;
						cpt = 0;
					}
				}
				else{
					reponses[cptBuffer][cpt] = staking[i];
					cpt+=1;
				}
			}
		 /* ---	---	---	---	---	---	---	---		---	---		---	---	*/
			
			/* Affichage des réponses */
			for(short i = 0; i < currentAT.nombre_reponses; i++){
					HAL_UART_Transmit(&huart2,(uint8_t*) reponses[i],sizeTabChar(reponses[i]),200);
					uartEndLine(&huart2);
			}
			uartEndLine(&huart2);
			
			/* Verification des réponses pour chaque type de commande */
			if (currentAT.type == AT_OE || currentAT.type == AT_OE_RI){
				if (tabsEquals(reponses[1],"OK\0"))
					statusAT = OK;
				else
					statusAT = FAILED;
			}
			else if (currentAT.type == AT_RI_OE){
				if(tabsEquals(reponses[2], "OK\0"))
					statusAT = OK;
				else
					statusAT = FAILED;
			}
			else if (currentAT.type == AT_C_CPIN){
				if (tabsEquals(reponses[1], "+CPIN: READY\0"))
					statusAT = OK;
				else
					statusAT = FAILED;
			}
			else if(currentAT.type == AT_C_COPS || currentAT.type == AT_RI){
				statusAT = OK;
			}
			else if(currentAT.type == AT_C_UHTTPC){
				if (reponses[2][15] == '1')
					statusAT = OK;
				else
					statusAT = FAILED;
			}
			else if (currentAT.type ==  RI_AT_C_UHTTPC){
				if (reponses[3][15] == '1')
					statusAT = OK;
				else
					statusAT = FAILED;
			}
			else if (currentAT.type == AT_C_UDWNFILE){
				statusAT = OK;
				
				/* On fait ça de manière à reset le buffer qui va contenir le ok de UDWNFILE */
				currentAT.type = AT_RI;
			}
			else if(currentAT.type == AT_C_UFTPC){
				if (reponses[2][12] == '1')
					statusAT = OK;
				else
					statusAT = FAILED;
			}
			else if(currentAT.type == AT_C_URDFILE){
				if(tabsEquals(reponses[2], "OK")){
					statusAT = OK;
					uint8_t value = reponses[1][30] - 48;
					if (valueBluetooth != value)
						valueBluetooth = value;
				}
				else
					statusAT = FAILED;
			}
			/* Reset des buffers*/
			for(short index_tab = 0; index_tab < RX_BUFFER_SIZE; index_tab++) //memset ?
				staking[index_tab] = 0x00;
			flag_reset_index = 'T';
			/* - - - - - - - - */
		}
		HAL_UART_Receive_IT(huart, (uint8_t *)rxBuffer, 1);
		
		if (flag_reset_index == 'T'){
			flag_reset_index = 'F';
			index = 0;
		}
		else
			index++;

		rxBuffer[0] = 0x00;
}
}
	
static void GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(300);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(100);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
