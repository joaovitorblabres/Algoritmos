#include <stdlib.h>
#include <stdio.h>
 #include <unistd.h>

int form_login();

int form_login(){
<<<<<<< HEAD
	char usuario[100];
=======
	char login[100];
>>>>>>> 01ddb6820b24f7de0c6eee691ee604181094397b
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
