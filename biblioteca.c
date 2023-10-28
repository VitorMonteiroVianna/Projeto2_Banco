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

void *criarExt(char cpf_extrato[12]) {
  char path[30];
  sprintf(path, "CLIENTES/%s/extrato.bin", cpf_extrato);
  FILE *extrato = fopen(path, "wb");
  fclose(extrato);
  return 0;
}

//Funcao feita para achar um cliente especifico
Cliente *procuraCliente(char cpf[12]) { 
  char path[30];
  sprintf(path, "CLIENTES/%s/info.bin", cpf); // Defino o caminho

  //Inicia a logica de verificação do cliente
  Cliente *pCliente = (Cliente *)malloc(sizeof(Cliente)); 
  FILE *t = fopen(path, "rb"); 
  if (t == NULL)            
    return NULL;
  fread(pCliente, sizeof(Cliente), 1, t);
  fclose(t);
  return pCliente;
}


// Cria um novo cliente
void novo_cliente() {
  limpaBuffer();

  Cliente novoCliente;
  Cliente *pCliente = (Cliente *)malloc(sizeof(Cliente)); // Ponteiro para a struct Cliente

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
  printf("Valor inicial: R$%.2lf\n", pCliente -> valor_init);
  printf("Senha: %s\n", pCliente -> senha);

  char path[50]; // create a buffer para o caminho do diretorio
  sprintf(path, "CLIENTES/%s", pCliente->CPF); // gerar o caminho do diretorio
  mkdir(path); // criar o diretorio

  // Abrindo o arquivo para colocar os valores la dentro.
  sprintf(path, "CLIENTES/%s/info.bin", pCliente->CPF); // gerar o caminho do arquivo
  FILE *f = fopen(path, "wb");
  fwrite(pCliente, sizeof(Cliente), 1, f);
  fclose(f);

  // cria o arquivo extrato.txt dentro da pasta do cliente
  criarExt(pCliente->CPF); 

  free(pCliente); // libera a memoria alocada para o ponteiro pCliente
  limpaBuffer();
  return;
}

void apaga_cliente() {
  char cpf_excluido[12]; // declaro a variavel que vai receber o CPF que será excluido
  printf("\n=====================================\n");
  printf("OK, Vamos apagar sua conta!\n");
  printf("Para isso, basta que me passe seu CPF:\n->");
  scanf("%s", cpf_excluido);

  // Usa a função procurar clientes, para achar o arquivo correto
  Cliente *pCliente = procuraCliente(cpf_excluido);

  // Confere se o CPF existe
  if (pCliente == NULL) {
    printf("\nEsse CPF não pertence a nenhum cliente!\n");
    return; 
  }

  //Definos as variaveis que serão responsaveis por receber os caminhos dos arquivos
  char pastaCliente_excluir[30];
  char caminhoCompleto[100]; 


  // caminho para apagar o arquivo cliente
  sprintf(pastaCliente_excluir, "CLIENTES/%s/info.bin", cpf_excluido);
  remove(pastaCliente_excluir); // arquivo cpf apagado
  // caminho para apagar o arquivo TXT
  sprintf(pastaCliente_excluir, "CLIENTES/%s/extrato.bin", cpf_excluido);
  remove(pastaCliente_excluir); // arquivo txt apagado
  // apagando o diretorio
  strcpy(caminhoCompleto, "CLIENTES/");
  strcat(caminhoCompleto, cpf_excluido);
  rmdir(caminhoCompleto); // a pasta referente ao cpf foi removida
  printf("\nCONTA REMOVIDA!\n");
  sleep(1);
  printf("Ate logo...\n");
  sleep(1);

  limpaBuffer();
}