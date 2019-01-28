#include <stm32f4xx_hal.h>
#include <string.h>

#define RX_BUFFER_SIZE 70

/*
Type :
	RI -> Réponse à ignorer qui sera juste affichée
	OE -> Réponse doit affiher OK, à tester pour mettre à jour le statusAT
	C  -> Commande particulière suivuit du nom de la commande
*/
typedef enum {AT_RI_OE, AT_OE, AT_OE_RI, AT_C_CPIN, AT_C_UHTTPC, AT_C_PING}TypeATCommand;

typedef enum {ECHO, REPONSE, OKouERR}Etat;
typedef enum {EN_COURS, OK, FAILED}StatusAT;

typedef struct {
	int nombre_reponses;
	char * command;
	int taille_max_reponses;
	TypeATCommand type;
	unsigned int temps_reponse; // en ms
}AT_command;

int sizeTabChar(char * s);
void sendAT(UART_HandleTypeDef* huart, AT_command at_command);
void uartEndLine(UART_HandleTypeDef *huart);
void initLARA(UART_HandleTypeDef *huart);
AT_command init_AT_command(int nombre_reponses, char * command, int taille_max_reponses, TypeATCommand type);
void initConnectionHTTP(UART_HandleTypeDef *huart);
