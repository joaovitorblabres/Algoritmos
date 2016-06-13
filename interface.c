#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_T 50

void opcao();
void risco();
void continua();

void risco(){
	int i;
	printf("+");
	for(i=0;i<TAM_T;i++)
		printf("-");
	printf("+\n");
}

void opcao(){
	printf("+\t\tROOT - SISTEMA ANDERSON E JOÃO\n");
	risco();
	printf("+\tEscolha a opção:\n+\t1 - INSERIR NOVO USUÁRIO\n+\t2 - USUÁRIOS\n+\t3 - ATUALIZAR USUÁRIOS\n+\t4 - DELETAR USUÁRIOS\n+\t0 - SAIR\n");
	printf("+ ");
}

void continua(){
	printf("Aperte [ENTER] para continuar ");
	getchar();
}
