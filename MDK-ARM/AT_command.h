#include <stm32f4xx_hal.h>
#include <string.h>


typedef enum {type1, type2, type3}Type;

typedef struct {
	int nombre_reponses;
	char * command;
	int taille_max_reponses;
	Type type;
	unsigned int temps_reponse; // en ms
}AT_command;

int sizeTabChar(char * s);
void sendAT(UART_HandleTypeDef* huart, char command[], int nbRep, int taille_max);
void uartEndLine(UART_HandleTypeDef *huart);
void initLARA(UART_HandleTypeDef *huart);
AT_command init_AT_command(int nombre_reponses, char * command, int taille_max_reponses);
void initConnectionHTTP(UART_HandleTypeDef *huart);
