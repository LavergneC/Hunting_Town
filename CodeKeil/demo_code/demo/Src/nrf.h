/*
 * nRF8001 handling code
 * Part of the Bluetooth LE example system
 *
 * Copyright 2017 Sven Gregori
 * Released under MIT License
 *
 */
 

#ifndef _NRF_H_
#define _NRF_H_

#include <stdint.h>
#include <string.h>
#include "main.h"
#include "services.h"

#define NRF_OPMODE_TEST     0x01
#define NRF_OPMODE_SETUP    0x02
#define NRF_OPMODE_STANDBY  0x03

#define NRF_STATE_DISCONNECT 0x00
#define NRF_STATE_CONNECTING 0x01
#define NRF_STATE_CONNECTED  0x02

#define NRF_CMD_ECHO						0x02
#define NRF_CMD_SETUP           0x06
#define NRF_CMD_TEST						0x01
#define NRF_CMD_GET_TEMPERATURE 0x0c
#define NRF_CMD_CONNECT         0x0f
#define NRF_CMD_SEND_DATA       0x15
#define NRF_CMD_GETDEVICEADDRESS 0x0A

#define NRF_ERR_NO_ERROR        0x00
#define NRF_ERR_ERROR_PIPE_TYPE_INVALID 0x95
#define NRF_EVT_DEVICE_STARTED  0x81
#define NRF_EVT_CMD_RESPONSE    0x84
#define NRF_EVT_CONNECTED       0x85
#define NRF_EVT_DISCONNECTED    0x86
#define NRF_EVT_PIPE_STATUS     0x88
#define NRF_EVT_DATA_RECEIVED   0x8c
#define NRF_EVT_PIPE_ERROR_EVENT 0x8D
#define NRF_EVT_DATA_CREDIT_EVENT 0x8A

#define NRF_PIN_RDYN GPIO_PIN_7
#define NRF_PIN_REQN_CS GPIO_PIN_12
#define NRF_PIN_RST GPIO_PIN_10

#define NRF_RX_DEFAULT_VALUE 		0xCC
#define NRF_DATA_PING						0xFE
#define NRF_DATA_DEFAULT				0x99

#pragma anon_unions

typedef union {
    uint8_t byte[2]; /* byte[0] = lsb, byte[1] = msb */

    uint16_t word;
    int16_t s_word;
    struct {
        uint8_t lsb;
        uint8_t msb;
    };
} data16_t;

struct nrf_setup_data {
    uint8_t status;
    uint8_t data[32];
};

struct nrf_tx {
    uint8_t length;
    uint8_t command;
    uint8_t data[34];
};

struct nrf_rx {
	  int8_t debug;
    uint8_t length;
    uint8_t data[30];
};


int8_t nrf_reset_module(void);
int8_t nrf_setup(void);
void nrf_advertise(void);
void nrf_manage_tx(uint8_t char_to_send);

int8_t nrf_transmit(struct nrf_tx *tx, struct nrf_rx *rx);
#define nrf_send(tx) nrf_transmit(tx, NULL)
#define nrf_receive(rx) nrf_transmit(NULL, rx)
#define nrf_txrx(tx, rx) do { nrf_transmit(tx, NULL); nrf_transmit(NULL, rx); } while (0);

int8_t nrf_send_button_data(uint8_t button);
void nrf_parse(struct nrf_rx *rx);
void nrf_print_rx(struct nrf_rx *rx);
void nrf_print_temperature(void);

extern uint8_t nrf_connect_state;
extern struct nrf_rx rx;
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;

//#define led_setup_on()      do { PORTD |= 0x80; } while (0)
//#define led_setup_off()     do { PORTD &= ~(0x80); } while (0)
//#define led_connect_on()    do { PORTD |= 0x10; } while (0)
//#define led_connect_off()   do { PORTD &= ~(0x10); } while (0)

#define ble_reset_high()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
#define ble_reset_low()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET)
#define reqn_set_high()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET)
#define reqn_set_low()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET)
#define rdyn_is_high()      (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8) == GPIO_PIN_SET)
#define rdyn_is_low()       (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8) == GPIO_PIN_RESET)


//struct service_pipe_mapping {
//    aci_pipe_store_t store;
//    aci_pipe_type_t type;
//};

void nrf_tx_map_pipes(void);

#endif /* _NRF_H_ */
