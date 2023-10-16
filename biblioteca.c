//
// Created by vmont on 28/09/2023.
//

#include "biblioteca.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// funcao para limpar o lixo de memoria
void limpaBuffer() {
  char a;
  do {
    a = getchar();
  } while (a != '\n' && a != EOF);
}

void novo_cliente() {
  limpaBuffer();

  Cliente novoCliente;
  Cliente *pCliente = &novoCliente; // Ponteiro para a struct Cliente

  // sessao UX
  printf("Bem vindo ao Banco Quem Poupa Tem\n");
  printf("Para presseguir com a criação da sua conta preciso que você me "
         "formneça algumas informações, ok?\n");
  printf("Vamos lá\n");

  // Solicita informações do cliente
  printf("Para começar, digite o seu nome: ");
  fgets(pCliente->nome, sizeof(pCliente->nome), stdin);
  pCliente->nome[strcspn(pCliente->nome, "\n")] =
      '\0'; // Remove o caractere de nova linha

  printf("\nMuito bem\n");
  printf("Agora, informe seu CPF: ");
  fgets(pCliente->CPF, sizeof(pCliente->CPF), stdin);
  pCliente->CPF[strcspn(pCliente->CPF, "\n")] =
      '\0'; // Remove o caractere de nova linha

  printf("\nOk\n");
  printf("Agora vamos escolher qual tipo de conta você deseja criar\n");
  printf("Nosso banco trabalha com 2 opções\n");
  printf("\n1 - CONTA COMUM: 5%% de taxa a cada debito realizado\n");
  printf("Você poderá ter um saldo negativo de até RS 1.000,00\n");
  printf("\n2 - CONTA PLUS: 3%% de taxa a cada debito realizado\n");
  printf("Você poderá um saldo negativo de ate RS 5.000,00\n");

  // condicao para que o valor da conta seja somente 1 ou 2
  do {
    printf("\nDigite 1 para COMUM ou 2 para PLUS): ");
    scanf("%d", &(pCliente->account_type));
    if (pCliente->account_type != 1 && pCliente->account_type != 2)
      printf("Dados invalidos!\n");

  } while (pCliente->account_type != 1 && pCliente->account_type != 2);

  // garante que o valor inicial seja maior que zero
  do {
    printf("\nCerto, estamos quase terminando\n");
    printf("Informe o valor inicial da sua conta: \n");
    scanf("%lf", &(pCliente->valor_init));
    if (pCliente->valor_init < 0)
      printf("Por favor digite um valor maior que zero\n");
  } while (pCliente->valor_init < 0);

  printf("\nAgora vamos finalizar\n");
  printf("Escolha uma senha segura e não compartilhe com niguém\n");
  scanf("%s", pCliente->senha);

  // Exibe os dados do cliente
  printf("\nDados do novo cliente:\n");
  printf("Nome: %s\n", pCliente->nome);
  printf("CPF: %s\n", pCliente->CPF);
  printf("Tipo de conta: %d\n", pCliente->account_type);
  printf("Valor inicial: %lf\n", pCliente->valor_init);
  printf("Senha: %s\n", pCliente->senha);
  limpaBuffer();
  return;
}
