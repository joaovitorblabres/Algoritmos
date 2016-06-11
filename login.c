#include <stdlib.h>
#include <stdio.h>

int form_login();

int form_login(){
	char login[100];
	char senha[100];

	printf("Informe seu login: ");
	fgets(login,100,stdin);		
	login[strlen(login)-1] = '\0';
	printf("Informe sua senha: ");
	fgets(senha,100,stdin);		
	senha[strlen(senha)-1] = '\0';
	
	

}
