#include <stdlib.h>
#include <stdio.h>
 #include <unistd.h>

int form_login();

int form_login(){
	char usuario[100];
	char senha[100];
	int espera,i;
	do{	
		printf("Informe seu login: ");
		fgets(usuario,100,stdin);
		usuario[strlen(usuario)-1] = '\0';
		printf("Informe sua senha: ");
		fgets(senha,100,stdin);
		senha[strlen(senha)-1] = '\0';
		printf("Conectando\n");
		sleep(1);
		if(!login(usuario,senha)){
			system("clear");
			printf("Usuário ou senha incorretos\n");
		}else{
			system("clear");
			return (1);
		}
	}while(!login(usuario,senha));
}
