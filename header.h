#define MAX 1000

FILE *ficheiro;
char vetor[MAX];

typedef struct Tipo Tipo;
Tipo analex(FILE *ficheiro);
char lerCaractere();
void VoltarCaractere();
void zerarVetor();