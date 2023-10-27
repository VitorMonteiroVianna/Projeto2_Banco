#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "biblioteca.h"

// funcao para limpar o lixo de memoria
void limpaBuffer() {
  char a;
  do {
    a = getchar();
  } while (a != '\n' && a != EOF);
}

// Cria uma pasta para armazenar as pastas com os clientes
void criarDiretorioClientes() {
    if (mkdir("CLIENTES") == 0) {
        printf("Diretorio CLIENTES criado com sucesso.\n");
    } else {
        printf("O diretorio CLIENTES ja existe ou ocorreu um erro ao cria-lo.\n");
    }
}

// Cria um novo cliente
void novo_cliente() {
  limpaBuffer();

  Cliente novoCliente;
  Cliente *pCliente = &novoCliente; // Ponteiro para a struct Cliente

  // sessao UX
  printf("Bem vindo ao Banco Quem Poupa Tem\n");
  printf("Para continuar com a criacao da sua conta preciso que voce me "
         "forneca algumas informacoes, ok?\n");
  printf("Vamos la\n");

  // Solicita informacoes do cliente
  printf("Para comecar, digite o seu nome: ");
  fgets(pCliente->nome, sizeof(pCliente->nome), stdin);
  pCliente->nome[strcspn(pCliente->nome, "\n")] = '\0'; // Remove o caractere de nova linha

  int valido = 0;
  do{
      printf("\nMuito bem\n");
      printf("Agora, informe seu CPF: ");
      fgets(pCliente->CPF, sizeof(pCliente->CPF), stdin);
      pCliente->CPF[strcspn(pCliente->CPF, "\n")] = '\0'; // Remove o caractere de nova linha

      // Verifica se o CPF tem 11 digitos
      if (strlen(pCliente->CPF) == 11) {
          valido = 1;
      } else {
          printf("CPF invalido. Tente novamente.\n");
      }
  } while (!valido);


  printf("\nOk\n");
  printf("Agora vamos escolher qual tipo de conta voce deseja criar\n");
  printf("Nosso banco trabalha com 2 opcoes\n");
  printf("\n1 - CONTA COMUM: 5%% de taxa a cada debito realizado\n");
  printf("Voce podera ter um saldo negativo de ate R$ 1.000,00\n");
  printf("\n2 - CONTA PLUS: 3%% de taxa a cada debito realizado\n");
  printf("Voce podera um saldo negativo de ate R$ 5.000,00\n");

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
  printf("Escolha uma senha segura e nao compartilhe com ninguem\n");
  scanf("%s", pCliente->senha);

  // Exibe os dados do cliente
  printf("\nDados do novo cliente:\n");
  printf("Nome: %s\n", pCliente->nome);
  printf("CPF: %s\n",  pCliente -> CPF);
  printf("Tipo de conta: %d\n", pCliente -> account_type);
  printf("Valor inicial: %lf\n", pCliente -> valor_init);
  printf("Senha: %s\n", pCliente -> senha);
  limpaBuffer();

  return;
}