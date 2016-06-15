CREATE TABLE Pessoa (
  idPessoa SERIAL  NOT NULL ,
  Nome VARCHAR(80)   NOT NULL ,
  CPF VARCHAR(20)   NOT NULL   ,
PRIMARY KEY(idPessoa));




CREATE TABLE Categ (
  idCateg SERIAL  NOT NULL ,
  Nome VARCHAR(50)   NOT NULL   ,
PRIMARY KEY(idCateg));




CREATE TABLE Conta (
  idConta SERIAL  NOT NULL ,
  Pessoa_idPessoa INTEGER   NOT NULL ,
  Numero INTEGER   NOT NULL ,
  Senha VARCHAR(6)   NOT NULL ,
  Limite DOUBLE PRECISION   NOT NULL ,
  Saldo DOUBLE PRECISION   NOT NULL   ,
PRIMARY KEY(idConta)  ,
  FOREIGN KEY(Pessoa_idPessoa)
    REFERENCES Pessoa(idPessoa));


CREATE INDEX Conta_FKIndex1 ON Conta (Pessoa_idPessoa);


CREATE INDEX IFK_Rel_01 ON Conta (Pessoa_idPessoa);


CREATE TABLE Gasto (
  idGasto SERIAL  NOT NULL ,
  Categ_idCateg INTEGER   NOT NULL ,
  Pessoa_idPessoa INTEGER   NOT NULL ,
  Lugar VARCHAR(50)   NOT NULL ,
  Dia DATE   NOT NULL ,
  Valor DOUBLE PRECISION   NOT NULL   ,
PRIMARY KEY(idGasto)    ,
  FOREIGN KEY(Pessoa_idPessoa)
    REFERENCES Pessoa(idPessoa),
  FOREIGN KEY(Categ_idCateg)
    REFERENCES Categ(idCateg));


CREATE INDEX Gasto_FKIndex1 ON Gasto (Pessoa_idPessoa);
CREATE INDEX Gasto_FKIndex2 ON Gasto (Categ_idCateg);


CREATE INDEX IFK_Rel_02 ON Gasto (Pessoa_idPessoa);
CREATE INDEX IFK_Rel_03 ON Gasto (Categ_idCateg);



