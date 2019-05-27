#ifndef GPS_H
#define GPS_H

#include "stm32f4xx_hal.h"
#include "fonctions_char.h"

#define GPS_RST GPIO_PIN_4
#define GPS_WAKE_UP GPIO_PIN_1
#define GPS_TEST_TIMEOUT 100

void initGPS(UART_HandleTypeDef *huart);
void parse_trame(UART_HandleTypeDef *huart, char* trameGlobale, char* latitude, char* longitude, uint8_t *gpsFail, uint8_t *premierPost);

#endif
