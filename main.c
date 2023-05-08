#include "estados.c"

char lerCaractere()
{
	char caractere;
	fread(&caractere, 1, 1, ficheiro);
	return caractere;
};
void VoltarCaractere()
{
	fseek(ficheiro, -1, SEEK_CUR); // posiciona o cursor uma posi??o anterior
};

void zerarVetor()
{
	int conte;
	for (conte = 0; conte < MAX; conte++)
	{
		vetor[conte] = '\0';
	}
	cont = 0;
}

int main()
{
	ficheiro = fopen("arquivo.txt", "rb");
	Tipo t;
	printf("LEXAMA\t\t\t\t\tTOKEN\t\t\t\t LINHA\n");
	do
	{
		t = analex(ficheiro);
		printf("%s\t\t\t\t\t %s\t\t\t\t%d\n", t.lexema, t.token, t.linha);
		// zerarVetor();
	} while (t.token != TK_END);
	return 0;
}
