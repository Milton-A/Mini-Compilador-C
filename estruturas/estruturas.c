#include "headerCompile.h"
#include "token.h"
#define MAX 10000

FILE *ficheiro;
char vetor[MAX];

typedef struct Tipo
{
	char *token;
	char *lexema;
   	int linha;
    char *valorAtribuido;
    int *enderecoMemoria;
    char *escopoVariavel;
    char *tipoVariavel; //funcao
    size_t tamanhoByte;
}Tipo;
typedef struct ListaCodigo
{
	Tipo dadoToken;
	struct ListaCodigo* next;
}ListaCodigo;