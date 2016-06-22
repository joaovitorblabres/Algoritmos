#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_T 50

void root();
void root_menu();
void user();
void user_menu();
void risco();
void continua();
void acesso();
void campos_edicao();
void nome();

// Imprimir desenho
void acesso(){
	printf("\t\t  _   _   _   _   _   _   \n"); 
	printf("\t\t / \\ / \\ / \\ / \\ / \\ / \\  \n");
	printf("\t\t( N | u | B | a | n | k ) \n");
	printf("\t\t \\_/ \\_/ \\_/ \\_/ \\_/ \\_/  \n");
  	printf("\n");
}

char nome_usr[500];
void nome(){
	char select[500];
	// Criar conexão com banco de dados
	PGconn *conn = PQconnectdb(STR_CON); 
	PGresult *res;
	sprintf(select,"SELECT primeiro_nome from pessoa where idPessoa = %s", id_usr);
	res = PQexec(conn,select);
	sprintf(nome_usr,"%s",PQgetvalue(res, 0, 0));
}
void risco(){
	int i;
	printf("+");
	for(i=0;i<TAM_T;i++)
		printf("-");
	printf("+\n");
}


// Tela para o usuario ROOT
void root_menu(){
	printf("+\tROOT - SISTEMA ANDERSON E JOÃO\n");
	risco();
	printf("+\tEscolha a opção:\n+\t1 - INSERIR NOVO USUÁRIO\n+\t2 - USUÁRIOS\n+\t3 - ATUALIZAR USUÁRIOS\n+\t4 - DELETAR USUÁRIOS\n+\t0 - SAIR\n");
	printf("+ ");
}

void campos_edicao(){
	printf("+\n+\t\t\tEDIÇÃO\n");
	risco();
	printf("+ Qual campo você deseja editar?\n+ 1 = Primeiro Nome | 2 = Ultimo Nome | 3 = Senha | 4 = Limite | 5 = Nível de Acesso | 6 = Adicionar conta | 0 = SAIR\n");
	printf("+ ");
}

// Escolha para o usario ROOT
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
				comandos_root(escolha);
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

// Escolha para USUARIO COMUM
void user(){
	user_menu();
	int escolha;
	while(scanf("%d",&escolha),escolha){
		getchar();
		switch(escolha){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				system("clear");
				comandos_user(escolha);
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

// Tela para USUARIO COMUM
void user_menu(){
	nome();
	printf("+\t\tBEM-VINDO \"%s\"\n",nome_usr);
	printf("+\t\tSISTEMA ANDERSON E JOÃO\n");
	risco();
	printf("+\tEscolha a opção:\n+\t1 - REALIZAR PAGAMENTO\n+\t2 - INFORMAÇÕES\n+\t3 - ATUALIZAR DADOS\n+\t4 - EFETUAR SAQUE\n+\t5 - EFETUAR DEPOSITO\n+\t0 - SAIR\n");
	printf("+ ");
}

void continua(){
	printf("Aperte [ENTER] para continuar ");
	getchar();
}
