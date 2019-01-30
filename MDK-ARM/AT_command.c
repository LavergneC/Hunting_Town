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
	char buff[100];
	int time_out = 5;
	int count_time_out = 0;
  statusAT = EN_COURS;
	
	if (at_command.temps_reponse > 999)
		time_out = 2;
	
	//vidage buffer
	HAL_UART_Receive(huart, (uint8_t*)buff,100,10);
	HAL_Delay(50);
	//-----
	
	HAL_UART_Receive_IT(huart, (uint8_t *)rxBuffer, 1);
	while (statusAT != OK && count_time_out < time_out){
		HAL_UART_Transmit(huart,(uint8_t*)at_command.command,sizeTabChar(at_command.command),10);
		HAL_Delay(at_command.temps_reponse);
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
	int timeout = 20;
	int nb_init = 0;
	
	// Code Pin
	//AT_command monAT = init_AT_command(1,"AT+CPIN=\"0264\"\r", 50);

	initsCommands[0] = init_AT_command(2,"AT+CPIN=\"0264\"\r", 50, AT_OE, 250);
	
	initsCommands[1] = init_AT_command(3,"AT+CPIN?\r", 50, AT_C_CPIN, 250);
	
	// Mode full fonctionnality

	initsCommands[2] = init_AT_command(2,"AT+CFUN=1\r", 50, AT_OE, 250);
	
	do{
		for(num_commande = 0; num_commande < nbCommand; num_commande++){
			currentAT = initsCommands[num_commande];
			sendAT(huart, currentAT);
		}
		HAL_Delay(100);
		nb_init++;
		if (statusAT == OK)
			HAL_UART_Transmit(&huart2,(uint8_t*)"***Init LARA : OK***\n\n",23,10);
		else
			HAL_UART_Transmit(&huart2,(uint8_t*)"***Init LARA : RETRY***\n",25,10);
	
	}while(statusAT == FAILED && nb_init < timeout);
}

StatusAT initConnectionHTTP(UART_HandleTypeDef *huart){
	int nbCommand = 7;
	AT_command initsCommands[nbCommand];
	int timeout_HTTP = 2;
	int nb_init = 0;
	
	/* Config réseau */
	
	// Automatic network registration
	initsCommands[0] = init_AT_command(2, "AT+COPS=0,0\r", 50, AT_C_COPS, 250);
	
	// On active le contexte PDP --> connection à l'internet
	initsCommands[1] = init_AT_command(3, "AT+UPSDA=0,3\r", 50, AT_OE_RI, 1000);
	
	/* Gestion de l'HTTP */
	
	// Reset de l'environnement HTTP
	initsCommands[2] = init_AT_command(2, "AT+UHTTP=0\r", 100, AT_OE, 250);
	
	// Renseignement du nom du serveur
	initsCommands[3] = init_AT_command(2, "AT+UHTTP=0,1,\"ptsv2.com\"\r", 50, AT_OE, 250);
	
	// Renseignement du port de communication HTTP
	initsCommands[4] = init_AT_command(2, "AT+UHTTP=0,5,80\r", 50, AT_OE, 250);
	
	// Résolution DNS à partir du nom du serveur
	initsCommands[5] = init_AT_command(3, "AT+UDNSRN=0,\"ptsv2.com\"\r", 80, AT_RI_OE, 2500);
	
	initsCommands[6] = init_AT_command(3, "AT+UHTTPC=0,1,\"/t/2y3ax-1548855809/post\",\"filename\"\r", 150, AT_C_UHTTPC, 4000);
	
	//initsCommands[7] = init_AT_command(5, "AT+UPING=\"www.google.com\"\r", 100, AT_C_PING);
	do{
		for(unsigned int num_commande = 0; num_commande < nbCommand; num_commande++){
			currentAT = initsCommands[num_commande];
			sendAT(huart, currentAT);
			HAL_Delay(100);
		}	
		if (statusAT == OK)
			HAL_UART_Transmit(&huart2,(uint8_t*)"***Init HTTP : OK***\n\n",23,10);
		else{
			HAL_UART_Transmit(&huart2,(uint8_t*)"***Init HTTP : RETRY***\n",23,10);
			uartEndLine(&huart2);
		}
			
		nb_init++;
	}while(statusAT == FAILED && nb_init < timeout_HTTP);
	
	if (nb_init >= timeout_HTTP){
		HAL_UART_Transmit(&huart2,(uint8_t*)"***Init HTTP : TIMEOUT***\n",27,10);
		return FAILED;
	}
	else
		return OK;
		
}

AT_command init_AT_command(int nombre_reponses, char * command, int taille_max_reponses, TypeATCommand type, uint32_t temps_reponse){
	AT_command mon_AT;
	mon_AT.command = command;
	mon_AT.nombre_reponses=nombre_reponses;
	mon_AT.taille_max_reponses=taille_max_reponses;
	mon_AT.type = type;
	mon_AT.temps_reponse = temps_reponse;

	return mon_AT;
}

/*void RX_interruption(){
	if(flag){
			switch(etat){
				case 
			}
	}
}*/
