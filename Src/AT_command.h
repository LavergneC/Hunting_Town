#include <stm32f4xx_hal_uart.h>

#define RX_BUFFER_SIZE 50

int sizeTabChar(char * s);
void sendAT(UART_HandleTypeDef* huart, char command[], int nbRep);
void uartEndLine(UART_HandleTypeDef *huart);