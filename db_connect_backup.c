#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>

#ifndef dbconnect_H
#define dbconnect_H
void do_exit();
int connect();
int insert();
#endif

void do_exit(PGconn *conn, PGresult *res) {
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    

    PQclear(res);
    PQfinish(conn);    
    
    exit(1);
}

int main(){
	char *nome;
	fgets(nome,500,stdin);
/*	char *user = {"joao"};//mudar dps
	char *password = {"jvbl0809"};//mudar dps
	char *dbname = {"testdb"}; //mudar dps
	char connect[100];
	sprintf(connect,"user=%s password=%s dbname=%s",user,password,dbname);*/

	PGconn *conn = PQconnectdb("user=joao password=jvbl0809 dbname=testdb");

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",
		PQerrorMessage(conn));
		PQfinish(conn);
		return (0);
	} else {
		printf("OK\n");
		insert(conn, nome);
		return (1);
	}
}

int insert(PGconn *conn, char *nome){
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		PQclear(res);
		char *insert;
		sprintf(insert,"INSERT INTO pessoa (\"Nome\") VALUES('%s')", nome);
		res = PQexec(conn, insert);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		PQclear(res);
	}
}
