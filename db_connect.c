#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <unistd.h>
#define STR_CON "user=postgres password=postgres dbname=NuBank"

// Definicao da struct pessoa
struct pessoa{
	char primeiro_nome[100];
	char ultimo_nome[100];
	char CPF[100];
};

// Definicao da struct conta
struct conta{
	int numero;
	char senha[100];
	double limite;
	double saldo;
	int nivel_acesso;
};

// Grava o ID do Usuario
char id_usr[10];
char id[10];
int idusr;
// Assinatura de funcoes
// Comandos comuns
void do_exit(PGconn *conn, PGresult *res);
int comandos_root(int esc);
int comandos_user(int esc);
void select_pessoa(PGconn *conn);
void select_user(PGconn *conn);
// Para acoes do usuario
void user_pagar(int valor, char *categoria, char *data);
void saque(int valor);
void deposito(int valor);
// Para Edicao
void editar(PGconn *conn, int receb);
void tratamento(char *ent, int tam);
int apagar(PGconn *conn, int receb);
int verifica_conta(int);
// Para Login
int login(char *login, char *senha);
char nv_login(char *login, char *senha);
// Para inserção
int insert_pessoa(PGconn *conn, struct pessoa *dadosCliente, int *i);
void form_include_usuario();
void separa_nome(char *nome, int tamanho, struct pessoa *dadosCliente, int *l);
int valida_cpf(char *cpf, int tamanho);
int verifica_cpf(char *);
void form_include_conta(int idpessoa);
int insert_conta(PGconn *conn, struct conta *contas, int *i);
void gera_numero(struct conta *contas, int *i);
int verifica_numero(int);
//tratamento de string para evitar sql injection
void tratamento(char *ent, int tam){ 
	int i;
	for(i=0;i<tam;i++){
		// Verifica se contém aspas simples e substitui por espaço
		if(ent[i] == 39){
			ent[i] = 32;
		}
	}
	for(i=0;i<tam;i++){
		// Verifica se contém ponto e virgula e substitui por espaço
		if(ent[i] == 59){
			ent[i] = 32;
		}
	}
}

// Funcao responsavel por retornar se houve erro de conexão com o banco de dados
void do_exit(PGconn *conn, PGresult *res) {
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    

    PQclear(res);
    PQfinish(conn);
    
    exit(1);
}

// Comandos para usuario do tipo Root
int comandos_root(int esc){
	int del_id, up_id, ins_conta;
	PGconn *conn = PQconnectdb(STR_CON);
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
		PQfinish(conn);
		return (0);
	}else{
		switch(esc){
			case 1:
				// Chama funcao responsavel por inclusao de conta
				form_include_usuario();
				printf("\n+Deseja incluir uma conta (0=NAO | 1=SIM)? ");
				scanf("%d",&ins_conta);
				getchar();
				if(ins_conta){
					form_include_conta(atoi(id));
				}
				break;
			case 2:
				// Chama funcao para mostrar informacoes do usuario
				select_pessoa(conn);
				break;
			case 3:
				// Chama funcao para editar usuarios
				select_pessoa(conn);
				printf("\nInforme o ID que você quer editar (0 = CANCELAR): ");
				scanf("%d",&up_id);
				getchar();
				system("clear");
				if(up_id)
					editar(conn, up_id);
				break;
			case 4:
				// Chama funcao para apgar usuarios
				select_pessoa(conn);
				printf("\nInforme o ID que você quer apagar (0 = CANCELAR): ");
				scanf("%d",&del_id);
				getchar();
				if(del_id)
					apagar(conn, del_id);
				break;
			default:
				printf("Opção inválida");
				return (0);
				break;
		}
	}
}

// Comandos possiveis do usuario
int comandos_user(int esc){
	int del_id, up_id, ins_conta, valor;
	char categoria[50];
	char data[20];
	PGconn *conn = PQconnectdb(STR_CON);
	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",PQerrorMessage(conn));
		PQfinish(conn);
		return (0);
	}else{
		switch(esc){
			case 1:
				// Chama funcao responsavel por efetur pagamente por limite
				system("clear");
				printf("+Informe o valor que deseja pagar: ");
				scanf("%d", &valor);
				getchar();
				printf("+Informe a categoria do lugar: ");
				fgets(categoria, 50, stdin);
				printf("+Informe a data da compra: ");
				fgets(data, 20, stdin);
				user_pagar(valor, categoria, data);
				break;
			case 2:
				// Chama funcao que mostra informacoes do usuario
				select_user(conn);
				break;
			case 3:
				// Chama a funcao para edicao
				select_pessoa(conn);
				printf("\nInforme o ID que você quer editar (0 = CANCELAR): ");
				scanf("%d",&up_id);
				getchar();
				system("clear");
				if(up_id)
					editar(conn, up_id);
				break;
			case 4:
				// Chama a funcao para saque
				printf("Digite o valor para sacar: ");
				scanf("%d", &valor);
				saque(valor);
				break;
			case 5:
				// Chama a funcao para deposito
				printf("Digite o valor para depositar: ");
				scanf("%d", &valor);
				deposito(valor);
				break;
			case 6:
				// Chama funcao para apagar pessao
				select_pessoa(conn);
				printf("\nInforme o ID que você quer apagar (0 = CANCELAR): ");
				scanf("%d",&del_id);
				getchar();
				if(del_id)
					apagar(conn, del_id);
				break;
			default:
				printf("Opção inválida");
				return (0);
				break;
		}
	}
}

// Funcao responsavel por selecionar informacoes para o usuario
void select_user(PGconn *conn){
	char query[500];
	sprintf(query,"SELECT * FROM pessoa full join conta on idpessoa = pessoa_idpessoa WHERE pessoa_idpessoa = %s",id_usr);
	PGresult *res = PQexec(conn, query);
	int rows = PQntuples(res);
	printf("+        Primeiro Nome |          Ultimo Nome |             CPF | Conta |  Senha |     Limite |      Saldo | Nivel\n");
	if(rows){
		for(int i=0; i<rows; i++) {
			printf("+ %20s | %20s | %15s | %5s | %6s | %10s | %10s | %2s\n", PQgetvalue(res, i, 1), PQgetvalue(res, i, 3), PQgetvalue(res, i, 2), PQgetvalue(res, i, 6), PQgetvalue(res, i, 7), PQgetvalue(res, i, 8), PQgetvalue(res, i, 9), PQgetvalue(res, i, 10));
		}    
	}
}

// Funcao responsavel por buscar informações para o root
void select_pessoa(PGconn *conn){ 
	PGresult *res = PQexec(conn, "SELECT * FROM pessoa full join conta on idpessoa = pessoa_idpessoa ORDER BY idpessoa");    
	int rows = PQntuples(res);
	printf("+%*d registros encontrados:\n",3,rows);
	printf("+ ID |        Primeiro Nome |          Ultimo Nome |             CPF | Conta |  Senha |     Limite |      Saldo | Nivel\n");
	if(rows){
		for(int i=0; i<rows; i++) {
			printf("+%3s | %20s | %20s | %15s | %5s | %6s | %10s | %10s | %2s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 3), PQgetvalue(res, i, 2), PQgetvalue(res, i, 6), PQgetvalue(res, i, 7), PQgetvalue(res, i, 8), PQgetvalue(res, i, 9), PQgetvalue(res, i, 10));
		}    
	}
}

// Funcao responsavel por apagar usuarios
int apagar(PGconn *conn, int receb){
	if(PQstatus(conn) != CONNECTION_BAD){
		char str_del[500];
		sprintf(str_del,"DELETE FROM conta WHERE pessoa_idpessoa = %d", receb);
		PGresult *res = PQexec(conn, str_del);
		PQclear(res);
		sprintf(str_del,"DELETE FROM pessoa WHERE idpessoa = %d", receb);
		res = PQexec(conn, str_del);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Exclusão do id: %d ====== OK\n",receb);
		PQclear(res);
		return (1);
	}
}

// Funcao responsavel por edicoes de usuario
void editar(PGconn *conn, int receb){
	if(PQstatus(conn) != CONNECTION_BAD){
		char str_up[500];
		char altera[500];
		int campo;
		while(select_pessoa(conn),campos_edicao(),scanf("%d",&campo),campo){
			getchar();
			switch(campo){
				// Edicao de nome
				case 1:
					printf("+ Escreva o novo Primeiro_Nome: \n+ ");
					fgets(altera,500,stdin);
					altera[strlen(altera)-1] = '\0';
					tratamento(altera,strlen(altera)); //trata strings
					sprintf(str_up,"UPDATE pessoa SET \"primeiro_nome\" = '%s' WHERE idpessoa = %d", altera, receb);
					break;
				// Edicao de sobrenome
				case 2:
					printf("+ Escreva o novo Ultimo_Nome: \n+ ");
					fgets(altera,500,stdin);
					altera[strlen(altera)-1] = '\0';
					tratamento(altera,strlen(altera));
					sprintf(str_up,"UPDATE pessoa SET \"ultimo_nome\" = '%s' WHERE idpessoa = %d", altera, receb);
					break;
				// Edicao de senha
				case 3:
					while(printf("+ Informe a nova senha: \n+ "),fgets(altera,500,stdin),strlen(altera)-1>6){
						printf("+ Quantidades de caracteres inválidos\n");
					}
					altera[strlen(altera)-1] = '\0';
					tratamento(altera,strlen(altera));
					sprintf(str_up,"UPDATE conta SET \"senha\" = '%s' WHERE pessoa_idpessoa = %d", altera, receb);
					break;
				// Edicao de limite
				case 4:
					printf("+ Informe o novo limite do usuário: \n+ ");
					fgets(altera,500,stdin);
					altera[strlen(altera)-1] = '\0';
					tratamento(altera,strlen(altera));
					sprintf(str_up,"UPDATE conta SET \"limite\" = '%s' WHERE pessoa_idpessoa = %d", altera, receb);
					break;
				// Edicao de nivel de acesso
				case 5:
					while(printf("+ Informe o novo nível de acesso: \n+ "),fgets(altera,500,stdin),altera[0]>50){
						printf("+ Nível de acesso inválido\n");
					}
						altera[strlen(altera)-1] = '\0';
						tratamento(altera,strlen(altera));
						sprintf(str_up,"UPDATE conta SET \"nivel_acesso\" = '%s' WHERE pessoa_idpessoa = %d", altera, receb);
					break;
				// Edicao se a conta ja esta cadastrada
				case 6:
					if(!verifica_conta(receb)){
						form_include_conta(receb);
					} else
						printf("\n+ Usuário já possui conta cadastrada");
					break;
			}
			// Checa se a edicao foi bem sucedida
			PGresult *res = PQexec(conn, str_up);
			if(PQresultStatus(res) != PGRES_COMMAND_OK){
				system("clear");
				printf("+\n");
			}else{
				system("clear");
				printf("+ Edição OK\n");
			}
			PQclear(res);
		}
	}
}
// Funcao resopnsavel por verificar a conta
int verifica_conta(int numero){
	char sl_num[100], select[1000];
	int num,i;
	PGconn *conn = PQconnectdb(STR_CON);
	PGresult *res;
	sprintf(select,"SELECT * from conta where pessoa_idpessoa = '%d'",numero);
	res = PQexec(conn,select);
	int rows = PQntuples(res);
	if(rows)
		return (1);
	else
		return (0);	
}

// Funcao responsavel por verificar o login
int login(char *login, char *senha){ 
	PGconn *conn = PQconnectdb(STR_CON);
	char str_login[500];
	sprintf(str_login,"SELECT * from conta where \"numero\" = '%s' AND \"senha\" = '%s'", login, senha);
	PGresult *res = PQexec(conn,str_login);
	int rows = PQntuples(res);
	if(rows)
		return (1);
	else
		return (0);
}

// Funcao responsavel por pegar o nivel do ususario - Root/Comum
char nv_login(char *login, char *senha){ 
	PGconn *conn = PQconnectdb(STR_CON);
	char str_login[500];
	char lvl[100];
	sprintf(str_login,"SELECT nivel_acesso from conta where \"numero\" = '%s' AND \"senha\" = '%s'", login, senha);
	PGresult *res = PQexec(conn,str_login);
	sprintf(lvl,"%s",PQgetvalue(res, 0, 0));
	PQclear(res);
	sprintf(str_login,"SELECT pessoa_idpessoa from conta where \"numero\" = '%s' AND \"senha\" = '%s'", login, senha);
	res = PQexec(conn,str_login);
	sprintf(id_usr,"%s",PQgetvalue(res, 0, 0));
	return (*lvl);
}
int i=0;
// Funcao responsalve por inserir usuarios
void form_include_usuario(){
	// Cria conexão com banco de dados
	PGconn *conn = PQconnectdb(STR_CON); 
	char nome[500];
	int ok = 0;
	struct pessoa pessoas[1000];
	printf("\n");
	printf("+Informe o nome: ");
	fgets(nome,500,stdin);
	nome[strlen(nome)-1] = '\0';
	separa_nome(nome,strlen(nome),pessoas,&i);

	do{
		do{
			ok = 0;
			printf("+Informe o CPF: ");
			fgets(pessoas[i].CPF,100,stdin);
			pessoas[i].CPF[strlen(pessoas[i].CPF)-1] = '\0';
			ok = valida_cpf(pessoas[i].CPF,strlen(pessoas[i].CPF));
			if(!ok)
				printf("+CPF inválido!\n");
		}while(!ok);
		ok = 0;
		ok = verifica_cpf(pessoas[i].CPF);
		if(ok)
			printf("+CPF já utilizado!\n");
	}while(ok);

	insert_pessoa(conn,pessoas,&i); // chama função para inserir no banco
	i++;
}

// Funcao responsavel por verificar CPF
int verifica_cpf(char * cpf){ 
	char sl_num[100], select[1000];
	int i,k =0,tamanho, ok, t=0, j;
	int num[11], n_cpf[11];
	PGconn *conn = PQconnectdb(STR_CON);
	PGresult *res;
	sprintf(select,"SELECT cpf from pessoa");
	res = PQexec(conn,select);
	int rows = PQntuples(res);
	tamanho = strlen(cpf);
	for(i=0;i<tamanho;i++){
		n_cpf[k] = cpf[i] - '0';
		if(n_cpf[k] >= 0 && n_cpf[k] <= 9)
			k++;
	}
	do{
		for(i=0; i<rows; i++) {
			k=0;
			sprintf(sl_num,"%s",PQgetvalue(res, i, 0));
			tamanho = strlen(sl_num);
			for(j=0;j<tamanho;j++){
				num[k] = sl_num[j] - '0';
				if(num[k] >= 0 && num[k] <= 9)
					k++;
			}
			ok = 0;
			for(j=0;j<11;j++){
				if(n_cpf[j] == num[j]){
					ok++;
				}
				if(ok==11){
					t++;
					printf("%d - %d - ok %d - t %d\n",n_cpf[j],num[j],ok,t);
					break;
				}
			}
		}
		if(t==0)
			break;
	}while(!t);
	PQclear(res);
	return (t);
}

// Funcao responsavel por inserir pessoas no banco de dados
int insert_pessoa(PGconn *conn, struct pessoa *dadosCliente, int *i){ 
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		char insert[500];
		sprintf(insert,"INSERT INTO Pessoa (\"primeiro_nome\",\"ultimo_nome\",\"cpf\") VALUES('%s','%s','%s')", (*(dadosCliente+*i)).primeiro_nome, (*(dadosCliente+*i)).ultimo_nome, (*(dadosCliente+*i)).CPF);
		res = PQexec(conn, insert);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Inserção ====== OK\n");
		PQclear(res);
		res = PQexec(conn, "SELECT max(idPessoa) from pessoa");
		sprintf(id,"%s",PQgetvalue(res, 0, 0));
		return (1);
	}
}
// Form para incluisao da conta
void form_include_conta(int idpessoa){ 
	int i=0;
	char nome[100], select[1000];
	struct conta contas[1000];
	PGconn *conn = PQconnectdb(STR_CON);
	PGresult *res;
	sprintf(select,"SELECT primeiro_nome from pessoa where idPessoa = '%d'", idpessoa);
	idusr = idpessoa;
	res = PQexec(conn,select);
	sprintf(nome,"%s",PQgetvalue(res, 0, 0));
	
	do{
		gera_numero(contas,&i);
	}while(!verifica_numero(contas[i].numero));
	do{
		printf("+Informe uma senha (até 6 caracteres) para o usuário \"%s\": ",nome);
		fgets(contas[i].senha,100,stdin);
		contas[i].senha[strlen(contas[i].senha)-1] = '\0';
		tratamento(contas[i].senha,strlen(contas[i].senha)); //trata strings
		if(strlen(contas[i].senha)>6 || strlen(contas[i].senha) < 1){
			printf("Senha ultrapassou a quantidade máxima de caracteres\n");
		}
	}while(strlen(contas[i].senha) > 6);

	printf("+Informe o limite de crédito para o usuário \"%s\": ",nome);
	scanf("%lf",&contas[i].limite);
	
	printf("+Informe o saldo inicial para o usuário \"%s\": ",nome);
	scanf("%lf",&contas[i].saldo);
	
	printf("+Informe o nível de acesso para o usuário \"%s\" (0 = ROOT | 1/VAZIO = NORMAL): ",nome);
	scanf("%d",&contas[i].nivel_acesso);

	insert_conta(conn,contas,&i);
	i++;

}

// Funcao responsavel por verificar numero da conta
int verifica_numero(int numero){ 
	char sl_num[100], select[1000];
	int num,i;
	PGconn *conn = PQconnectdb(STR_CON);
	PGresult *res;
	sprintf(select,"SELECT numero from conta");
	res = PQexec(conn,select);
	int rows = PQntuples(res);
	for(i=0; i<rows; i++) {
		sprintf(sl_num,"%s",PQgetvalue(res, i, 0));
		num = atoi(sl_num);
		if(numero == num){
			PQclear(res);
			return (0);
		}
	}
	return (1);	
}
// Funcao responsavel por inserir dados de conta no banco de dados
int insert_conta(PGconn *conn, struct conta *contas, int *i){ 
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		char insert[500];
		sprintf(insert,"INSERT INTO conta (\"pessoa_idpessoa\",\"numero\",\"senha\",\"limite\",\"saldo\",\"nivel_acesso\") VALUES('%d','%d','%s','%lf','%lf','%d')", idusr, (*(contas+*i)).numero, (*(contas+*i)).senha,(*(contas+*i)).limite, (*(contas+*i)).saldo, (*(contas+*i)).nivel_acesso);
		res = PQexec(conn, insert);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			do_exit(conn, res);
		else
			printf("Inserção ====== OK\n");
		PQclear(res);
		return (1);
	}
}	

//------------------------------------------------------------------------------------------gera numero da conta
void gera_numero(struct conta *contas, int *i){ 
	int numRand[5], j, quinto=0, numeroConta=0;
	char strNum[5];
	// Gera os 4 primeiros digitos aleatoriamente indo de 0 a 9
	for(j=0; j<4; j++){
		// O primeiro numero nao armazena 0, entao soma 1
		if(j==0)
			numRand[j] = (rand() % 10)+1;
		else
			numRand[j] = rand() % 10;
		// Gera o quinto digito multiplicando aleatorio*9 aleatorio*8...
		quinto += numRand[j]*(9-j);
	}
	// Faz o mod
	quinto=(quinto%11);
	// Atribui 0 caso o mod retorne 10
	if(quinto==10)
		quinto=0;
	// Atribui ao numero da conta o digito verificador
	numRand[4] = quinto;
	// Concatena os numeros gerados
	sprintf(strNum, "%d%d%d%d%d", numRand[0], numRand[1], numRand[2], numRand[3], numRand[4]);
	// Converte o valor para int
	numeroConta = atoi(strNum);
	printf("+O numero da conta é \"%d\"\n" , numeroConta);
	(*(contas+*i)).numero = numeroConta;
}

//Funca responsavel por separar primeiro e ultimo nome
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
			(*(dadosCliente+*l)).primeiro_nome[strlen((*(dadosCliente+*l)).primeiro_nome)] = '\0';
		}
		for(i=(pos_ultimo+1);i<tamanho;i++,k++){
			(*(dadosCliente+*l)).ultimo_nome[k] = nome[i];
			(*(dadosCliente+*l)).ultimo_nome[strlen((*(dadosCliente+*l)).ultimo_nome)] = '\0';
		}
	}
}

//Funcao responsavel por validar o CPF
int valida_cpf(char *cpf, int tamanho){
	int num[11],j,i,k=0, multiplica = 0, resto, verificador[2];
	for(i=0;i<tamanho;i++){
		num[k] = cpf[i] - '0';
		if(num[k] >= 0 && num[k] <= 9)
			k++;
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

// Funcao responsavel por efetuar o pagamento de uma conta
void user_pagar(int valor, char *categoria, char *data){
	// Conexao do Postgres
	PGconn *conn = PQconnectdb(STR_CON);
	// Checa se a conexao foi bem sucedida
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		char insert[500], select[500], update[500], id_Cat[500], limite[6];
		int escolha=0, pagar=1, j=0, id_Categ, rows;
		double total=0.0, saldo;
		// Faz o insert da categoria - se nao existir
		sprintf(select,"SELECT idcateg FROM categ WHERE nome = '%s'", categoria);
		res = PQexec(conn,select);
		// Se a categoria nao existir cria uma nova
		if(PQgetisnull(res, 0, 0)){
			system("clear");
			printf("Deseja inserir uma nova categoria? [0 = NAO/1 = SIM]: ");
			getchar();
			scanf("%d", &escolha);
			if(escolha){
				sprintf(insert,"INSERT INTO categ (\"nome\") VALUES ('%s')", categoria);
				res = PQexec(conn, insert);
				if (PQresultStatus(res) != PGRES_COMMAND_OK){ 
					do_exit(conn, res);
				}else{
					printf("Categoria inserida ====== OK\n");	
				}
			}
		}
		// Verifica se o usuario tem limite
		sprintf(select, "SELECT limite FROM conta WHERE pessoa_idpessoa = '%s'", id_usr);
		res = PQexec(conn, select);
		sprintf(limite,"%s",PQgetvalue(res, 0, 0));
		if(valor > atoi(limite)){
			printf("Limite insuficiente!! Deseja efetuar pagamento? [1 - SIM/0 - NAO]: ");
			getchar();
			scanf("%d", &pagar);
			printf("%d", pagar);
			if(pagar){
				// Somar todos os valores ainda não pagos
				sprintf(select, "SELECT valor FROM gasto WHERE pago = 0");
				res = PQexec(conn, select);
				rows = PQntuples(res);
				printf("%d", rows);
				for(j=0; j<rows; j++){
					total += atoi(PQgetvalue(res, j, 0));
				}
				printf("Divida %lf", total);
				// Verifica se o valor a_pagar é menor ou igual ao saldo disponivel
				sprintf(select, "SELECT saldo FROM conta WHERE pessoa_idpessoa = '%s'", id_usr);
				res = PQexec(conn, select);
				saldo = atoi(PQgetvalue(res, 0, 0));
				if(total<=saldo)
					printf("Você não tem saldo suficiente! Favor faca um deposito\n");
				else{
					// Se o usuario tiver saldo, subtrai o valor do pagamento
					sprintf(update, "UDPATE conta SET saldo=saldo-%.2lf WHERE pessoa_idpessoa = '%s'", total, id_usr);
					res = PQexec(conn, update);
					if(PQresultStatus(res) != PGRES_COMMAND_OK){
						do_exit(conn, res);
					}else{
						printf("Pagamento efetuado ====== OK\n");
						sprintf(update, "UPDATE conta SET limite=limite+%.2lf WHERE pessoa_idpessoa = '%s'", total, id_usr);
						res = PQexec(conn, update);
						if(PQresultStatus(res) != PGRES_COMMAND_OK){
							do_exit(conn, res);
						}else{
							printf("Limite atualizado ====== OK\n");
						}
						// Marca as contas que já foram pagas
						sprintf(update, "UPDATE gasto SET pago=1 WHERE pessoa_idpessoa = '%s'", id_usr);
						res = PQexec(conn, update);
						if(PQresultStatus(res) != PGRES_COMMAND_OK){
							do_exit(conn, res);
						}else{
							printf("Contas pagas ====== OK\n");
						}
					}
				}
			}
		}else{
			// Se tiver limite, faz o update do limtie
			sprintf(update, "UPDATE conta SET limite=limite-%d WHERE pessoa_idpessoa = '%s'", valor, id_usr);
			res = PQexec(conn, update);
			if(PQresultStatus(res) != PGRES_COMMAND_OK){
				do_exit(conn, res);
			}else{
				printf("Pagamento no credito ====== OK\n");
			}
			// Pega o id da categoria
			sprintf(select, "SELECT idcateg FROM categ WHERE nome = '%s'", categoria);
			res = PQexec(conn, select);
			id_Categ = atoi(PQgetvalue(res, 0, 0));
			// Cadastra que a pesoa gastou um valor naquela categoria
			sprintf(insert, "INSERT INTO gasto (\"categ_idcateg\",\"pessoa_idpessoa\",\"dia\",\"valor\",\"pago\") VALUES('%d','%s','%s','%d','0')", id_Categ, id_usr, data, valor);
			res = PQexec(conn, insert);
			if(PQresultStatus(res) != PGRES_COMMAND_OK){
				do_exit(conn, res);
			}else{
				printf("Gasto cadastrado ====== OK\n");
			}
		}
		PQclear(res);
	}
}

// Funcao responsavel por efetuar um saque
void saque(int valor){
	PGconn *conn = PQconnectdb(STR_CON);
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		char select[500], update[500];
		// Verifica se o usuario tem saldo para sacar
		sprintf(select, "SELECT saldo FROM conta WHERE pessoa_idpessoa = '%s'", id_usr);
		res = PQexec(conn, select);
		if(valor>atoi(PQgetvalue(res, 0, 0))){
			printf("Saldo insuficiente!");
		}else{
			// Se tiver saldo, efetua o saque
			sprintf(update, "UPDATE conta SET saldo=saldo-%d WHERE pessoa_idpessoa = '%s'", valor, id_usr);
			res = PQexec(conn, update);
			if(PQresultStatus(res) != PGRES_COMMAND_OK){
				do_exit(conn, res);
			}else{
				printf("Saque efetuado ====== OK\n");
			}
		}
	}
}

// Funcao responsavel por efetuar um deposito
void deposito(int valor){
	PGconn *conn = PQconnectdb(STR_CON);
	if(PQstatus(conn) != CONNECTION_BAD){
		PGresult *res;
		char update[500];
		// Efetua o update do usuario
		sprintf(update, "UPDATE conta SET saldo=saldo+%d WHERE pessoa_idpessoa = '%s'", valor, id_usr);
		res = PQexec(conn, update);
		if(PQresultStatus(res) != PGRES_COMMAND_OK){
			do_exit(conn, res);
		}else{
			printf("Saque efetuado ====== OK\n");
		}
	}
}
