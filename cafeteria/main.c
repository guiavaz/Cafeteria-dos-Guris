#include<stdio.h>
#include<stdlib.h>
#include <locale.h>
#include "cafeteria.h"

int main ()
{
    setlocale(LC_ALL, "Portuguese");

    int option=0;
    printf("-------Cafeteria dos Guris-------\n");

    //Menu
    while(option != 99)
    {
        printf("\nSelecione uma opção:\n\n");
        printf("1- Cadastrar Produto\n");
        printf("2- Consultar Produto\n");
        printf("3- Excluir Produto\n");
        printf("4- Orçamento\n");
        printf("99- Sair\n\n");

        scanf("%i", &option);


        switch(option)
        {

            case 1:
                printf("Função não implementada!\n");
            break;

            case 2:
                printf("Função não implementada!\n");
            break;

            case 3:
                printf("Função não implementada!\n");
            break;

            case 4:
                printf("Função não implementada!\n");
            break;

            case 99:
                printf("\nTem certeza que deseja sair?\n");
                printf("1- Sim\n2- Não\n\n");
                scanf("%i", &option);
                if(option == 1)
                {
                    option=99;
                    printf("Sistema encerrado!\n\n");
                }
                else
                {
                    getchar();
                    break;
                }

            break;

            default:
                printf("Entrada inválida!\n\n");
            break;
        }
    }
}
