/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal.h"
#include "..\MDK-ARM\fonctions_char.h"
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
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char rxBuffer[1];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

void config_GPIO(void);
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
	char msg[] = "\nDebut Transmission : \n";
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	config_GPIO();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,24,10); //message de début
	initLARA(&huart3);
	initConnectionHTTP(&huart3);
  /* USER CODE END 2 */

  /* Infinite loop */
	//sendAT(&huart3, init_AT_command(2, (char*)"AT+CCLK?\r", 50));
	//sendAT(&huart3, init_AT_command(2, (char*)"AT+CCLK?\r", 50));
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void config_GPIO(void){
	GPIO_InitTypeDef maLED;
	
	/* Activation des modules GPIO */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Initialisation des LEDs */
	maLED.Pin = GPIO_PIN_10;
	maLED.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOE, &maLED);
	
	maLED.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &maLED);

	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
}

StatusAT statusAT = EN_COURS;

AT_command currentAT;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

	if (huart->Instance != huart3.Instance){
		return;
	}
	static char staking[RX_BUFFER_SIZE];
	static unsigned int index = 0;
	char flag_reset_index = 'F';
	char flag_end_responce = 0;
	char forDebugONLY = 'o';
	
	
	staking[index] = rxBuffer[0];
	
	/*test fin des reponses*/
	if (currentAT.type == AT_C_UHTTPC){
		if(staking[index-6] == ':'){
			flag_end_responce = 1;
			forDebugONLY = 'C';
		}
	}
	else if((staking[index] == 'K' && staking[index-1] == 'O') || (staking[index] == 'R' && staking[index-1] == 'O'))
		flag_end_responce = 1;
	
	/*Actions grâce aux réponses*/
  if (flag_end_responce){
		/*DEBUG*/
		rxBuffer[0] = forDebugONLY;
		/*END DEBUG*/
		flag_end_responce = 0;
		char reponses[currentAT.nombre_reponses][60];
		volatile unsigned int nb_reponse = currentAT.nombre_reponses;
	
		/* récupération des réponses dans des buffers spécifiques */
		uint8_t cptBuffer=0, cpt = 0;
		for(int i=0 ; i<index+1 ; i++){
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
		for(int i = 0; i < currentAT.nombre_reponses; i++){
				HAL_UART_Transmit(&huart2,(uint8_t*) reponses[i],sizeTabChar(reponses[i]),10);
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
		else if(currentAT.type == AT_C_COPS)
			statusAT = OK;
		
		else if(currentAT.type == AT_C_UHTTPC){
			char tested = reponses[2][15];
			if (tested == '1')
				statusAT = OK;
			else
				statusAT = FAILED;
		}
		
	  //reset---
		for(int index_tab = 0; index_tab < currentAT.nombre_reponses; index_tab++){ //memset ?
			for(int new_index = 0 ; new_index < 60 ; new_index++){
				reponses[index_tab][new_index] = 0x00;
			}
		}
		for(int index_tab = 0; index_tab < RX_BUFFER_SIZE; index_tab++) //memset ?
			staking[index_tab] = 0x00;
		flag_reset_index = 'T';
	  //-------	
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
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	while (1){
		HAL_UART_Transmit(&huart2,(uint8_t*)"Err_Hand l-306",15,1);
		HAL_Delay(1000);
		
		
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
