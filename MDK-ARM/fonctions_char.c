#include "fonctions_char.h"


uint8_t tabsEquals(char* buffer, char* exp_answer){
	int i=0;
	while(buffer[i] != '\0' && exp_answer[i] != '\0'){
		if(buffer[i] != exp_answer[i])
			return 0;
		else
			i+=1;
	}
	return 1;
}

char** split_trame(char* trame, unsigned int size){
	uint8_t cptBuffer=0, cpt = 0;
	char** buffer;
	for(int i=0 ; i<size ; i++){
		if(trame[i] == '\r' || trame[i] == '\n'){
			if(cpt != 0){
				cptBuffer+=1;
			}
		}
		else{
			buffer[cptBuffer][cpt] = trame[i];
			cpt+=1;
		}
	}
	return buffer;
}
