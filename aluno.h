#ifndef ALUNO_H
#define ALUNO_H

#define MAX  2500
#define ARQ  "alunos.dat"

//Registro de um único aluno
typedef struct {
    int   matricula;
    char  nome[50];
    float n1, n2, n3, n4;
} Aluno;

/* Coleção de alunos */
typedef struct {
    Aluno alunos[MAX];
    int   total;
} Turma;

//Calcula e retorna a média das quatro notas
float Aluno_media(const Aluno *a);

//Operações sobre a turma
void Turma_adicionar(Turma *t);
void Turma_remover  (Turma *t);
void Turma_listar   (const Turma *t);
void Turma_editar   (Turma *t);
int  Turma_salvar   (const Turma *t);
int  Turma_carregar (Turma *t);

#endif