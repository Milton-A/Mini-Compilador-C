#include "../AnalisadorSintatico/analisadorSintatico.c"

int main()
{
    ficheiro = fopen("ficheiro.txt", "rb");
    if (ficheiro == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    program(ficheiro);
    fclose(ficheiro);
    return 1;
}
