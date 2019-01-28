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

void split_trame(char* trame, unsigned int size, char buffer[3][40]){
	uint8_t cptBuffer=0, cpt = 0;
	for(int i=0 ; i<size ; i++){
		if(trame[i] == '\r' || trame[i] == '\n'){
			if(cpt != 0){
				buffer[cptBuffer][cpt+1] = '\0';
				cptBuffer+=1;
				cpt = 0;
			}
		}
		else{
			buffer[cptBuffer][cpt] = trame[i];
			cpt+=1;
		}
	}
}
