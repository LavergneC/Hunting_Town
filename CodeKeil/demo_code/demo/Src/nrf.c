/*
 * nRF8001 handling code
 * Part of the Bluetooth LE example system
 *
 * Copyright 2017 Sven Gregori
 * Released under MIT License
 *
 */
#include "nrf.h"

/* BLE connection state */
uint8_t nrf_connect_state = NRF_STATE_DISCONNECT;
/* Global rx data structure */
struct nrf_rx rx;

static uint8_t opmode;
static struct nrf_tx tx;
static const struct nrf_setup_data setup_data[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;

/**
 * Setup nRF8001 module.
 *
 * Send all setup data generated from nRFgo Studio in nrf/services.h to the
 * module via SPI and take care that everything is set up properly.
 *
 * If anything goes wrong during the setup phase, the setup process is
 * aborted and the module will not be functional. A negative return value
 * will indicate an error.
 *
 * @param none
 * @return 0 on success, a negative value in case of an error.
 */
int8_t nrf_setup(void)
{
    uint8_t cnt;
    HAL_UART_Transmit(&huart2,(uint8_t *) "Setup Bluetooth ",17, 10); 
    ble_reset_high();
    /* 
     * data sheet says RDYN signal is not valid until 62ms after nRF reset
     * pin goes high. Let's be on the safe side and wait 100ms.
     */
    HAL_Delay(100);

    memset(&rx, 0, sizeof(rx));
    nrf_receive(&rx);
		HAL_UART_Transmit(&huart2,(uint8_t *) ".",2, 10); 
		
	  /*test for DeviceStartedEvent */
    if (rx.data[0] != NRF_EVT_DEVICE_STARTED || rx.data[2] != NRF_ERR_NO_ERROR) {
        return -1;
    }
    opmode = rx.data[1];

    if (opmode != NRF_OPMODE_SETUP) {
        return -2;
    }

    /* Send all setup data to nRF8001 */
    for (cnt = 0; cnt < NB_SETUP_MESSAGES - 1; cnt++) {
				HAL_Delay(1000);
			
        memset(&rx, 0, sizeof(rx));
        memcpy(&tx, &setup_data[cnt].data, sizeof(struct nrf_tx));

				nrf_txrx(&tx,&rx);
        HAL_UART_Transmit(&huart2,(uint8_t *) ".",2, 10); 
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			
				if (rx.length == 0) 
									continue;
				
        /* Make sure transaction continue command response event is received or TRANSACTION_CONTINUE*/
        if (rx.data[0] != NRF_EVT_CMD_RESPONSE ||
            rx.data[1] != NRF_CMD_SETUP ||
            rx.data[2] != ACI_STATUS_TRANSACTION_CONTINUE)
        {
							return -3;
				}
    }
		/* Send last setup command */
		memset(&rx, 0, sizeof(rx));
    memcpy(&tx, &setup_data[NB_SETUP_MESSAGES -1].data, sizeof(struct nrf_tx));
		nrf_txrx(&tx,&rx);
		HAL_UART_Transmit(&huart2,(uint8_t *) ".",2, 10); 
		/* Make sure transaction complete command response event is received */
		if (rx.data[0] != NRF_EVT_CMD_RESPONSE ||
        rx.data[1] != NRF_CMD_SETUP ||
        rx.data[2] != ACI_STATUS_TRANSACTION_COMPLETE)
		{
				return -4;
    }

    /* One last receive loop to wait for DeviceStartedEvent */
    do {
        memset(&rx, 0, sizeof(rx));
        nrf_receive(&rx);
    } while (rx.data[0] != NRF_EVT_DEVICE_STARTED);
		

		if ( rx.data[0] != NRF_EVT_DEVICE_STARTED ||
         rx.data[1] != NRF_OPMODE_STANDBY ||
         rx.data[2] !=  NRF_ERR_NO_ERROR) 
		{
        return -5;
    }
    opmode = rx.data[0];
    
    return 0;
}


/**
 * Start advertising, waiting for remote side to connect.
 *
 * @param none
 * @return none
 */
void nrf_advertise(void)
{
    data16_t timeout;
    data16_t advival;

    memset(&tx, 0, sizeof(tx));

    timeout.word = 60;
    advival.word = 128;

    tx.length = 5;
    tx.command = NRF_CMD_CONNECT;
	
    /* send LSB first */
    tx.data[0] = timeout.lsb;
    tx.data[1] = timeout.msb;
    tx.data[2] = advival.lsb;
    tx.data[3] = advival.msb;
		HAL_GPIO_WritePin(GPIOD, NRF_PIN_RST, GPIO_PIN_SET);
    nrf_send(&tx);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOD, NRF_PIN_RST, GPIO_PIN_RESET);
}

/**
 * nRF8001 transmission function.
 * Send and simultaniously receive data to and from the nRF8001 module.
 * Note, received data is always related to a previous transmission or
 * an otherwise asynchronous event, but will never be the direct result of
 * the call in progress. If data is expected to be sent from the nRF8001
 * module afterwards, a subsequent transmit call has to be made in order to
 * receive the data. But keep in mind that asynchronous data might be sent,
 * so ignoring the received data during a send-only operation is not
 * recommended.
 *
 * In cases where received data really can be safely ignored, or no data
 * has to be trasmitted in the first place, but only received, NULL values
 * can be given for the tx and rx parameter respectively. In addition, two
 * macros exist as a shortcut for these cases:
 *      nrf_send(struct nrf_tx *)       for send-only
 *      nrf_receive(struct nrf_rx *)    for receive-only
 *
 * In addition, a third macro, nrf_txrx(struct nrf_tx *, struct nrf_rx *)
 * exists to have a sequential send and receive operation, i.e calling
 * a send-only nrf_transmit() followed directly by a receive-only one.
 *
 * @param tx nrf_tx structure for sending, make sure the data buffer is
 *           filled with zeroes. Can be NULL
 * @param rx nrf_rx structure for receiving, can be NULL
 * @return 0
 *
 */
int8_t nrf_transmit(struct nrf_tx *tx, struct nrf_rx *rx)
{
    uint8_t i;
		uint8_t dummyData;

    reqn_set_low();
    while (rdyn_is_high()) {
        /* wait */
    }
    /*
     * Check if given tx struct is NULL and only rx is of interest.
    */
    if (tx == NULL) {
				HAL_SPI_Receive(&hspi2, &dummyData, 1, 1500); // Because we receive a random 0x01
			  HAL_SPI_Receive(&hspi2, &rx->length, 1, 1500);
			  HAL_SPI_Receive(&hspi2, &rx->data[0], rx->length , 1500);
    }
    else if (rx == NULL) {
				HAL_SPI_Transmit(&hspi2,&(tx->length),1,10);
			  HAL_SPI_Transmit(&hspi2,&(tx->command),1,10);
				for (i = 0; i < tx->length - 1 || i < rx->length; i++){
						HAL_SPI_Transmit(&hspi2,&tx->data[i],1,10);
				}
    }   
    reqn_set_high();
    while (rdyn_is_low()) {
        /* wait */
    }
    /*
     * Make sure REQN inactive time (Tcwh, nRF datasheet page 26) is given.
     * Experienced some timing issues, i.e. empty events read after requesting
     * data (simplest case reading temperature) without a small delay here.
     */
    HAL_Delay(1);

    return 0;
}

/**
 * Parse received data from nRF8001 module.
 *
 * @param rx Data received from nRF8001 module
 * @return none
 */
void nrf_parse(struct nrf_rx *rx)
{	
	if (rx->length == 0 || rx->data[1] == NRF_RX_DEFAULT_VALUE) {
			return;
	}

	switch (rx->data[0]) {
			case NRF_EVT_CMD_RESPONSE:
				HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_EVT_CMD_RESPONSE : ", 23, 10);
					if (rx->data[1] == NRF_CMD_CONNECT)
					{
							HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_CMD_CONNECT, ", 18, 10);
					}
					if ( rx->data[2] == NRF_ERR_NO_ERROR )
						HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_NO_ERROR\n\r", 15, 10);
					else
						HAL_UART_Transmit(&huart2,(uint8_t *) "!NRF_ERR_ERROR!\n\r", 18, 10);
					break;

			case NRF_EVT_CONNECTED:
					nrf_connect_state = NRF_STATE_CONNECTED;
					HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_EVT_CONNECTED\n\r", 20, 10);
					break;

			case NRF_EVT_DISCONNECTED:
					nrf_connect_state = NRF_STATE_DISCONNECT;
					HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_EVT_DISCONNECTED\n\r", 23, 10);
					break;

			case NRF_EVT_PIPE_STATUS:
					HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_EVT_PIPE_STATUS\n\r", 22, 10);
					break;

			case NRF_EVT_DATA_RECEIVED:
					HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_EVT_DATA_RECEIVED\n\r", 24, 10);
					break;
			
			case NRF_EVT_PIPE_ERROR_EVENT :
				HAL_UART_Transmit(&huart2,(uint8_t *) "NRF_EVT_PIPE_ERROR_EVENT", 25, 10);
				if(rx->data[2] == NRF_ERR_ERROR_PIPE_TYPE_INVALID)
					HAL_UART_Transmit(&huart2,(uint8_t *) " : NRF_ERR_ERROR_PIPE_TYPE_INVALID", 36, 10);
				HAL_UART_Transmit(&huart2,(uint8_t *)"\n\r",3,10);
			break;
				
			case NRF_EVT_DATA_CREDIT_EVENT :
				HAL_UART_Transmit(&huart2, (uint8_t *) "NRF_EVT_DATA_CREDIT_EVENT\n\r", 28, 10);
			break;
			
			default:
			HAL_UART_Transmit(&huart2,(uint8_t *) "default\n\r", 10, 10);
		
	}
}

void nrf_manage_tx(uint8_t char_to_send){
	memset(&tx, 0, sizeof(tx));

	tx.length = 3;
	tx.command = NRF_CMD_SEND_DATA;
	tx.data[0] = PIPE_MONSERVICE_PLAYVIDEO_TX;
	tx.data[1] = char_to_send;
	nrf_send(&tx);
	
	for(unsigned char i = 0; i < 2; i++){
		nrf_receive(&rx);
		nrf_parse(&rx);
		memset(&rx, 0xCC, sizeof(rx));
		HAL_Delay(40);
	}
}
/**
 * Print the given rx struct content.
 *
 * Mainly meant for debug
 *
 * @param rx Received data to print
 * @return none
 */
void nrf_print_rx(struct nrf_rx *rx)
{
	unsigned char i;
	
	for(i=0; i < rx->length; i++){
		HAL_UART_Transmit(&huart2,(uint8_t*) ".", 1, 10);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)"\n", 1, 10);
}

void nrf_init_bluetooth(void){
	volatile int setupStatus;
		/* precedure de reset */ 
	HAL_GPIO_WritePin(GPIOE, NRF_PIN_RST, GPIO_PIN_RESET);
  HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOE, NRF_PIN_RST, GPIO_PIN_SET); 
	HAL_Delay(100);

	setupStatus = nrf_setup();
	if (setupStatus < 0){
		HAL_UART_Transmit(&huart2,(uint8_t *) "ERROR\n\r",8, 10);
		Error_Handler();
	}
	HAL_UART_Transmit(&huart2,(uint8_t *) "OK\n\r",5, 10); 
	
	memset(&tx, 0, sizeof(tx));
	
	struct nrf_rx rx;
	memset(&rx, 0, sizeof(rx));
	
	nrf_advertise();
	for(unsigned char i = 0; i < 3; i++){
		nrf_receive(&rx);
		nrf_parse(&rx);
		memset(&rx, 0, sizeof(rx));
		HAL_Delay(40);
	}	
	
	nrf_manage_tx(NRF_DATA_DEFAULT);
	HAL_Delay(100);
	nrf_manage_tx(NRF_DATA_DEFAULT);
	HAL_Delay(1000);
	nrf_manage_tx(NRF_DATA_DEFAULT);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}
