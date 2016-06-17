#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.c"
#define TAM_T 50

void root();
void root_menu();
void user();
void user_menu();
void risco();
void continua();
void acesso();

void acesso(){
	printf("\t\t  _   _   _   _   _   _   \n"); 
	printf("\t\t / \\ / \\ / \\ / \\ / \\ / \\  \n");
	printf("\t\t( N | u | B | a | n | k ) \n");
	printf("\t\t \\_/ \\_/ \\_/ \\_/ \\_/ \\_/  \n");
  	printf("\n");                             
	form_login();
}
                                     

void risco(){
	int i;
	printf("+");
	for(i=0;i<TAM_T;i++)
		printf("-");
	printf("+\n");
}


//-----------------------------------------------------------------------ROOT
void root_menu(){
	printf("+\tROOT - SISTEMA ANDERSON E JOÃO\n");
	risco();
	printf("+\tEscolha a opção:\n+\t1 - INSERIR NOVO USUÁRIO\n+\t2 - USUÁRIOS\n+\t3 - ATUALIZAR USUÁRIOS\n+\t4 - DELETAR USUÁRIOS\n+\t0 - SAIR\n");
	printf("+ ");
}

void root(){
	root_menu();
	int escolha;
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
				root_menu();
				break;
			default:
				system("clear");
				printf("+ Opção inválida\n");
				root_menu();
				break;
		}
		escolha=0;
	}
}

//-----------------------------------------------------------------------USER COMUM
void user(){
	user_menu();
	int escolha;
	while(scanf("%d",&escolha),escolha){
		getchar();
		switch(escolha){
			case 1:
			case 2:
			case 3:
				system("clear");
				comandos(escolha);
				continua();
				system("clear");
				user_menu();
				break;
			default:
				system("clear");
				printf("+ Opção inválida\n");
				user_menu();
				break;
		}
		escolha=0;
	}
}

void user_menu(){
	printf("+\t\tSISTEMA ANDERSON E JOÃO\n");
	risco();
	printf("+\tEscolha a opção:\n+\t1 - REALIZAR PAGAMENTO\n+\t2 - INFORMAÇÕES\n+\t3 - ATUALIZAR DADOS\n+\t0 - SAIR\n");
	printf("+ ");
}

void continua(){
	printf("Aperte [ENTER] para continuar ");
	getchar();
}
