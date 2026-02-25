#include <stdio.h>
#include <stdlib.h>
#include "aluno.h"

static void limpar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static void limpar_tela(void)
{
    system("clear");
}

static void menu(void)
{
    printf("\n=== MENU ===\n");
    printf("1 - Adicionar aluno\n");
    printf("2 - Listar alunos\n");
    printf("3 - Editar notas\n");
    printf("4 - Remover aluno\n");
    printf("5 - Salvar agora\n");
    printf("0 - Salvar e sair\n");
    printf("Escolha: ");
}

int main(void)
{
    Turma turma;

    if (!Turma_carregar(&turma)) {
        printf("Erro ao carregar %s (arquivo corrompido ou formato invalido).\n", ARQ);
        return 1;
    }

    int opcao;
    while (1) {
        menu();

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        if      (opcao == 1) { limpar_tela(); Turma_adicionar(&turma); }
        else if (opcao == 2) { limpar_tela(); Turma_listar(&turma);    }
        else if (opcao == 3) { limpar_tela(); Turma_editar(&turma);    }
        else if (opcao == 4) { limpar_tela(); Turma_remover(&turma);   }
        else if (opcao == 5) {
            if (Turma_salvar(&turma))
                printf("Salvo em %s.\n", ARQ);
            else
                printf("Erro ao salvar.\n");
        }
        else if (opcao == 0) {
            if (!Turma_salvar(&turma))
                printf("Erro ao salvar.\n");
            else
                printf("Salvo em %s. Tchau!\n", ARQ);
            break;
        }
        else {
            printf("Opcao invalida.\n");
        }
    }

    return 0;
}