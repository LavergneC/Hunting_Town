#ifndef FONCTIONS_CHAR_H
#define FONCTIONS_CHAR_H

#include <stm32f4xx_hal.h>

uint8_t tabsEquals(char* buffer, char* exp_anwser);
void split_trame(char* trame, unsigned int size, char reponses[3][40]);
char * myStrcat(char* commande, char* suiteCommande);
void uartEndLine(UART_HandleTypeDef *huart);

#endif
