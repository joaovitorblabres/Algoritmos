#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int form_login();
int checa(char lvl);

int checa(char lvl){
	if(lvl == 48)
		return (0);
	else
		return (1);
}

int form_login(){
	char usuario[100];
	char senha[100];
	int espera,i;
	do{	
		acesso();
		printf("Informe sua conta: ");
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
			if(!checa(nv_login(usuario,senha)))
				root();
			else
				user();
		}
	}while(!login(usuario,senha));
}
