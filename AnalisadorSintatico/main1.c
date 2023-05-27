#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_IDENTIFICADOR 1000


// Protótipos das funções
void programa(void);
void declaracoes(void);
void declaracao(void);
void tipo(void);
void lista_declaracoes(void);
void comandos(void);
void comando(void);
void comando_if_else(void);
void comando_while(void);
void comando_for(void);
void comando_atribuicao(void);
void comando_chamada_funcao(void);
void argumentos(void);
void expressao(void);
void expressao_simples(void);
void termo(void);
void fator(void);
void chamada_funcao(void);
void constante(void);
void constante_numerica(void);
void constante_caracter(void);
void constante_cadeia(void);
void caracteres(void);
void identificador(void);
void letra(void);
void digito(void);
void op_relacional(void);
void op_soma(void);
void op_multiplicacao(void);
void atribuicao(void);

// Variáveis globais
char *entrada; // Ponteiro para a entrada
char *posicao_atual; // Ponteiro para a posição atual na entrada

int main(void) {
    entrada = "(int a, float b, char c) { int i; for(i = 0; i < 10; i++) { if(a > 0) { b = a * c; } else { b = 0; } } }";
    posicao_atual = entrada;
    programa();
    printf("Entrada válida!\n");
    return 0;
}

void programa(void) {
    declaracoes();
    comandos();
}

void declaracoes(void) {
    declaracao();
    while (*posicao_atual != '}') {
        declaracao();
    }
}

void declaracao(void) {
    tipo();
    lista_declaracoes();
    posicao_atual++;
}

void tipo(void) {
    if (strncmp(posicao_atual, "int", 3) == 0) {
        posicao_atual += 3;
    } else if (strncmp(posicao_atual, "float", 5) == 0) {
        posicao_atual += 5;
    } else if (strncmp(posicao_atual, "char", 4) == 0) {
        posicao_atual += 4;
    } else if (strncmp(posicao_atual, "double", 6) == 0) {
        posicao_atual += 6;
    } else if (strncmp(posicao_atual, "void", 4) == 0) {
        posicao_atual += 4;
    } else {
        printf("Erro: tipo inválido na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void lista_declaracoes(void) {
    identificador();
    while (*posicao_atual == ',') {
        posicao_atual++;
        identificador();
    }
}

void comandos(void) {
    posicao_atual++; // Ignora o '{'
    while (*posicao_atual != '}') {
        comando();
    }
}

void comando(void) {
    if (strncmp(posicao_atual, "if", 2) == 0) {
        comando_if_else();
    } else if (strncmp(posicao_atual, "while", 5) == 0) {
        comando_while();
    } else if (strncmp(posicao_atual, "for", 3) == 0) {
        comando_for();
    } else if (isalpha(*posicao_atual)) {
        identificador();
        if (*posicao_atual == '(') {
            comando_chamada_funcao();
        } else {
            atribuicao();
        }
    } else {
        printf("Erro: comando inválido na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void comando_if_else(void) {
    if (strncmp(posicao_atual, "if", 2) == 0) {
        posicao_atual += 2;
        if (*posicao_atual == '(') {
            posicao_atual++;
            expressao();
            if (*posicao_atual == ')') {
                posicao_atual++;
                comando();
                if (strncmp(posicao_atual, "else", 4) == 0) {
                    posicao_atual += 4;
                    comando();
                }
            } else {
                printf("Erro: esperava ')' na posição %ld\n", posicao_atual - entrada);
                exit(1);
            }
        } else {
            printf("Erro: esperava '(' na posição %ld\n", posicao_atual - entrada);
            exit(1);
        }
    } else {
        printf("Erro: esperava 'if' na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void comando_while(void) {
    if (strncmp(posicao_atual, "while", 5) == 0) {
        posicao_atual += 5;
        if (*posicao_atual == '(') {
            posicao_atual++;
            expressao();
            if (*posicao_atual == ')') {
                posicao_atual++;
                comando();
            } else {
                printf("Erro: esperava ')' na posição %ld\n", posicao_atual - entrada);
                exit(1);
            }
        } else {
            printf("Erro: esperava '(' na posição %ld\n", posicao_atual - entrada);
            exit(1);
        }
    } else {
        printf("Erro: esperava 'while' na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void comando_for(void) {
    if (strncmp(posicao_atual, "for", 3) == 0) {
        posicao_atual += 3;
        if (*posicao_atual == '(') {
            posicao_atual++;
            expressao_opt();
            if (*posicao_atual == ';') {
                posicao_atual++;
                expressao_opt();
                if (*posicao_atual == ';') {
                    posicao_atual++;
                    expressao_opt();
                    if (*posicao_atual == ')') {
                        posicao_atual++;
                        comando();
                    } else {
                        printf("Erro: esperava ')' na posição %ld\n", posicao_atual - entrada);
                        exit(1);
                    }
                } else {
                    printf("Erro: esperava ';' na posição %ld\n", posicao_atual - entrada);
                    exit(1);
                }
            } else {
                printf("Erro: esperava ';' na posição %ld\n", posicao_atual - entrada);
                exit(1);
            }
        } else {
            printf("Erro: esperava '(' na posição %ld\n", posicao_atual - entrada);
            exit(1);
        }
    } else {
        printf("Erro: esperava 'for' na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void comando_atribuicao(void) {
    char identificador[MAX_IDENTIFICADOR];
    int valor;

    if (isalpha(*posicao_atual)) {
        int i = 0;
        while (isalnum(*posicao_atual)) {
            identificador[i++] = *posicao_atual++;
        }
        identificador[i] = '\0';
        if (*posicao_atual == '=') {
            posicao_atual++;
            valor = expressao();
            if (*posicao_atual == ';') {
                posicao_atual++;
                // realizar a atribuição do valor para o identificador
                printf("%s = %d\n", identificador, valor);
            } else {
                printf("Erro: esperava ';' na posição %ld\n", posicao_atual - entrada);
                exit(1);
            }
        } else {
            printf("Erro: esperava '=' na posição %ld\n", posicao_atual - entrada);
            exit(1);
        }
    } else {
        printf("Erro: esperava identificador na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void comando_chamada_funcao(void) {
    char identificador[MAX_IDENTIFICADOR];
    int argumento;

    if (isalpha(*posicao_atual)) {
        int i = 0;
        while (isalnum(*posicao_atual)) {
            identificador[i++] = *posicao_atual++;
        }
        identificador[i] = '\0';
        if (*posicao_atual == '(') {
            posicao_atual++;
            argumento = expressao();
            if (*posicao_atual == ')') {
                posicao_atual++;
                // realizar a chamada da função com o argumento
                printf("Chamada da função %s com argumento %d\n", identificador, argumento);
            } else {
                printf("Erro: esperava ')' na posição %ld\n", posicao_atual - entrada);
                exit(1);
            }
        } else {
            printf("Erro: esperava '(' na posição %ld\n", posicao_atual - entrada);
            exit(1);
        }
    } else {
        printf("Erro: esperava identificador na posição %ld\n", posicao_atual - entrada);
        exit(1);
    }
}

void argumentos(void) {
    expressao();
    while (*posicao_atual == ',') {
        posicao_atual++;
        expressao();
    }
}

void expressao(void) {
    int resultado = termo();

    while (*posicao_atual == '+' || *posicao_atual == '-') {
        char operador = *posicao_atual++;
        int proximo_termo = termo();
        if (operador == '+') {
            resultado += proximo_termo;
        } else {
            resultado -= proximo_termo;
        }
    }

    printf("Resultado da expressão: %d\n", resultado);
}

void expressao_simples(void) {
    termo();
    while (*posicao_atual == '+' || *posicao_atual == '-') {
        posicao_atual++;
        termo();
    }
}

void termo(void) {
    int resultado //= fator();

    while (*posicao_atual == '*' || *posicao_atual == '/') {
        char operador = *posicao_atual++;
        int proximo_fator = fator();
        if (operador == '*') {
            resultado *= proximo_fator;
        } else {
            resultado /= proximo_fator;
        }
    }

    printf("Resultado do termo: %d\n", resultado);
}

void fator(void) {
    if (*posicao_atual == '(') {
        // se o próximo caractere for '(', significa que temos uma expressão dentro de parênteses
        posicao_atual++;
        expressao();
        if (*posicao_atual != ')') {
            printf("Erro de sintaxe: esperado ')' em vez de '%c'\n", *posicao_atual);
            exit(1);
        }
        posicao_atual++;
    } else if (isdigit(*posicao_atual)) {
        // se o próximo caractere for um dígito, significa que temos um número
        int numero = 0;
        while (isdigit(*posicao_atual)) {
            numero = 10 * numero + (*posicao_atual - '0');
            posicao_atual++;
        }
        printf("Número: %d\n", numero);
    } else if (isalpha(*posicao_atual)) {
        // se o próximo caractere for uma letra, significa que temos um identificador
        char identificador[MAX_IDENTIFICADOR];
        int i = 0;
        while (isalnum(*posicao_atual)) {
            identificador[i++] = *posicao_atual++;
        }
        identificador[i] = '\0';
        if (*posicao_atual == '(') {
            // se o próximo caractere for '(', significa que temos uma chamada de função
            posicao_atual++;
            argumentos();
            if (*posicao_atual != ')') {
                printf("Erro de sintaxe: esperado ')' em vez de '%c'\n", *posicao_atual);
                exit(1);
            }
            posicao_atual++;
            printf("Chamada de função: %s\n", identificador);
        } else {
            // senão, temos apenas uma variável
            printf("Variável: %s\n", identificador);
        }
    } else {
        // se chegamos aqui, significa que houve um erro de sintaxe
        printf("Erro de sintaxe: esperado fator em vez de '%c'\n", *posicao_atual);
        exit(1);
    }
}

void constante(void) {
    int tem_ponto = 0;
    while (isdigit(*posicao_atual) || *posicao_atual == '.') {
        if (*posicao_atual == '.') {
            if (tem_ponto) {
                printf("Erro de sintaxe: mais de um ponto encontrado em constante\n");
                exit(1);
            }
            tem_ponto = 1;
        }
        posicao_atual++;
    }
    printf("Constante: ");
    if (tem_ponto) {
        printf("float\n");
    } else {
        printf("int\n");
    }
}

void constante_numerica(void) {
    if (*posicao_atual == '\'') { // é uma constante char
        posicao_atual++;
        if (*posicao_atual == '\\') {
            posicao_atual++;
            if (*posicao_atual == 'n' || *posicao_atual == 't' || *posicao_atual == '\\') {
                posicao_atual++;
            } else {
                printf("Erro de sintaxe: caractere de escape inválido\n");
                exit(1);
            }
        } else if (*posicao_atual == '\'') {
            printf("Erro de sintaxe: caractere vazio\n");
            exit(1);
        }
        if (*posicao_atual == '\'') {
            printf("Constante char\n");
            posicao_atual++;
            return;
        } else {
            printf("Erro de sintaxe: caractere não fechado\n");
            exit(1);
        }
    } else { // é um número inteiro ou um número de ponto flutuante
        int tem_ponto = 0;
        while (isdigit(*posicao_atual) || *posicao_atual == '.') {
            if (*posicao_atual == '.') {
                if (tem_ponto) {
                    printf("Erro de sintaxe: mais de um ponto encontrado em constante numérica\n");
                    exit(1);
                }
                tem_ponto = 1;
            }
            posicao_atual++;
        }
        if (*posicao_atual == 'f') { // é um número de ponto flutuante seguido de f
            printf("Constante numérica: float\n");
            posicao_atual++;
        } else if (*posicao_atual == 'u' || *posicao_atual == 'U' || *posicao_atual == 'l' || *posicao_atual == 'L') {
            printf("Erro de sintaxe: sufixo inválido em constante numérica\n");
            exit(1);
        } else { // é um número inteiro ou um número de ponto flutuante sem sufixo
            printf("Constante numérica: ");
            if (tem_ponto) {
                printf("float\n");
            } else {
                printf("int\n");
            }
        }
    }
}

void constante_caracter(void) {
    if (*posicao_atual == '\'') {
        posicao_atual++;
        if (*posicao_atual == '\\') {
            posicao_atual++;
            if (*posicao_atual == 'n' || *posicao_atual == 't' || *posicao_atual == '\\') {
                posicao_atual++;
            } else {
                printf("Erro de sintaxe: caractere de escape inválido\n");
                exit(1);
            }
        } else if (*posicao_atual == '\'') {
            printf("Erro de sintaxe: caractere vazio\n");
            exit(1);
        }
        if (*posicao_atual == '\'') {
            printf("Constante caractere\n");
            posicao_atual++;
            return;
        } else {
            printf("Erro de sintaxe: caractere não fechado\n");
            exit(1);
        }
    }
}

void caracteres(void) {
    while ((*posicao_atual >= 32 && *posicao_atual <= 126) || *posicao_atual == 9 || *posicao_atual == 10 || *posicao_atual == 13) {
        posicao_atual++;
    }
}

void identificador(void) {
    if (isalpha(*posicao_atual) || *posicao_atual == '_') {
        posicao_atual++;
        while (isalnum(*posicao_atual) || *posicao_atual == '_') {
            posicao_atual++;
        }
        printf("Identificador\n");
    } else {
        printf("Erro de sintaxe: identificador inválido\n");
        exit(1);
    }
}

void letra(void) {
    if (isalpha(*posicao_atual)) {
        posicao_atual++;
    } else {
        printf("Erro de sintaxe: esperado uma letra\n");
        exit(1);
    }
}

void digito(void) {
    if (isdigit(*posicao_atual)) {
        posicao_atual++;
    } else {
        printf("Erro de sintaxe: esperado um dígito\n");
        exit(1);
    }
}

void op_relacional(void) {
    if (*posicao_atual == '=') {
        posicao_atual++;
        if (*posicao_atual == '=') {
            posicao_atual++;
            printf("Operador relacional: ==\n");
            return;
        }
    } else if (*posicao_atual == '!') {
        posicao_atual++;
        if (*posicao_atual == '=') {
            posicao_atual++;
            printf("Operador relacional: !=\n");
            return;
        }
    } else if (*posicao_atual == '<') {
        posicao_atual++;
        if (*posicao_atual == '=') {
            posicao_atual++;
            printf("Operador relacional: <=\n");
            return;
        } else {
            printf("Operador relacional: <\n");
            return;
        }
    } else if (*posicao_atual == '>') {
        posicao_atual++;
        if (*posicao_atual == '=') {
            posicao_atual++;
            printf("Operador relacional: >=\n");
            return;
        } else {
            printf("Operador relacional: >\n");
            return;
        }
    }
    printf("Erro de sintaxe: esperado um operador relacional\n");
    exit(1);
}

