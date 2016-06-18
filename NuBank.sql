--usar banco de dados direto do terminal

CREATE DATABASE NuBank;

\c nubank

CREATE TABLE pessoa (
  idpessoa SERIAL  NOT NULL ,
  primeiro_nome VARCHAR(80)   NOT NULL ,
  cpf VARCHAR(20)   NOT NULL   ,
  ultimo_nome VARCHAR(80)   NOT NULL ,
PRIMARY KEY(idpessoa));


CREATE TABLE categ (
  idcateg SERIAL  NOT NULL ,
  nome VARCHAR(50)   NOT NULL   ,
PRIMARY KEY(idcateg));



CREATE TABLE conta (
  idconta SERIAL  NOT NULL ,
  pessoa_idpessoa INTEGER   NOT NULL ,
  numero INTEGER   NOT NULL ,
  senha VARCHAR(6)   NOT NULL ,
  limite DOUBLE PRECISION   NOT NULL ,
  saldo DOUBLE PRECISION   NOT NULL   ,
  nivel_acesso INTEGER   NOT NULL DEFAULT 1 ,
PRIMARY KEY(idconta)  ,
  FOREIGN KEY(pessoa_idpessoa)
    REFERENCES pessoa(idpessoa));


CREATE INDEX conta_fkindex1 ON conta (pessoa_idpessoa);


CREATE INDEX ifk_rel_01 ON conta (pessoa_idpessoa);


CREATE TABLE gasto (
  idgasto SERIAL  NOT NULL ,
  categ_idcateg INTEGER   NOT NULL ,
  pessoa_idpessoa INTEGER   NOT NULL ,
  lugar VARCHAR(50)   NOT NULL ,
  dia DATE   NOT NULL ,
  valor DOUBLE PRECISION   NOT NULL   ,
PRIMARY KEY(idgasto)    ,
  FOREIGN KEY(pessoa_idpessoa)
    REFERENCES pessoa(idpessoa),
  FOREIGN KEY(categ_idcateg)
    REFERENCES categ(idCateg));


CREATE INDEX gasto_fkindex1 ON gasto (pessoa_idpessoa);
CREATE INDEX gasto_fkindex2 ON gasto (categ_idcateg);


CREATE INDEX ifk_rel_02 ON gasto (pessoa_idpessoa);
CREATE INDEX ifk_rel_03 ON gasto (categ_idcateg);


INSERT INTO pessoa ("primeiro_nome", "ultimo_nome","cpf") VALUES('Joao','Labres','09984187900');
INSERT INTO Conta ("pessoa_idpessoa","numero","senha","limite","saldo","nivel_acesso") VALUES('1','10000','rootjv','9999.99','9999.99','0');
