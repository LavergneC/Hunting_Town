#include <stm32f4xx_hal.h>
#include <string.h>
#include <stdlib.h>

#include "..\MDK-ARM\fonctions_char.h"

#define RX_BUFFER_SIZE 300


uint8_t flag_call;

/*
Type :
	RI -> R�ponse � ignorer qui sera juste affich�e
	OE -> R�ponse doit affiher OK, � tester pour mettre � jour le statusAT
	C  -> Commande particuli�re suivuit du nom de la commande
*/

typedef enum {AT_C_UDWNFILE, AT_RI, AT_RI_OE, AT_OE, AT_OE_RI, AT_C_CPIN, AT_C_UHTTPC, RI_AT_C_UHTTPC, AT_C_PING, AT_C_COPS}TypeATCommand;
typedef enum {ECHO, REPONSE, OKouERR}Etat;
typedef enum {EN_COURS, OK, FAILED}StatusAT;

typedef struct {
	int nombre_reponses;
	char * command;
	TypeATCommand type;
	uint32_t temps_reponse; // en ms
}AT_command;

int sizeTabChar(char * s);
void sendAT(UART_HandleTypeDef* huart, AT_command at_command);
void uartEndLine(UART_HandleTypeDef *huart);
void initLARA(UART_HandleTypeDef *huart);
AT_command init_AT_command(int nombre_reponses, char * command, TypeATCommand type, uint32_t temps_reponse);
StatusAT initConnectionHTTP(UART_HandleTypeDef *huart);
void postGPS(UART_HandleTypeDef* huart);
void creationFichier(UART_HandleTypeDef* huart, int8_t* latitude, int8_t* longitude);
void appel_via_GSM(UART_HandleTypeDef *huart);
void hang_up_call(UART_HandleTypeDef *huart);
	