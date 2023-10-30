//CODIGO FEITO POR VITOR MONTEIRO VIANNA RA: 22.223.085-6 E HUGO EMILIO NOMURA RA 22.123.051-9


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "biblioteca.h"

int operador;

int main() {
  criarDiretorioClientes();

  do{
  

    printf("BANCO QUEM POUPA TEM\n\n");
    printf("Digite o numero da opcao desejada:\n");
    printf("1 -> Novo cliente\n");
    printf("2 -> Apagar cliente\n");
    printf("3 -> Listar clientes\n");
    printf("4 -> Efetuar um debito\n");
    printf("5 -> Efetuar um deposito\n");
    printf("6 -> Exibir extrato\n");
    printf("7 -> Tranferencia entre contas\n");
    printf("0 -> Sair\n");

    scanf("%d", &operador);

    switch (operador) {
    case 1:
      novo_cliente();
      break;
    case 2:
      apaga_cliente();
      break;
    case 3:
      listaClientes();
      break;
    case 4:
      debito();
      break;
    case 5:
      deposito();
      break;
    case 6:
      exibeExtrato();
      break;
    case 7:
      tranferencia();
      break;
    case 0:
      printf("Saindo!\n");
      sleep(1);
      break;
    default:
      printf("Digite uma opcao VALIDA!\n");
    }
  } while (operador != 0);
  return  0;
};
