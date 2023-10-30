//CODIGO FEITO POR VITOR MONTEIRO VIANNA RA: 22.223.085-6 E HUGO EMILIO NOMURA RA 22.123.051-9


#ifndef PROJETO2_BANCO_BIBLIOTECA_H
#define PROJETO2_BANCO_BIBLIOTECA_H

typedef struct Cliente {
  char nome[100];
  char CPF[12];
  int account_type;
  double saldo;
  char senha[20];
} Cliente;

void limpaBuffer();

void novo_cliente();

void criarDiretorioClientes();

void *atualiza_cliente(const char *cpf, Cliente *cliente);

void apaga_cliente();

void resgataInfo_cliente(const char *cpf);

void listaClientes();

void debito();

void deposito();

void exibeExtrato();

void tranferencia();

#endif
