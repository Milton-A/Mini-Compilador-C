#include "../AnalisadorLexico/analisadorLexico.c"
//#include "../AnalisadorSintatico/analisadorSintatico.c"

int main()
{
	ficheiro = fopen("arquivo.txt", "rb");
	Tipo t;
	do{
		t = analex(ficheiro);
		printf("%s %d \n", t.lexema, t.linha);
	}while(t.token != TK_END);
}
