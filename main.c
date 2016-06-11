#include <stdio.h>
#include <stdlib.h>
#include "db_connect.c"
#include "interface.c"
#include "login.c"

int main(){
	system("clear");
	if(form_login()){
		int escolha;
		opcao();
		while(scanf("%d",&escolha),escolha){
			getchar();
			switch(escolha){
				case 1:
				case 2:
				case 3:
				case 4:
					system("clear");
					comandos(escolha);
					continua();
					system("clear");
					opcao();
					break;
				default:
					system("clear");
					printf("+ Opção inválida\n");
					opcao();
					break;
			}
			escolha=0;
		}
	}
}
/*
CREATE TABLE pessoa(
  "Nome" character varying(500),
  idpessoa serial NOT NULL,
  CONSTRAINT idpessoa PRIMARY KEY (idpessoa)
);*/
