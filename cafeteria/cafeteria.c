#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void realizarVenda() {
  FILE *cadastro;
  FILE *arquivovendas;

  struct Carrinho {
    char produto[101];
    int quantidade;
    int cod;
    float valor;
    int dia, mes, ano;
  };

  int totalProdutos = 0;
  char nome[101];
  int codigo;
  float preco;

  int resposta = 1;
  int indice = 0;
  float totalcarrinho = 0;

  cadastro = fopen("cadastro.txt", "r");
  arquivovendas = fopen("vendas.txt", "a");

  if (cadastro == NULL || arquivovendas == NULL) {
    printf("Falha na leitura dos arquivos\n");
    return;
  }

  // Contar o numero total de produtos no arquivo
  while (fscanf(cadastro, "%[^;];%d;%f\n", nome, &codigo, &preco) != EOF) {
    totalProdutos++;
  }

  struct Carrinho carrinho[2 * totalProdutos];

  do {
    // Reiniciar a leitura do arquivo do inicio para cada busca
    fseek(cadastro, 0, SEEK_SET);

    printf("\nQual o codigo do produto desejado?\n");
    int busca;
    scanf("%d", &busca);

    int produtoEncontrado = 0;

    while (fscanf(cadastro, "%[^;];%d;%f\n", nome, &codigo, &preco) != EOF) {
      if (codigo == busca) {
        produtoEncontrado = 1;
      }
    }

    if (produtoEncontrado == 0) {
      printf("\nProduto nao encontrado!\n");
      printf("\nDeseja adicionar mais algum produto?\n");
      printf("1 - Sim\n0 - Nao\n");
      scanf("%d", &resposta);
    } else if (produtoEncontrado == 1) {
      printf("Qual a quantidade do produto?\n");
      int unidade;
      scanf("%d", &unidade);

      // Logica para obter o nome e preco do produto
      strcpy(carrinho[indice].produto, nome);
      carrinho[indice].quantidade = unidade;
      carrinho[indice].cod = busca;
      carrinho[indice].valor = preco;

      time_t t;
      struct tm *data_hora;
      time(&t);
      data_hora = localtime(&t);

      carrinho[indice].dia = data_hora->tm_mday;
      carrinho[indice].mes = data_hora->tm_mon + 1;
      carrinho[indice].ano = data_hora->tm_year + 1900;

      indice++;
      printf("\nProduto adicionado!\n");
      printf("Deseja adicionar mais algum produto?\n");
      printf("1 - Sim\n0 - Nao\n");
      scanf("%d", &resposta);
    }

  } while (resposta);

  printf("\nProdutos no carrinho:\n\n");
  for (int i = 0; i < indice; i++) {
    totalcarrinho += (carrinho[i].quantidade * carrinho[i].valor);
    printf("Produto: %-12s| Quantidade: %-2d| Valor: R$%.2f\n",
           carrinho[i].produto, carrinho[i].quantidade, carrinho[i].valor);
    fprintf(arquivovendas, "%s;%d;%d;%.2f;%d/%d/%d;%.2f\n", carrinho[i].produto,
            carrinho[i].quantidade, carrinho[i].cod, carrinho[i].valor,
            carrinho[i].dia, carrinho[i].mes, carrinho[i].ano, totalcarrinho);
  }

  printf("\nTotal da Compra:%.2f\n\n", totalcarrinho);

  fclose(cadastro);
  fclose(arquivovendas);

  printf("Pressione Enter para voltar ao Menu\n");
  getchar();
}

void consultaProduto() {
  FILE *arquivo;

  char nome[101];
  int cod;
  float preco;

  arquivo = fopen("cadastro.txt", "r");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  printf("\n\nTabela de Produtos Cadastrados\n");
  printf("=====================================================\n");

  while (fscanf(arquivo, "%[^;];%d;%.2f\n", nome, &cod, &preco) != EOF) {
    printf("Nome: %s\n", nome);
    printf("Codigo: %d\n", cod);
    printf("Preco: %.2f\n", preco);
    printf("=====================================================\n");
  }

  fclose(arquivo);

  printf("Pressione Enter para voltar ao Menu\n");
  getchar();
}

void cadastraProduto() {
  struct Produtos {
    char nome[101];
    int cod;
    float preco;
  };

  struct Produtos produto;

  FILE *arquivo;

  arquivo = fopen("cadastro.txt", "a");

  if (arquivo == NULL) {
    printf("\nErro ao abrir o arquivo 'cadastro.txt' para escrita.\n");
    return;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;

  printf("\nDigite o nome do produto:\n");
  fgets(produto.nome, sizeof(produto.nome), stdin);
  produto.nome[strcspn(produto.nome, "\n")] = '\0';

  printf("\nDigite o codigo do produto:\n");
  scanf("%d", &produto.cod);
  getchar();

  printf("\nDigite o valor do produto:\n");
  scanf("%f", &produto.preco);

  // Limpar o buffer apos a leitura do preco
  while ((c = getchar()) != '\n' && c != EOF)
    ;

  fprintf(arquivo, "%s;%d;%.2f\n", produto.nome, produto.cod, produto.preco);

  fclose(arquivo);

  printf("\nProduto cadastrado\n");

  printf("\nPressione Enter para voltar ao Menu\n");
}

void excluiProduto() {
  FILE *arquivo, *arquivoTemp;
  char nome[101];
  int cod;
  float preco;
  int codigo;

  arquivo = fopen("cadastro.txt", "r");
  arquivoTemp = fopen("temp.txt", "w");

  if (arquivo == NULL || arquivoTemp == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  printf("\nDigite o codigo do produto que deseja excluir\n\n");

  scanf("%d", &codigo);

  while (fscanf(arquivo, "%[^;];%d;%f\n", nome, &cod, &preco) != EOF) {
    if (cod != codigo) {
      fprintf(arquivoTemp, "%s;%d;%.2f\n", nome, cod, preco);
    }
  }

  fclose(arquivo);
  fclose(arquivoTemp);

  remove("cadastro.txt");

  rename("temp.txt", "cadastro.txt");

  printf("\nProduto excluido com sucesso.\n");

  printf("\nPressione Enter para voltar ao Menu\n");
  getchar();
}

void saidas() {
  struct Saida {
    char descricao[200];
    float valor;
    int dia, mes, ano;
  };

  int indice = 0;
  int resposta = 1;

  struct Saida saidas[20];

  FILE *arquivo;
  arquivo = fopen("saidas.txt", "a");

  if (arquivo == NULL) {
    printf("Falha na abertura do arquivo\n");
    return;
  }

  do {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    printf("Digite a descricao da saida do caixa!\n");
    char nome[200];
    fgets(nome, sizeof(nome), stdin);
    nome[strlen(nome) - 1] = '\0';

    printf("Digite o valor da saida\n");
    scanf("%f", &saidas[indice].valor);

    time_t t;
    struct tm *data_hora;
    time(&t);
    data_hora = localtime(&t);

    strcpy(saidas[indice].descricao, nome);
    saidas[indice].dia = data_hora->tm_mday;
    saidas[indice].mes = data_hora->tm_mon + 1;
    saidas[indice].ano = data_hora->tm_year + 1900;

    fprintf(arquivo, "%s;%.2f;%d/%d/%d\n", saidas[indice].descricao,
            saidas[indice].valor, saidas[indice].dia, saidas[indice].mes,
            saidas[indice].ano);

    printf("\n\nSaida de valores registrada\n");

    indice++;

    printf("\nDeseja informar outra saida do caixa?\n");
    printf("1-Sim\n0-Nao\n");
    scanf("%d", &resposta);

    while ((c = getchar()) != '\n' && c != EOF)
      ;

  } while (resposta);

  fclose(arquivo);

  printf("\nPressione Enter para voltar ao Menu\n");
}

void relatorioDiario() {
  FILE *arquivoVendas;
  FILE *arquivoSaidas;
  FILE *arquivoDiario;

  float faturamento = 0.0;
  float totalSaidas = 0.0;
  int quantidadeVendas = 0;

  struct Carrinho {
    char produto[101];
    int quantidade;
    int cod;
    float valor;
    int dia, mes, ano;
    float total;
  };

  int dia, mes, ano;

  struct Carrinho carrinho;

  arquivoVendas = fopen("vendas.txt", "r");
  arquivoSaidas = fopen("saidas.txt", "r");
  arquivoDiario = fopen("relatoriodiario.txt", "w");

  if (arquivoVendas == NULL || arquivoSaidas == NULL) {
    printf("Falha na leitura dos arquivos\n");
    return;
  }

  // Solicitar a data desejada
  printf("Digite a data desejada (formato: dd/mm/yyyy):\n");
  scanf("%d/%d/%d", &dia, &mes, &ano);

  // Calcular faturamento e quantidade de vendas
  while (fscanf(arquivoVendas, "%[^;];%d;%d;%f;%d/%d/%d;%f\n", carrinho.produto,
                &carrinho.quantidade, &carrinho.cod, &carrinho.valor,
                &carrinho.dia, &carrinho.mes, &carrinho.ano,
                &carrinho.total) != EOF) {
    if (carrinho.dia == dia && carrinho.mes == mes && carrinho.ano == ano) {
      faturamento += carrinho.total;
      quantidadeVendas++;
      fprintf(arquivoDiario, "%s;%d;%d;%.2f;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.quantidade, carrinho.cod, carrinho.valor, carrinho.dia,
              carrinho.mes, carrinho.ano, carrinho.total);
    }
  }

  // Calcular total em saídas
  while (fscanf(arquivoSaidas, "%[^;];%f;%d/%d/%d\n", carrinho.produto,
                &carrinho.valor, &carrinho.dia, &carrinho.mes,
                &carrinho.ano) != EOF) {
    if (carrinho.dia == dia && carrinho.mes == mes && carrinho.ano == ano) {
      totalSaidas += carrinho.valor;
      fprintf(arquivoDiario, "%s;;;;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.dia, carrinho.mes, carrinho.ano, totalSaidas);
    }
  }

  float lucroReal = faturamento - totalSaidas;

  // Exibir o relatorio
  printf("\nRelatorio Diario para %02d/%02d/%04d:\n\n", dia, mes, ano);
  printf("Faturamento:           ||R$%12.2f\n", faturamento);
  printf("Saidas:                ||R$%12.2f\n", totalSaidas);
  printf("Lucro Real:            ||R$%12.2f\n", lucroReal);
  printf("Quantidade de Vendas:  ||  %8d\n\n", quantidadeVendas);

  // Fechar os arquivos
  fclose(arquivoVendas);
  fclose(arquivoSaidas);
  fclose(arquivoDiario);

  // gravando o relatorio nas pastas destino

  char nomePastaRelatorios[] = "Relatorios";
  char nomePastaRelatoriosDiarios[] = "Diarios";

  char novonome[101];
  sprintf(novonome, "%s/%s/Relatorio Diario %02d-%02d-%04d.txt",
          nomePastaRelatorios, nomePastaRelatoriosDiarios, dia, mes, ano);

  rename("relatoriodiario.txt", novonome);

  printf("Relatorio salvo na pasta Relatorios\n");
}

void relatoriopersonalizado() {
  FILE *arquivoVendas;
  FILE *arquivoSaidas;
  FILE *arquivoPersonalizado;

  float faturamento = 0.0;
  float totalSaidas = 0.0;
  int quantidadeVendas = 0;

  struct Carrinho {
    char produto[101];
    int quantidade;
    int cod;
    float valor;
    int dia, mes, ano;
    float total;
  };

  int diainicial, mesinicial, anoinicial;
  int diafinal, mesfinal, anofinal;

  struct Carrinho carrinho;

  arquivoVendas = fopen("vendas.txt", "r");
  arquivoSaidas = fopen("saidas.txt", "r");
  arquivoPersonalizado = fopen("relatoriopersonalizado.txt", "w");

  if (arquivoVendas == NULL || arquivoSaidas == NULL) {
    printf("Falha na leitura dos arquivos\n");
    return;
  }

  // Solicitar a data desejada
  printf("\nDigite a data inicial (formato: dd/mm/yyyy):\n");
  scanf("%d/%d/%d", &diainicial, &mesinicial, &anoinicial);

  printf("\nDigite a data final (formato: dd/mm/yyyy):\n");
  scanf("%d/%d/%d", &diafinal, &mesfinal, &anofinal);

  // Calcular faturamento e quantidade de vendas
  while (fscanf(arquivoVendas, "%[^;];%d;%d;%f;%d/%d/%d;%f\n", carrinho.produto,
                &carrinho.quantidade, &carrinho.cod, &carrinho.valor,
                &carrinho.dia, &carrinho.mes, &carrinho.ano,
                &carrinho.total) != EOF) {
    if ((carrinho.ano > anoinicial ||
         (carrinho.ano == anoinicial && carrinho.mes > mesinicial) ||
         (carrinho.ano == anoinicial && carrinho.mes == mesinicial &&
          carrinho.dia >= diainicial)) &&
        (carrinho.ano < anofinal ||
         (carrinho.ano == anofinal && carrinho.mes < mesfinal) ||
         (carrinho.ano == anofinal && carrinho.mes == mesfinal &&
          carrinho.dia <= diafinal))) {
      faturamento += carrinho.total;
      quantidadeVendas++;
      fprintf(arquivoPersonalizado, "%s;%d;%d;%.2f;%d/%d/%d;%.2f\n",
              carrinho.produto, carrinho.quantidade, carrinho.cod,
              carrinho.valor, carrinho.dia, carrinho.mes, carrinho.ano,
              carrinho.total);
    }
  }

  // Calcular total em saídas
  while (fscanf(arquivoSaidas, "%[^;];%f;%d/%d/%d\n", carrinho.produto,
                &carrinho.valor, &carrinho.dia, &carrinho.mes,
                &carrinho.ano) != EOF) {
    if ((carrinho.ano > anoinicial ||
         (carrinho.ano == anoinicial && carrinho.mes > mesinicial) ||
         (carrinho.ano == anoinicial && carrinho.mes == mesinicial &&
          carrinho.dia >= diainicial)) &&
        (carrinho.ano < anofinal ||
         (carrinho.ano == anofinal && carrinho.mes < mesfinal) ||
         (carrinho.ano == anofinal && carrinho.mes == mesfinal &&
          carrinho.dia <= diafinal))) {
      totalSaidas += carrinho.valor;
      fprintf(arquivoPersonalizado, "%s;;;;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.dia, carrinho.mes, carrinho.ano, totalSaidas);
    }
  }

  float lucroReal = faturamento - totalSaidas;

  // Exibir o relatório
  printf("\nRelatório Personalizado para %02d/%02d/%04d a %02d/%02d/%04d:\n\n",
         diainicial, mesinicial, anoinicial, diafinal, mesfinal, anofinal);
  printf("Faturamento:           ||R$%12.2f\n", faturamento);
  printf("Saídas:                ||R$%12.2f\n", totalSaidas);
  printf("Lucro Real:            ||R$%12.2f\n", lucroReal);
  printf("Quantidade de Vendas:  ||  %8d\n\n", quantidadeVendas);

  // Fechar os arquivos
  fclose(arquivoVendas);
  fclose(arquivoSaidas);
  fclose(arquivoPersonalizado);

  // gravando o relatorio nas pastas destino
  char nomePastaRelatorios[] = "Relatorios";
  char nomePastaRelatoriosPersonalizados[] = "Personalizados";

  char novonome[256];
  sprintf(novonome, "%s/%s/Relatorio %02d-%02d-%04d_a_%02d-%02d-%04d.txt",
          nomePastaRelatorios, nomePastaRelatoriosPersonalizados, diainicial,
          mesinicial, anoinicial, diafinal, mesfinal, anofinal);

  rename("relatoriopersonalizado.txt", novonome);

  printf("Relatorio salvo na pasta Relatorios\n");
}

void relatorioMesal() {
  FILE *arquivoVendas;
  FILE *arquivoSaidas;
  FILE *arquivoMensal;

  float faturamento = 0.0;
  float totalSaidas = 0.0;
  int quantidadeVendas = 0;

  struct Carrinho {
    char produto[101];
    int quantidade;
    int cod;
    float valor;
    int dia, mes, ano;
    float total;
  };

  int dia, mes, ano;

  struct Carrinho carrinho;

  arquivoVendas = fopen("vendas.txt", "r");
  arquivoSaidas = fopen("saidas.txt", "r");
  arquivoMensal = fopen("relatoriomensal.txt", "w");

  if (arquivoVendas == NULL || arquivoSaidas == NULL) {
    printf("Falha na leitura dos arquivos\n");
    return;
  }

  // Solicitar a data desejada
  printf("Digite mes desejado (formato: mm/yyyy):\n");
  scanf("%d/%d", &mes, &ano);

  dia = 1;

  // Calcular faturamento e quantidade de vendas
  while (fscanf(arquivoVendas, "%[^;];%d;%d;%f;%d/%d/%d;%f\n", carrinho.produto,
                &carrinho.quantidade, &carrinho.cod, &carrinho.valor,
                &carrinho.dia, &carrinho.mes, &carrinho.ano,
                &carrinho.total) != EOF) {
    if (carrinho.dia >= dia && carrinho.mes == mes && carrinho.ano == ano) {
      faturamento += carrinho.total;
      quantidadeVendas++;
      fprintf(arquivoMensal, "%s;%d;%d;%.2f;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.quantidade, carrinho.cod, carrinho.valor, carrinho.dia,
              carrinho.mes, carrinho.ano, carrinho.total);
    }
  }

  // Calcular total em saídas
  while (fscanf(arquivoSaidas, "%[^;];%f;%d/%d/%d\n", carrinho.produto,
                &carrinho.valor, &carrinho.dia, &carrinho.mes,
                &carrinho.ano) != EOF) {
    if (carrinho.dia >= dia && carrinho.mes == mes && carrinho.ano == ano) {
      totalSaidas += carrinho.valor;
      fprintf(arquivoMensal, "%s;;;;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.dia, carrinho.mes, carrinho.ano, totalSaidas);
    }
  }

  float lucroReal = faturamento - totalSaidas;

  // Exibir o relatório
  printf("\nRelatorio Mensal para %02d/%04d:\n\n", mes, ano);
  printf("Faturamento:           ||R$%12.2f\n", faturamento);
  printf("Saidas:                ||R$%12.2f\n", totalSaidas);
  printf("Lucro Real:            ||R$%12.2f\n", lucroReal);
  printf("Quantidade de Vendas:  ||  %8d\n\n", quantidadeVendas);

  // Fechar os arquivos
  fclose(arquivoVendas);
  fclose(arquivoSaidas);
  fclose(arquivoMensal);

  // gravando o relatorio nas pastas destino

  char nomePastaRelatorios[] = "Relatorios";
  char nomePastaRelatoriosMensais[] = "Mensais";

  char novonome[101];
  sprintf(novonome, "%s/%s/Relatorio Mensal %02d-%04d.txt", nomePastaRelatorios,
          nomePastaRelatoriosMensais, mes, ano);

  rename("relatoriomensal.txt", novonome);

  printf("Relatorio salvo na pasta Relatorios\n");
}

void relatorioAnual() {
  FILE *arquivoVendas;
  FILE *arquivoSaidas;
  FILE *arquivoAnual;

  float faturamento = 0.0;
  float totalSaidas = 0.0;
  int quantidadeVendas = 0;

  struct Carrinho {
    char produto[101];
    int quantidade;
    int cod;
    float valor;
    int dia, mes, ano;
    float total;
  };

  int dia, mes, ano;

  struct Carrinho carrinho;

  arquivoVendas = fopen("vendas.txt", "r");
  arquivoSaidas = fopen("saidas.txt", "r");
  arquivoAnual = fopen("relatorioAnual.txt", "w");

  if (arquivoVendas == NULL || arquivoSaidas == NULL) {
    printf("Falha na leitura dos arquivos\n");
    return;
  }

  // Solicitar a data desejada
  printf("Digite ano desejado (formato:yyyy):\n");
  scanf("%d", &ano);

  dia = 1;
  mes = 1;

  // Calcular faturamento e quantidade de vendas
  while (fscanf(arquivoVendas, "%[^;];%d;%d;%f;%d/%d/%d;%f\n", carrinho.produto,
                &carrinho.quantidade, &carrinho.cod, &carrinho.valor,
                &carrinho.dia, &carrinho.mes, &carrinho.ano,
                &carrinho.total) != EOF) {
    if (carrinho.dia >= dia && carrinho.mes >= mes && carrinho.ano == ano) {
      faturamento += carrinho.total;
      quantidadeVendas++;
      fprintf(arquivoAnual, "%s;%d;%d;%.2f;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.quantidade, carrinho.cod, carrinho.valor, carrinho.dia,
              carrinho.mes, carrinho.ano, carrinho.total);
    }
  }

  // Calcular total em saidas
  while (fscanf(arquivoSaidas, "%[^;];%f;%d/%d/%d\n", carrinho.produto,
                &carrinho.valor, &carrinho.dia, &carrinho.mes,
                &carrinho.ano) != EOF) {
    if (carrinho.dia >= dia && carrinho.mes >= mes && carrinho.ano == ano) {
      totalSaidas += carrinho.valor;
      fprintf(arquivoAnual, "%s;;;;%d/%d/%d;%.2f\n", carrinho.produto,
              carrinho.dia, carrinho.mes, carrinho.ano, totalSaidas);
    }
  }

  float lucroReal = faturamento - totalSaidas;

  // Exibir o relatorio
  printf("\nRelatorio Anual para %04d:\n\n", ano);
  printf("Faturamento:           ||R$%12.2f\n", faturamento);
  printf("Saidas:                ||R$%12.2f\n", totalSaidas);
  printf("Lucro Real:            ||R$%12.2f\n", lucroReal);
  printf("Quantidade de Vendas:  ||  %8d\n\n", quantidadeVendas);

  // Fechar os arquivos
  fclose(arquivoVendas);
  fclose(arquivoSaidas);
  fclose(arquivoAnual);

  // Gravando o relatorio nas pastas destino

  char nomePastaRelatorios[] = "Relatorios";
  char nomePastaRelatoriosAnuais[] = "Anuais";

  char novonome[101];
  sprintf(novonome, "%s/%s/Relatorio Anual %04d.txt", nomePastaRelatorios,
          nomePastaRelatoriosAnuais, ano);

  rename("relatorioAnual.txt", novonome);

  printf("Relatorio salvo na pasta Relatorios\n");
}

void relatorioFinanceiro() {

  printf("\nQual o relatorio desejado?\n");
  printf("1-Diario\n");
  printf("2-Personalizado/Semanal\n");
  printf("3-Mensal\n");
  printf("4-Anual\n\n");

  int opcao = 0;

  scanf("%d", &opcao);

  switch (opcao) {
  case 1:
    relatorioDiario();
    break;
  case 2:
    relatoriopersonalizado();
    break;
  case 3:
    relatorioMesal();
    break;
  case 4:
    relatorioAnual();
    break;
  default:
    printf("Opcao invalida.\n");
  }

  printf("\nPressione Enter para voltar ao Menu\n");
  getchar();
}
