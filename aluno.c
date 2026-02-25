#include <stdio.h>
#include <string.h>
#include "aluno.h"

/* ------------------------------------------------------------------ */
/* Utilitários internos                                                 */
/* ------------------------------------------------------------------ */

static void limpar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int encontrar_por_matricula(const Turma *t, int mat)
{
    for (int i = 0; i < t->total; i++)
        if (t->alunos[i].matricula == mat)
            return i;
    return -1;
}


//Média
float Aluno_media(const Aluno *a)
{
    return (a->n1 + a->n2 + a->n3 + a->n4) / 4.0f;
}

//Média
int Turma_carregar(Turma *t)
{
    FILE *f = fopen(ARQ, "rb");
    if (!f) {
        t->total = 0;
        return 1;   /* arquivo ainda não existe — tudo bem */
    }

    int total_lido = 0;
    if (fread(&total_lido, sizeof(int), 1, f) != 1 ||
        total_lido < 0 || total_lido > MAX) {
        fclose(f);
        return 0;   /* arquivo corrompido */
    }

    if (total_lido > 0 &&
        fread(t->alunos, sizeof(Aluno), (size_t)total_lido, f) != (size_t)total_lido) {
        fclose(f);
        return 0;
    }

    fclose(f);
    t->total = total_lido;
    return 1;
}

int Turma_salvar(const Turma *t)
{
    FILE *f = fopen(ARQ, "wb");
    if (!f) return 0;

    if (fwrite(&t->total, sizeof(int), 1, f) != 1) {
        fclose(f);
        return 0;
    }

    if (t->total > 0 &&
        fwrite(t->alunos, sizeof(Aluno), (size_t)t->total, f) != (size_t)t->total) {
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}

//Operações CRUD
void Turma_adicionar(Turma *t)
{
    if (t->total >= MAX) {
        printf("Limite de alunos atingido.\n");
        return;
    }

    Aluno a;

    printf("Matricula (int): ");
    if (scanf("%d", &a.matricula) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    if (encontrar_por_matricula(t, a.matricula) != -1) {
        printf("Ja existe aluno com essa matricula.\n");
        return;
    }

    printf("Nome do aluno: ");
    if (fgets(a.nome, sizeof(a.nome), stdin) == NULL) {
        printf("Entrada invalida.\n");
        return;
    }
    a.nome[strcspn(a.nome, "\n")] = '\0';

    printf("Nota BIMESTRE 1: ");
    if (scanf("%f", &a.n1) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    printf("Nota BIMESTRE 2: ");
    if (scanf("%f", &a.n2) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    printf("Nota BIMESTRE 3: ");
    if (scanf("%f", &a.n3) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    printf("Nota BIMESTRE 4: ");
    if (scanf("%f", &a.n4) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    limpar_buffer();

    t->alunos[t->total] = a;
    t->total++;

    printf("Aluno adicionado!\n");
}

void Turma_remover(Turma *t)
{
    if (t->total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    int mat;
    printf("Matricula do aluno a remover: ");
    if (scanf("%d", &mat) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    int idx = encontrar_por_matricula(t, mat);
    if (idx == -1) {
        printf("Aluno nao encontrado.\n");
        return;
    }

    /* Puxa os itens da direita uma posição para a esquerda */
    for (int i = idx; i < t->total - 1; i++)
        t->alunos[i] = t->alunos[i + 1];

    t->total--;
    printf("Aluno removido!\n");
}

void Turma_listar(const Turma *t)
{
    if (t->total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\n=== LISTA ===\n");
    printf("%-6s | %-20s | %5s | %5s | %5s | %5s | %5s\n",
           "MAT", "NOME", "N1", "N2", "N3", "N4", "MED");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < t->total; i++) {
        const Aluno *a = &t->alunos[i];
        printf("%-6d | %-20s | %5.1f | %5.1f | %5.1f | %5.1f | %5.1f\n",
               a->matricula, a->nome,
               a->n1, a->n2, a->n3, a->n4,
               Aluno_media(a));
    }
}

void Turma_editar(Turma *t)
{
    if (t->total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    int mat;
    printf("Matricula do aluno a editar: ");
    if (scanf("%d", &mat) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    int idx = encontrar_por_matricula(t, mat);
    if (idx == -1) {
        printf("Aluno nao encontrado.\n");
        return;
    }

    Aluno *a = &t->alunos[idx];
    printf("Editando %s (mat %d)\n", a->nome, a->matricula);

    printf("Nota BIMESTRE 1 atual: %.1f | Nova: ", a->n1);
    if (scanf("%f", &a->n1) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    printf("Nota BIMESTRE 2 atual: %.1f | Nova: ", a->n2);
    if (scanf("%f", &a->n2) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    printf("Nota BIMESTRE 3 atual: %.1f | Nova: ", a->n3);
    if (scanf("%f", &a->n3) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    printf("Nota BIMESTRE 4 atual: %.1f | Nova: ", a->n4);
    if (scanf("%f", &a->n4) != 1) { printf("Entrada invalida.\n"); limpar_buffer(); return; }

    limpar_buffer();
    printf("Notas atualizadas!\n");
}