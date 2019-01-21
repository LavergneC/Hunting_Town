#include <stm32f4xx_hal_uart.h>

int sizeTabChar(char * s);
void sendAT(UART_HandleTypeDef* huart, char command[], int nbRep, int taille_max);
void uartEndLine(UART_HandleTypeDef *huart);
void initLARA(UART_HandleTypeDef *huart);

typedef struct {
	int nombre_reponses;
	char * command;
	int taille_max_reponses;
}AT_command;