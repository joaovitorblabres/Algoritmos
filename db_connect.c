#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>
#define STR_CON "user=joao password=jvbl0809 dbname=testdb"

struct pessoa{
	char primeiro_nome[100];
	char ultimo_nome[100];
	char login[100];
	char senha[100];
	char CPF[100];
};

void do_exit(PGconn *conn, PGresult *res);
int comandos(int esc);
void select(PGconn *conn);
int apagar(PGconn *conn, int receb);
void editar(PGconn *conn, int receb);
void tratamento(char *ent, int tam);
int login(char *login, char *senha);

// declaração de funções para inserção
int insert(PGconn *conn, struct pessoa *dadosCliente, int *i);
void form_include_usuario();
void separa_nome(char *nome, int tamanho, struct pessoa *dadosCliente, int *l);
int valida_cpf(char *cpf, int tamanho);

//tratamento de string para evitar sql injection
void tratamento(char *ent, int tam){ 
	int i;
	for(i=0;i<tam;i++){
		if(ent[i] == 39){ //verifica se contém aspas simples e substitui por espaço
			ent[i] = 32;
		}
	}
	for(i=0;i<tam;i++){
		if(ent[i] == 59){ //verifica se contém ponto e virgula e substitui por espaço
			ent[i] = 32;
		}
	}
}

void do_exit(PGconn *conn, PGresult *res) { //retorna erro de conexão
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    

    PQclear(res);
    PQfinish(conn);
    
    exit(1);
}

int comandos(int esc){ //comandos para conexão com banco de dados
	PGconn *conn = PQconnectdb(STR_CON); //cria conexão com banco de dados
	int del_id, up_id;
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
		PQfinish(conn);
		return (0);
	}else{
		switch(esc){
			case 1:
				form_include_usuario();
				break;
			case 2:
				select(conn);
				break;
			case 3:
				select(conn);
				printf("\nInforme o ID que você quer editar: ");
				scanf("%d",&up_id);
				editar(conn, up_id);
				break;
			case 4:
				select(conn);
				printf("\nInforme o ID que você quer apagar: ");
				scanf("%d",&del_id);
				apagar(conn, del_id);
				getchar();
				break;
			default:
				printf("Opção inválida");
				return (0);
				break;
		}
	}
}

void select(PGconn *conn){
	PGresult *res = PQexec(conn, "SELECT * FROM pessoa ORDER BY idpessoa");    
	int rows = PQntuples(res);
	printf("+%*d registros encontrados:\n",3,rows);
	if(rows){
		for(int i=0; i<rows; i++) {
			printf("+%*s | %s | %s | %s | %s | %s\n",3, PQgetvalue(res, i, 1), PQgetvalue(res, i, 0), PQgetvalue(res, i, 4), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3), PQgetvalue(res, i, 5));
		}    
	}

}

int apagar(PGconn *conn, int receb){
	if(PQstatus(conn) != CONNECTION_BAD){
		char str_del[500];
		sprintf(str_del,"DELETE FROM pessoa WHERE idpessoa = %d", receb);
		PGresult *res = PQexec(conn, str_del);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Exclusão do id: %d ====== OK\n",receb);
		PQclear(res);
		return (1);
	}
}

void editar(PGconn *conn, int receb){ //inserção no banco de dados
	if(PQstatus(conn) != CONNECTION_BAD){
		char str_up[500];
		char altera[500];
		getchar();
		printf("Escrava o novo nome: ");
		fgets(altera,500,stdin);
		altera[strlen(altera)-1] = '\0';
		tratamento(altera,strlen(altera)); //trata strings
		sprintf(str_up,"UPDATE pessoa SET \"Primeiro_Nome\" = '%s' WHERE idpessoa = %d", altera, receb);
		PGresult *res = PQexec(conn, str_up);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Alteração do id: %d ====== OK\n",receb);
		PQclear(res);
	}
}

int login(char *login, char *senha){ // verificação de login
	PGconn *conn = PQconnectdb(STR_CON);
	char str_login[500];
	sprintf(str_login,"SELECT * from pessoa where \"login\" = '%s' AND \"senha\" = '%s'", login, senha);
	PGresult *res = PQexec(conn,str_login);
	int rows = PQntuples(res);
	if(rows){
		return (1);
	}else
		return (0);
}

//----------------------------------------------------------------------------inserção de usuários
void form_include_usuario(){ // form para incluir usuários
	PGconn *conn = PQconnectdb(STR_CON); //cria conexão com banco de dados
	char nome[500];
	int i=0, ok = 0;
	struct pessoa pessoas[1000];
	printf("\n");
	printf("+Informe o nome: ");
	fgets(nome,500,stdin);
	nome[strlen(nome)-1] = '\0';
	separa_nome(nome,strlen(nome),pessoas,&i);

	printf("+Informe um login para o usuário \"%s\": ",pessoas[i].primeiro_nome);
	fgets(pessoas[i].login,100,stdin);		
	pessoas[i].login[strlen(pessoas[i].login)-1] = '\0';
	tratamento(pessoas[i].login,strlen(pessoas[i].login));

	printf("+Informe uma senha para o usuário \"%s\": ",pessoas[i].primeiro_nome);
	fgets(pessoas[i].senha,100,stdin);
	pessoas[i].senha[strlen(pessoas[i].senha)-1] = '\0';
	tratamento(pessoas[i].senha,strlen(pessoas[i].senha)); //trata strings

	do{
		printf("+Informe o CPF: ");
		fgets(pessoas[i].CPF,100,stdin);
		pessoas[i].CPF[strlen(pessoas[i].CPF)-1] = '\0';
		ok = valida_cpf(pessoas[i].CPF,strlen(pessoas[i].CPF));
		if(!ok)
			printf("+CPF inválido!\n");
	}while(!ok);

	insert(conn,pessoas,&i); // chama função para inserir no banco
	i++;
}

int insert(PGconn *conn, struct pessoa *dadosCliente, int *i){ //inserção no banco de dados
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		char insert[500];
		sprintf(insert,"INSERT INTO pessoa (\"Primeiro_Nome\",\"Ultimo_Nome\",\"login\",\"senha\",\"CPF\") VALUES('%s','%s','%s','%s','%s')", (*(dadosCliente+*i)).primeiro_nome, (*(dadosCliente+*i)).ultimo_nome, (*(dadosCliente+*i)).login, (*(dadosCliente+*i)).senha,(*(dadosCliente+*i)).CPF);
		res = PQexec(conn, insert);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Inserção ====== OK\n");
		PQclear(res);
		return (1);
	}
}

void separa_nome(char *nome, int tamanho, struct pessoa *dadosCliente, int *l){
	tratamento(nome,tamanho);
	int i, k=0, pos_primeiro, pos_ultimo, cont=0;
	for(i=0;i<tamanho;i++){
		if(nome[i] == 32){
			cont++;
			if(cont==1)
				pos_primeiro = i;
			pos_ultimo = i;
		}
	}
	if(cont == 0){
		for(i=0;i<tamanho;i++){
			(*(dadosCliente+*l)).primeiro_nome[i] = nome[i];
		}
		printf("+Informe o último nome do usuário: ");
		fgets((*(dadosCliente+*l)).ultimo_nome,100,stdin);
		(*(dadosCliente+*l)).ultimo_nome[strlen((*(dadosCliente+*l)).ultimo_nome)-1] = '\0';
	}else{
		for(i=0;i<pos_primeiro;i++){
			(*(dadosCliente+*l)).primeiro_nome[i] = nome[i];
		}
		for(i=(pos_ultimo+1);i<tamanho;i++,k++){
			(*(dadosCliente+*l)).ultimo_nome[k] = nome[i];
		}
	}
}

int valida_cpf(char *cpf, int tamanho){
	int num[11],j,i,k=0, multiplica = 0, resto, verificador[2];
	for(i=0;i<tamanho;i++){
		switch(cpf[i]){
			case '0':
				num[k] = 0;
				k++;
				break;
			case '1':
				num[k] = 1;
				k++;
				break;
			case '2':
				num[k] = 2;
				k++;
				break;
			case '3':
				num[k] = 3;
				k++;
				break;
			case '4':
				num[k] = 4;
				k++;
				break;
			case '5':
				num[k] = 5;
				k++;
				break;
			case '6':
				num[k] = 6;
				k++;
				break;
			case '7':
				num[k] = 7;
				k++;
				break;
			case '8':
				num[k] = 8;
				k++;
				break;
			case '9':
				num[k] = 9;
				k++;
				break;
			default:
				break;
		}
	}
	for(k=0;k<2;k++){
		j=0;
		multiplica=0;
		for(i=(10+k);i>1;i--,j++){
			multiplica += num[j]*i;
		}
		resto = multiplica % 11;
		if(resto>=2)
			verificador[k] = 11 - resto;
		else
			verificador[k] = 0;
	}
	if(verificador[0] == num[9] && verificador[1] == num[10])
		return (1);
	else
		return (0);
}
