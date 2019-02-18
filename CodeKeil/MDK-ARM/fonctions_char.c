#include "fonctions_char.h"

//extern AT_command currentAT;

uint8_t tabsEquals(char* buffer, char* exp_answer){
	int i=0;
	
	if(buffer[0] == 0x00)
		return 0;
	
	while(buffer[i] != '\0' && exp_answer[i] != '\0'){
		if(buffer[i] != exp_answer[i])
			return 0;
		else
			i+=1;
	}
	return 1;
}

char * myStrcat(char* commande, char* suiteCommande){
	char * cp_return = commande;
	char * cp_commande = commande;
	char * cp_suiteCommande = suiteCommande;
	
	while (*cp_commande != '\0')
		cp_commande++;
	while (*cp_suiteCommande != '\0'){
		*cp_commande = *cp_suiteCommande;
		cp_commande++;
		cp_suiteCommande++;
	}
	*cp_commande = '\0';
	
	return cp_return;
}
