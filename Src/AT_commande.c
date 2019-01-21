#include "AT_command.h"

extern UART_HandleTypeDef huart2;

int sizeTabChar(char * s){
	int lenght = 0;
	while (s[lenght] != '\0')
		lenght++;
	return lenght;
}

void sendAT(UART_HandleTypeDef* huart, char command[], int nbRep){
	char rxBuff[RX_BUFFER_SIZE];

	if (nbRep < 0)
		return;
	
	HAL_UART_Transmit(huart,(uint8_t*)command,sizeTabChar(command),10);
	uartEndLine(&huart2);
	
	for(int nb_reponse = 0; nb_reponse < nbRep; nb_reponse++){
		HAL_UART_Receive(huart, (uint8_t*)rxBuff,RX_BUFFER_SIZE,1000);
		HAL_UART_Transmit(&huart2,(uint8_t*)rxBuff,RX_BUFFER_SIZE,10);
		uartEndLine(&huart2);
	}
}

void uartEndLine(UART_HandleTypeDef *huart){
	char n[] = {'\n'};
	
	HAL_UART_Transmit(huart,(uint8_t*)n,1,10);
	
	return;
}