#include <stm32f4xx_hal.h>
#include <string.h>

#define RX_BUFFER_SIZE 70

typedef enum {type1, type2, type3}Type;
typedef enum {ECHO, REPONSE, OKouERR}Etat;
typedef enum {EN_COURS, OK, FAILED}StatusAT;

typedef struct {
	int nombre_reponses;
	char * command;
	int taille_max_reponses;
	Type type;
	unsigned int temps_reponse; // en ms
}AT_command;

int sizeTabChar(char * s);
void sendAT(UART_HandleTypeDef* huart, AT_command at_command);
void uartEndLine(UART_HandleTypeDef *huart);
void initLARA(UART_HandleTypeDef *huart);
AT_command init_AT_command(int nombre_reponses, char * command, int taille_max_reponses);
void initConnectionHTTP(UART_HandleTypeDef *huart);
