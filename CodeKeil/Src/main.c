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
#include "..\MDK-ARM\AT_command.h"
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
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
char rxBuffer[1];
StatusAT statusAT = EN_COURS;
AT_command currentAT;
int8_t latitude[12];
int8_t longitude[13];
uint8_t flag_new_data_GPS = 0;
static uint8_t flag_reinit_GPS = 0;
uint8_t flag_call;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
void config_GPIO(void);
void initGPS(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char bufGPS[200];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
	
	char msg[] = "\nDebut Transmission : \n";
	StatusAT initStatus = EN_COURS;
	char debugStop = 0;
	int nb_try = 0;
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
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
	
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,24,10); //message de début
	HAL_UART_Transmit(&huart2,(uint8_t*)"\n---Debut Init---\n\n",21,10); 
	
	
	/*do{
		if (nb_try != 0)
			HAL_UART_Transmit(&huart2,(uint8_t*)"\n- Nouvel init...\n\n",20,10); 
		initLARA(&huart3);
		initStatus = initConnectionHTTP(&huart3);
		nb_try++;
	}while(initStatus == FAILED && nb_try < 10);
	
	if(initStatus == FAILED || initStatus == EN_COURS || nb_try == 10){
		HAL_UART_Transmit(&huart2,(uint8_t*)"\n---Init FAILED---\n\n",21,10);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
		while(1){}
	}
	else{
		HAL_UART_Transmit(&huart2,(uint8_t*)"\n---Init SUCCESS---\n\n",21,10); 
	}*/
	
	initLARA(&huart3);
	configuration_appel(&huart3);
	appel_via_GSM(&huart3);
	
	//initGPS();

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
	//HAL_UART_Receive_IT(&huart6,(uint8_t *)bufGPS,1);
	
	/* Mise en place des appels */ 
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//postGPS(&huart3, "12345678901","123456789012");
	
  while (1){
		/*if(flag_new_data_GPS == 1){
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
			creationFichier(&huart3,latitude,longitude);
			postGPS(&huart3);
			flag_new_data_GPS = 0;
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
		}
		
		if (flag_reinit_GPS){
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
			initGPS();
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
		}
		
		*/
		if(flag_call)
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		
		HAL_Delay(1000);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
}

void initGPS(void)
{
	HAL_UART_Transmit(&huart2,(uint8_t*)"\n***Init-GPS***\n",17,10);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);//RST=1
	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);//RST=0
		
	/*Wake up the module*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart2,(uint8_t*)"done\n",6,10);
	
	flag_reinit_GPS = 0;
}
  /* USER CODE END 3 */


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
  huart6.Init.BaudRate = 4800;
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
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

#define GPS_TEST_TIMEOUT 100

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	static char premierPost = 0;
	static char gpsFail = 0;
	
	if (huart->Instance == huart3.Instance){
		static char staking[RX_BUFFER_SIZE];
		static unsigned short index = 0;
		char flag_reset_index = 'F';
		char flag_end_responce = 0;
		uint8_t flag_URC = 0; 	// Type de réponse : URC
		
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
		
		/* Détection d'un URC */
		else if((staking[3] == '+' || staking[4] == '+') && staking[index] == '\r'){
			flag_end_responce = 1;
			flag_URC = 1;
		}
		/*-------*/
		
		if (flag_end_responce){
			/*Actions grâce aux réponses*/
			flag_end_responce = 0;
			static char reponses[5][TAILLE_REPONSE];
			/* Reset de reponses */
			for(short index_tab = 0; index_tab < 5; index_tab++){ //memset ?
				for(short new_index = 0; new_index < TAILLE_REPONSE; new_index++){
					reponses[index_tab][new_index] = 0x00;
				}
			}
			//volatile unsigned int nb_reponse = currentAT.nombre_reponses;
		
			/* récupération des réponses dans des buffers spécifiques */
			uint8_t cptBuffer=0, cpt = 0;
			for(short i=0 ; i<index+1 ; i++){
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
			
			/* Gestion des URCs */
			if (flag_URC){
				if (reponses[0][1] == 'U'){		// URC de type UCALLSTAT --> Gestion de l'état des appels
					switch(reponses[0][14]){
						case '6':		// appel fini
							flag_call = 0;
							break;
						
						case '7' : 	// Appel qui a marché
							flag_call = 1;
							break;
					}
				}
				flag_URC = 0; 
			}
			
			/* Verification des réponses pour chaque type de commande */
			else if (currentAT.type == AT_OE || currentAT.type == AT_OE_RI){
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
			
				currentAT.type = AT_RI;
			}
			
				
			/* On fait ça de manière à reset le buffer qui va contenir le ok de UDWNFILE */

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
	else if(huart->Instance == huart6.Instance){

		static char trameGlobale[150];
		static char heure[6];

		static unsigned char index=0;
		static unsigned char i=0;
		
		trameGlobale[index]=bufGPS[0];
		index++;
		
		if((trameGlobale[index-2]=='\r') && (trameGlobale[index-1]=='\n'))
		{
			if(strstr((const char*)trameGlobale,"$GPRMC") && flag_new_data_GPS == 0 && flag_reinit_GPS == 0)
			{
				for(i=7;i<15;i++)
					heure[i-7]=trameGlobale[i];
				for(i=20;i<31;i++)
					latitude[i-20]=trameGlobale[i];
				for(i=32;i<44;i++)
					longitude[i-32]=trameGlobale[i];
				
				/*for(i=0;i<2;i++)
					latitudeConvpartieun[i]=latitude[i];
				for(i=2;i<8;i++)
					latitudeConvpartiedeux[i]=latitude[i];
				
				for(i=0;i<3;i++)
					longitudeConvpartieun[i]=longitude[i];
				for(i=3;i<9;i++)
					longitudeConvpartiedeux[i]=longitude[i];
				
				latitudeFinale=atoi(latitudeConvpartieun)+atof(latitudeConvpartiedeux)/60;
				longitudeFinale=atoi(longitudeConvpartieun)+atof(longitudeConvpartiedeux)/60;
				
				sprintf(l,"%f %c",latitudeFinale,trameGlobale[25]);
				sprintf(L,"%f %c",longitudeFinale,trameGlobale[37]);*/
				
				HAL_UART_Transmit(&huart2,(uint8_t *)heure,6,100);
				uartEndLine(&huart2);
				HAL_UART_Transmit(&huart2,(uint8_t *)latitude,11,100);
				uartEndLine(&huart2);
				HAL_UART_Transmit(&huart2,(uint8_t *)longitude,12,100);
				uartEndLine(&huart2);
				uartEndLine(&huart2);
				
				if (!premierPost)
					HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
				/*for debug only
				char * la = "4451.1810,N";
				for(int i = 0; i < 13; i++)
					latitude[i] = la[i];
				char* lo =  "00033.8545,W";
				for(int i = 0; i < 14; i++)
					longitude[i] = lo[i];
				end debug only*/
				
				flag_new_data_GPS = 1;
				if(longitude[11] != 'W'){
					flag_new_data_GPS = 0;
					gpsFail++;
				}
				else if (!premierPost){
					premierPost = 1;
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
				}
				if (gpsFail > GPS_TEST_TIMEOUT && premierPost == 0){
					flag_reinit_GPS = 1;
					gpsFail = 0;
				}
				
			}
			index=0;
			memset(trameGlobale, 0, sizeof(trameGlobale));
		}
		HAL_UART_Receive_IT(huart,(uint8_t *) bufGPS,1);
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
	HAL_UART_Transmit(&huart2,(uint8_t*)"Err_Hand l-306",15,1);
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
