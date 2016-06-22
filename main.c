#include <stdio.h>
#include <stdlib.h>
#include "db_connect.c"
#include "interface.c"
#include "login.c"

int main(){
	// Chama o form inicial 
	system("clear");
	form_login();
}

/*
TABLE PARA TESTES
CREATE TABLE pessoa
(
  "Primeiro_Nome" character varying(100),
  idpessoa serial NOT NULL,
  login character varying(100),
  senha character varying(100),
  "Ultimo_Nome" character varying(100),
  "CPF" character varying(20),
  fl_lvl integer,
  CONSTRAINT idpessoa PRIMARY KEY (idpessoa)
)*/
