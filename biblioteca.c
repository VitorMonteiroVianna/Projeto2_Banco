#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

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

// Funcao para criar o arquivo extrato.txt
void *criarExt(char cpf_extrato[12]) {
  limpaBuffer();
  // Pega o caminho do arquivo
  char path[30];
  sprintf(path, "CLIENTES/%s/extrato.txt", cpf_extrato);
  // Cria o arquivo
  FILE *extrato = fopen(path, "w");
  fclose(extrato);
  return 0;
}

//Função que irá inserir as informações de extrato formatadas
void insereExtrato(const char *CPF, const char *mensagem) {
    // Define o caminho para o extrato
    char caminhoArquivo[100];
    snprintf(caminhoArquivo, sizeof(caminhoArquivo), "CLIENTES/%s/extrato.txt", CPF);

    //Abre o arquivo e trata um possivel erro
    FILE *arquivo = fopen(caminhoArquivo, "a");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de extrato");
        return;
    }

    // Cria as variaveis com data e hora
    time_t tempoAtual;
    struct tm *infoTempo;
    char dataHora[100];
    time(&tempoAtual);
    infoTempo = localtime(&tempoAtual);
    strftime(dataHora, sizeof(dataHora), "%Y-%m-%d  %H:%M:%S", infoTempo);

    // Faz a formatação da mensagem
    char linha[256];
    snprintf(linha, sizeof(linha), "|%s| ==> %s\n", dataHora, mensagem);

    // Insere as informacoes e fecha o arquivo
    fputs(linha, arquivo);
    fclose(arquivo);
}


// Funcao feita para achar um cliente especifico
Cliente *procuraCliente(char cpf[12]) {
  limpaBuffer();
  // Defino o caminho do arquivo
  char path[30];
  sprintf(path, "CLIENTES/%s/info.bin", cpf);

  //Inicia a logica de verificação do cliente

  // Cria um ponteiro para o arquivo
  Cliente *pCliente = (Cliente *)malloc(sizeof(Cliente)); 
  FILE *t = fopen(path, "rb");
  // Verifica se o arquivo existe
  if (t == NULL)            
    return NULL;
  fread(pCliente, sizeof(Cliente), 1, t);
  fclose(t);
  return pCliente;
}

// Função criada para retornar as informações formatadas de um cliente especifico 
void pegaInfo_cliente(const char *cpf) {
    limpaBuffer();
    char nomePasta[120]; 
    //Cria o caminho completo para abrir a pasta do cliente
    snprintf(nomePasta, sizeof(nomePasta), "CLIENTES/%s", cpf);

    // Cria o caminho completo para abrir o arquivo info.bin
    char nomeArquivo[150];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s/info.bin", nomePasta);

    // Abre o arquivo
    FILE *arquivo = fopen(nomeArquivo, "rb");

    Cliente cliente;

    //Le o arquivo e passa seu valores para o Struct cliente
    size_t lidos = fread(&cliente, sizeof(Cliente), 1, arquivo);

    printf("   Nome: %s\n", cliente.nome);
    printf("   Tipo de conta: %d\n", cliente.account_type);
    printf("   Valor inicial: %.2f\n", cliente.saldo);
    printf("   Senha: %s\n\n", cliente.senha);

    fclose(arquivo);
}

// Funcao para atualizar o saldo
void *atualiza_cliente(const char *cpf, Cliente *cliente){
  // Defino o caminho do arquivo
  char path[30];
  sprintf(path, "CLIENTES/%s/info.bin", cpf);

  // Abre o arquivo
  FILE *a = fopen(path, "wb");
  // Verifica se o arquivo existe
  if (a == NULL)            
    return NULL;
  fwrite(cliente, sizeof(Cliente), 1, a);
  fclose(a);
  return 0;
}

// Cria um novo cliente
void novo_cliente() {
  limpaBuffer();

  Cliente novoCliente;
  Cliente *pCliente = (Cliente *)malloc(sizeof(Cliente)); // Ponteiro para a struct Cliente

  // Sessao UX
  printf("Bem vindo ao Banco Quem Poupa Tem\n");
  printf("Para continuar com a criacao da sua conta preciso que voce me "
         "forneca algumas informacoes, ok?\n");
  printf("Vamos la\n");

  // Solicita informacoes do cliente
  printf("Para comecar, digite o seu nome: ");
  fgets(pCliente -> nome, sizeof(pCliente -> nome), stdin);
  pCliente -> nome[strcspn(pCliente -> nome, "\n")] = '\0'; // Remove o caractere de nova linha

  int valido = 0;
  do{
      printf("\nMuito bem\n");
      printf("Agora, informe seu CPF: ");
      fgets(pCliente -> CPF, sizeof(pCliente -> CPF), stdin);
      pCliente -> CPF[strcspn(pCliente -> CPF, "\n")] = '\0'; // Remove o caractere de nova linha

      // Verifica se o CPF tem 11 digitos
      if (strlen(pCliente -> CPF) == 11) {
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
    scanf("%d", &(pCliente -> account_type));
    if (pCliente -> account_type != 1 && pCliente -> account_type != 2)
      printf("Dados invalidos!\n");

  } while (pCliente -> account_type != 1 && pCliente -> account_type != 2);

  // garante que o valor inicial seja maior que zero
  do {
    printf("\nCerto, estamos quase terminando\n");
    printf("Informe o valor inicial da sua conta: \n");
    scanf("%lf", &(pCliente -> saldo));
    if (pCliente -> saldo < 0)
      printf("Por favor digite um valor maior que zero\n");
  } while (pCliente -> saldo < 0);

  printf("\nAgora vamos finalizar\n");
  printf("Escolha uma senha segura e nao compartilhe com ninguem\n");
  scanf("%s", pCliente -> senha);

  // Exibe os dados do cliente
  printf("\nDados do novo cliente:\n");
  printf("Nome: %s\n", pCliente -> nome);
  printf("CPF: %s\n",  pCliente -> CPF);
  printf("Tipo de conta: %d\n", pCliente -> account_type);
  printf("Valor inicial: R$%.2lf\n", pCliente -> saldo);
  printf("Senha: %s\n", pCliente -> senha);

  char path[50]; // create a buffer para o caminho do diretorio
  sprintf(path, "CLIENTES/%s", pCliente -> CPF); // gerar o caminho do diretorio
  mkdir(path); // criar o diretorio

  // Abrindo o arquivo para colocar os valores la dentro.
  sprintf(path, "CLIENTES/%s/info.bin", pCliente -> CPF); // gerar o caminho do arquivo
  FILE *f = fopen(path, "wb");
  fwrite(pCliente, sizeof(Cliente), 1, f);
  fclose(f);

  // cria o arquivo extrato.txt dentro da pasta do cliente
  criarExt(pCliente -> CPF); 

  //Cria a mensagem de texto completa para o extrato
  char mensagemExtrato[100];
  sprintf(mensagemExtrato, "Conta criada: NOME: %s, CPF: %s, Tipo de Conta: %d, Saldo: %2lf", pCliente -> nome, pCliente -> CPF, pCliente -> account_type,pCliente -> saldo);

  //Insere a mensagem formatada no extrato
  insereExtrato(pCliente -> CPF, mensagemExtrato);

  free(pCliente); // libera a memoria alocada para o ponteiro pCliente
  limpaBuffer();
  return;
}

// Funcao para apagar um cliente
void apaga_cliente() {
  limpaBuffer();
  printf("\n=====================================\n");
  printf("OK, Vamos apagar sua conta!\n");

  // Declaro a variavel que vai receber o CPF que será excluido
  char cpf_excluido[12];
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

  // Caminho para apagar o arquivo cliente
  sprintf(pastaCliente_excluir, "CLIENTES/%s/info.bin", cpf_excluido);
  remove(pastaCliente_excluir); 

  // Caminho para apagar o arquivo TXT
  sprintf(pastaCliente_excluir, "CLIENTES/%s/extrato.txt", cpf_excluido);
  remove(pastaCliente_excluir); // arquivo txt apagado

  // Parte responsavel por apagar todos os arquivos dentro da pasta
  snprintf(caminhoCompleto, sizeof(caminhoCompleto), "CLIENTES/%s", cpf_excluido);
  // Abre o diretorio
  DIR *dir = opendir(caminhoCompleto);
  struct dirent *entry;

  // Loop para percorrer todos os arquivos dentro da pasta
  while ((entry = readdir(dir)) != NULL) {
    // Ignora os arquivos . e ..
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      char path[100];
      // Cria o caminho completo para o arquivo
      snprintf(path, sizeof(path), "%s/%s", caminhoCompleto, entry->d_name);
      // Apaga o arquivo
      remove(path);
    }
  }
  closedir(dir);

  // Parte responsavel por apagar a pasta vazia *********
  rmdir(caminhoCompleto); 
  
  printf("\nConta excluida!!\n");

  limpaBuffer();
}

// Funcao para listar todos os clientes
void listaClientes() {
    limpaBuffer();
    const char *diretorio = "CLIENTES";
    struct dirent *entry;
    DIR *dir;

    dir = opendir(diretorio);

    printf("\nLista de clientes ativos (pressione ENTER para continuar):\n");

    //Cria um contador para saber a quantidade de clientes
    int count = 0;

    //Laço while para listar o nome de cada pasta (CPF do cliente)
    while ((entry = readdir(dir)) != NULL) {
        if (entry -> d_type == DT_DIR) {
            if (strcmp(entry -> d_name, ".") != 0 && strcmp(entry -> d_name, "..") != 0) {
                printf("%s\n", entry -> d_name);

                pegaInfo_cliente(entry -> d_name); // Usa o nome da pasta para pegar o CPF e chama a função responsavel por formatar as infos do cliente

                count ++;
            }
        }
    }

    closedir(dir);

    //Caso o contador se mantenha em 0 não existem clientes
    if (count == 0){
      printf("====>Não existem clientes cadastrados");
    }
}

// Funcao para efetuar um debito
void debito (){
  limpaBuffer();
  printf("Bem vindo a area de debito\n");
  printf("Para realizar a operacao preciso que voce me forneca algumas informacoes\n");
  
  // Pega o CPF do cliente
  char cpf_debito[12];
  printf("Primeiro, digite o seu CPF:\n->");
  scanf("%s", cpf_debito);

  // Usa a função procurar clientes, para achar o arquivo correto
  Cliente *pCliente = procuraCliente(cpf_debito);

  // Confere se o CPF existe
  if (pCliente == NULL) {
    printf("\nEsse CPF não pertence a nenhum cliente!\n");
    return; 
  }

  // Pega a senha do cliente
  char senha_d[12];
  printf("Agora, digite a sua senha:\n->");
  scanf("%s", senha_d);

  // Confere se a senha está correta
  if (strcmp(senha_d, pCliente -> senha) != 0) {
    printf("Senha incorreta!\n");
    return;
  }

  // Pega o valor do debito
  double valor;
  printf("Para finalizar, digite o valor que deseja debitar:\n->");
  scanf("%lf", &valor);

  //Declaro o tipo de conta que será usado para facilitar na criação do EXTRATO
  char mensagemExtrato_TipoConta[100];

  // Logica para calcular a taxa de acordo com o tipo de conta e verificacao de saldo negativo
  float taxa;
  if (pCliente -> account_type == 1) {

    //Defino a parte da mensagem de taxa que irá para o extrato
    strcpy(mensagemExtrato_TipoConta, "5%");

    taxa = 1.05;
    double saldo_final = (pCliente -> saldo) - (valor * taxa);
    if (saldo_final < -1000) {
      printf("Saldo insuficiente!\n");
      return;
    }
    else{
        pCliente -> saldo = saldo_final;
        printf("Debito realizado com sucesso!\n");
        printf("Saldo atual: %.2f\n", pCliente -> saldo);
        atualiza_cliente(cpf_debito, pCliente);
        }   
  } else {
    taxa = 1.03;

    //Defino a parte da mensagem de taxa que irá para o extrato
    strcpy(mensagemExtrato_TipoConta, "3%");

    double saldo_final = (pCliente -> saldo) - (valor * taxa);
    if (saldo_final < -5000) {
      printf("Saldo insuficiente!\n");
      return;
    } else {
      pCliente -> saldo = saldo_final;
      printf("Debito realizado com sucesso!\n");
      printf("Saldo atual: %.2f\n", pCliente -> saldo);
      atualiza_cliente(cpf_debito, pCliente);

    };
  };

  //Cria a mensagem de texto completa
  char mensagemExtrato[100];
  sprintf(mensagemExtrato, "DEBITO de %.2lf na conta %s com taxa %s. Novo saldo: %.2lf", valor, cpf_debito, mensagemExtrato_TipoConta, pCliente->saldo);
  insereExtrato(cpf_debito, mensagemExtrato);

  limpaBuffer();  
  free(pCliente); // libera a memoria alocada para o ponteiro pCliente

};

// Funcao para efetuar um deposito
void deposito(){
  limpaBuffer();
  printf("Bem vindo a area de deposito\n");
  printf("Para realizar a operacao preciso que voce me forneca algumas informacoes\n");
  
  // Pega o CPF do cliente
  char cpf_deposito[12];
  printf("Primeiro, digite o CPF da conta em que deseja depositar:\n->");
  scanf("%s", cpf_deposito);

  // Usa a função procurar clientes, para achar o arquivo correto
  Cliente *pCliente = procuraCliente(cpf_deposito);

  // Confere se o CPF existe
  if (pCliente == NULL) {
    printf("\nEsse CPF não pertence a nenhum cliente!\n");
    return; 
  }

  // Pega o valor do deposito
  double valor;
  printf("Para finalizar, digite o valor que deseja depositar:\n->");
  scanf("%lf", &valor);

  pCliente -> saldo = pCliente -> saldo + valor;
  printf("Deposito realizado com sucesso!\n");
  printf("Saldo atual: %.2f\n", pCliente -> saldo);

  // atualiza o saldo do cliente
  atualiza_cliente(cpf_deposito, pCliente);


  char mensagemExtrato[100];
  sprintf(mensagemExtrato, "DEPOSITO de %.2lf na conta %s. Novo saldo: %.2f", valor, cpf_deposito, pCliente -> saldo);
  insereExtrato(cpf_deposito, mensagemExtrato);

  free(pCliente); // libera a memoria alocada para o ponteiro pCliente
  limpaBuffer();
};

//Função que irá exibir o extrato
void exibeExtrato() {
  limpaBuffer();
  printf("Bem vindo a area de EXTRATO\n");
  printf("Para realizar a operacao preciso que voce me forneca algumas informacoes\n");
  
  // Pega o CPF do cliente
  char CPF[12];
  printf("Digite o seu CPF:\n->");
  scanf("%s", CPF);

  // Usa a função procurar clientes, para achar o arquivo correto
  Cliente *pCliente = procuraCliente(CPF);

  // Confere se o CPF existe
  if (pCliente == NULL) {
    printf("\nEsse CPF não pertence a nenhum cliente!\n");
    return; 
  }

  // Pega a senha do cliente
  char senha_d[12];
  printf("Agora, digite a sua senha:\n->");
  scanf("%s", senha_d);

  // Confere se a senha está correta
  if (strcmp(senha_d, pCliente -> senha) != 0) {
    printf("Senha incorreta!\n");
    return;
  }

    // Pega o caminho da arquivo extrato
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "CLIENTES/%s/extrato.txt", CPF);

    // Abre o arquivo com validações de ERRO 
    FILE *arquivo = fopen(caminho_arquivo, "r");
  
    if (arquivo == NULL) {
        printf("Arquivo de extrato não encontrado para o CPF %s. Confira se realmente é um cliente!\n", CPF);
    } else {
        char linha[100];  // Pega as 100 primeiras linhas
        
        printf("Extrato de CPF: %s\n", CPF);
        
        // Exibe o conteudo
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            printf("%s", linha);
        }

        printf("AVISO: essa mensagm pega somente os ultimos 100 registros. Para ver mais registros entre em CLIENTES -> CPF -> EXTRATO\n\n");
        fclose(arquivo);
    }

    free(pCliente); // libera a memoria alocada para o ponteiro pCliente
    limpaBuffer();
}

// Funcao para transferir dinheiro entre contas
void tranferencia(){
  limpaBuffer();
  printf("Bem vindo a area de transferencia\n");
  printf("Para realizar a operacao preciso que voce me forneca algumas informacoes\n");
  
  // Pega o CPF do cliente
  char cpf_origem[12];
  printf("Primeiro, digite o seu CPF:\n->");
  scanf("%s", cpf_origem);

  // Usa a função procurar clientes, para achar o arquivo correto
  Cliente *pCliente1 = procuraCliente(cpf_origem);

  // Confere se o CPF existe
  if (pCliente1 == NULL) {
    printf("\nEsse CPF nao pertence a nenhum cliente!\n");
    return; 
  }

  // Pega a senha do cliente
  char senha_origem[12];
  printf("Agora, digite a sua senha:\n->");
  scanf("%s", senha_origem);

  // Confere se a senha está correta
  if (strcmp(senha_origem, pCliente1 -> senha) != 0) {
    printf("Senha incorreta!\n");
    return;
  }

  // Pega o CPF da conta que vai receber a transferencia
  char cpf_destino[12];
  printf("Agora, digite o CPF da conta para a qual deseja transferir:\n->");
  scanf("%s", cpf_destino);

  // Usa a função procurarCliente, para achar o arquivo correto
  Cliente *pCliente2 = procuraCliente(cpf_destino);

  // Confere se o CPF existe
  if (pCliente2 == NULL) {
    printf("\nEsse CPF não pertence a nenhum cliente!\n");
    return; 
  }

  // Pega o valor do debito
  double valor;
  printf("Para finalizar, digite o valor que deseja transferir:\n->");
  scanf("%lf", &valor);

  double saldo_final = (pCliente1 -> saldo) - valor;
  if (pCliente1 -> account_type == 1) {
    if (saldo_final < -1000) {
      printf("Saldo insuficiente!\n");
      return;
    } else{
        pCliente1 -> saldo = saldo_final;
        pCliente2 -> saldo = pCliente2 -> saldo + valor;
        printf("Transferencia realizada com sucesso!\n");
        printf("Saldo atual: %.2f\n", pCliente1 -> saldo);
        atualiza_cliente(cpf_origem, pCliente1);
        atualiza_cliente(cpf_destino, pCliente2);
    }
  } else {
    if (saldo_final < -5000) {
      printf("Saldo insuficiente!\n");
      return;
    } else {
      pCliente1 -> saldo = saldo_final;
      pCliente2 -> saldo = pCliente2 -> saldo + valor;
      printf("Transferencia realizada com sucesso!\n");
      printf("Saldo atual: %.2f\n", pCliente1 -> saldo);
      atualiza_cliente(cpf_origem, pCliente1);
      atualiza_cliente(cpf_destino, pCliente2);
    };
  };

  //Crio as mensagens para inserir no extrato e as insiro
  char mensagemExtrato_cpfOrigem[100];
  char mensagemExtrato_cpfDestino[100];
  sprintf(mensagemExtrato_cpfOrigem, "Transferencia feita de %.2lf para conta %s. Novo saldo: %.2lf", valor, cpf_destino, pCliente1 -> saldo);
  sprintf(mensagemExtrato_cpfDestino, "Transferencia recebida de %.2lf da conta %s. Novo saldo: %.2lf", valor, cpf_origem, pCliente2 -> saldo);
  insereExtrato(cpf_origem, mensagemExtrato_cpfOrigem);
  insereExtrato(cpf_destino, mensagemExtrato_cpfDestino);


  // adicionar o valor das transferencias nos extratos
  free(pCliente1); // libera a memoria alocada para o ponteiro pCliente
  free(pCliente2); // libera a memoria alocada para o ponteiro pCliente
  limpaBuffer();
}


