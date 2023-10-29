//
// Created by vmont on 28/09/2023.
//

#ifndef PROJETO2_BANCO_BIBLIOTECA_H
#define PROJETO2_BANCO_BIBLIOTECA_H

typedef struct Cliente {
  char nome[100];
  char CPF[12];
  int account_type;
  double valor_init;
  char senha[20];
} Cliente;

void limpaBuffer();

void novo_cliente();

void criarDiretorioClientes();

void apaga_cliente();

void resgataInfo_cliente(const char *cpf);

void listaClientes();


#endif // PROJETO2_BANCO_BIBLIOTECA_H
