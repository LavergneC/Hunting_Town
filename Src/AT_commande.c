#include "AT_command.h"

extern UART_HandleTypeDef huart2;

int sizeTabChar(char * s){
	int lenght = 0;
	while (s[lenght] != '\0')
		lenght++;
	return lenght;
}

void sendAT(UART_HandleTypeDef* huart, char command[], int nbRep, int taille_max){
	char rxBuff[taille_max];

	if (nbRep < 0)
		return;
	
	HAL_UART_Transmit(huart,(uint8_t*)command,sizeTabChar(command),10);
	uartEndLine(&huart2);
	
	for(int nb_reponse = 0; nb_reponse < nbRep; nb_reponse++){
		HAL_UART_Receive(huart, (uint8_t*)rxBuff,taille_max,1000);
		HAL_UART_Transmit(&huart2,(uint8_t*)rxBuff,taille_max,10);
		uartEndLine(&huart2);
	}
}

void uartEndLine(UART_HandleTypeDef *huart){
	char n[] = {'\n'};
	
	HAL_UART_Transmit(huart,(uint8_t*)n,1,10);
	
	return;
}
void initLARA(UART_HandleTypeDef *huart){
	int nbCommand = 5;
	AT_command initsCommands[nbCommand];
	int num_commande;
	
	AT_command monAT;
	monAT.command = "bla";
	monAT.nombre_reponses = 2;
	monAT.taille_max_reponses = 80;
	initsCommands[0] = monAT;
	
	for(num_commande = 0; num_commande < nbCommand; num_commande++){
		AT_command currentAT = initsCommands[num_commande];
		sendAT(huart, currentAT.command, currentAT.nombre_reponses, currentAT.taille_max_reponses);
	}
}