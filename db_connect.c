#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>

void do_exit(PGconn *conn, PGresult *res);
int comandos(int esc);
int insert(PGconn *conn, char *nome);
void select(PGconn *conn);
int apagar(PGconn *conn, int receb);
void editar(PGconn *conn, int receb);
void tratamento(char *ent, int tam);

void tratamento(char *ent, int tam){ //tratamento de string para evitar sql injection
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

	char user[50] = {"joao"};
	char password[50] = {"jvbl0809"};
	char dbname[50] = {"testdb"}; 
	char db[100];
	sprintf(db,"user=%s password=%s dbname=%s",user,password,dbname); //concatena string para conexão com banco de dados
	PGconn *conn = PQconnectdb(db); //cria conexão com banco de dados
	char nome[500];
	int del_id, up_id;
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
		PQfinish(conn);
		return (0);
	}else{
		switch(esc){
			case 1:
				printf("Informe o nome: ");
				fgets(nome,500,stdin);		
				nome[strlen(nome)-1] = '\0';
				tratamento(nome,strlen(nome)); //trata strings
				insert(conn,nome); // chama função para inserir no banco
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

int insert(PGconn *conn, char *nome){ //inserção no banco de dados
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		PQclear(res); //limpa lixo de memória na string res
		char insert[500];
		sprintf(insert,"INSERT INTO pessoa (\"Nome\") VALUES('%s')", nome);
		res = PQexec(conn, insert);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Inserção ====== OK\n");
		PQclear(res);
		return (1);
	}
}

void select(PGconn *conn){
	PGresult *res = PQexec(conn, "SELECT * FROM pessoa ORDER BY idpessoa");    

	int rows = PQntuples(res);
	printf("+%*d registros encontrados:\n",3,rows);
	if(rows){
		for(int i=0; i<rows; i++) {
			printf("+%*s | %s\n",3, PQgetvalue(res, i, 1), PQgetvalue(res, i, 0));
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
		sprintf(str_up,"UPDATE pessoa SET \"Nome\" = '%s' WHERE idpessoa = %d", altera, receb);
		PGresult *res = PQexec(conn, str_up);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Alteração do id: %d ====== OK\n",receb);
		PQclear(res);
	}
}
