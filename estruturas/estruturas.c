#include "header.h"

typedef struct Tipo
{
	char *token;
	char *lexema;
   	int linha;
    char *tipoDeDado;
    char *valorAtribuido;
	int posicaoToken;
	int tamanhoLexema;
    int endereçoMemoria;
    char *escopo;
    int tamanhoByte;
}Tipo;