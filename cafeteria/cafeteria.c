#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cafeteria.h"

int main() {
  setlocale(LC_ALL, "Portuguese");

  printf("-------Cafeteria dos Guris-------\n");

  int option = 0;

  do {

    printf("\nSelecione uma opcao:\n\n");
    printf("1- Realizar Venda\n");
    printf("2- Consultar Produto\n");
    printf("3- Cadastrar Produto\n");
    printf("4- Excluir Produto\n");
    printf("5- Saidas\n");
    printf("6- Relatorio financeiro\n");
    printf("99- Sair\n\n");

    scanf("%i", &option);

    switch (option) {

    case 1:
      realizarVenda();
      getchar();
      break;

    case 2:
      consultaProduto();
      getchar();
      break;

    case 3:
      cadastraProduto();
      getchar();
      break;

    case 4:
      excluiProduto();
      getchar();
      break;

    case 5:
      saidas();
      getchar();
      break;

    case 6:
      relatorioFinanceiro();
      getchar();
      break;

    case 99:
      printf("\nPrograma Encerrado!\n");
      break;

    default:
      printf("Opcao invalida. Tente novamente.\n");
    }
  } while (option != 99);

  return 0;
}
