#define MAX 1000

FILE *ficheiro;
char vetor[MAX];
typedef struct TabelaSim TabelaSim;
typedef struct Coment Coment;
typedef struct ErroLexico ErroLexico;
typedef struct Tipo Tipo;
typedef struct Lista Lista;
char lerCaractere();
void VoltarCaractere();
void zerarVetor();
void analisadorLexico();
