#include "AT_command.h"

extern UART_HandleTypeDef huart2;
extern char rxBuffer[RX_BUFFER_SIZE];
extern AT_command currentAT;
extern StatusAT statusAT;

int sizeTabChar(char * s){
	int lenght = 0;
	while (s[lenght] != '\0')
		lenght++;
	return lenght;
}

void sendAT(UART_HandleTypeDef* huart, AT_command at_command){
	char buff[at_command.taille_max_reponses];
	int time_out = 10;
	int count_time_out = 0;

	if (at_command.nombre_reponses < 0)
		return;
	
	//vidage buffer
	HAL_UART_Receive(huart, (uint8_t*)buff,at_command.taille_max_reponses,10);
	HAL_Delay(50);
	//-----
	
	HAL_UART_Receive_IT(huart, (uint8_t *)rxBuffer, 1);
	
	while (statusAT != OK && count_time_out < time_out){
		HAL_UART_Transmit(huart,(uint8_t*)at_command.command,sizeTabChar(at_command.command),10);
		HAL_Delay(250);
		count_time_out++;
	}
	
//	for(uint8_t nb_reponse = 0; nb_reponse < nbRep; nb_reponse++){
//		memset(rxBuff, 0x00, taille_max);
//		HAL_UART_Receive(huart, (uint8_t*)rxBuff, taille_max,500);
//		HAL_UART_Transmit(&huart2,(uint8_t*)rxBuff,taille_max,10);
//	}
}

void uartEndLine(UART_HandleTypeDef *huart){
	char n[] = {'\n'};
	
	HAL_UART_Transmit(huart,(uint8_t*)n,1,10);
	
	return;
}
void initLARA(UART_HandleTypeDef *huart){
	int nbCommand = 3;
	AT_command initsCommands[nbCommand];
	int num_commande;
	
	// Code Pin
	//AT_command monAT = init_AT_command(1,"AT+CPIN=\"0264\"\r", 50);
	initsCommands[0] = init_AT_command(1,"AT+CPIN=\"0264\"\r", 50);
	
	initsCommands[1] = init_AT_command(1,"AT+CPIN?\r", 50);
	
	// Mode full fonctionnality
	initsCommands[2] = init_AT_command(1,"AT+CFUN=1\r", 50);
	
	for(num_commande = 0; num_commande < nbCommand; num_commande++){
		currentAT = initsCommands[num_commande];
		sendAT(huart, currentAT);
	}
}

void initConnectionHTTP(UART_HandleTypeDef *huart){
	int nbCommand = 8;
	AT_command initsCommands[nbCommand];
	
	/* Config réseau */
	
	// Automatic network registration
	initsCommands[0] = init_AT_command(1, "AT+COPS=0\r", 50);
	
	// On active le contexte PDP --> connection à l'internet
	initsCommands[1] = init_AT_command(1, "AT+UPSDA=0,3\r", 50);
	
	/* Gestion de l'HTTP */
	
	// Reset de l'environnement HTTP
	initsCommands[2] = init_AT_command(1, "AT+UHTTP=0\r", 100);
	
	// Renseignement du nom du serveur
	initsCommands[3] = init_AT_command(1, "AT+UHTTP=0,1,\"ptsv2.com\"\r", 50);
	
	// Renseignement du port de communication HTTP
	initsCommands[4] = init_AT_command(1, "AT+UHTTP=0,5,80\r", 50);
	
	// Résolution DNS à partir du nom du serveur
	initsCommands[5] = init_AT_command(1, "AT+UDNSRN=0,\"ptsv2.com\"\r", 80);
	
	initsCommands[6] = init_AT_command(2, "AT+UHTTPC=0,1,\"/t/cn00t-1548418994/post\",\"filename\"\r", 150);
	
	initsCommands[7] = init_AT_command(5, "AT+UPING=\"www.google.com\"\r", 100);
	
	for(unsigned int num_commande = 0; num_commande < nbCommand; num_commande++){
		currentAT = initsCommands[num_commande];
		sendAT(huart, currentAT);
		HAL_Delay(35);
	}
}

AT_command init_AT_command(int nombre_reponses, char * command, int taille_max_reponses){
	AT_command mon_AT;
	mon_AT.command = command;
	mon_AT.nombre_reponses=nombre_reponses;
	mon_AT.taille_max_reponses=taille_max_reponses;
	// TRES PROVISOIRE ! ! !
	mon_AT.type = AT_RI_OE;
	//-----
	return mon_AT;
}

/*void RX_interruption(){
	if(flag){
			switch(etat){
				case 
			}
	}
}*/
