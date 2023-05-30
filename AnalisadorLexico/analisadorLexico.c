#include "../estruturas/estruturas.c"

int cont = 0;
int contLinha = 1;

Tipo k, t;

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

void adicionarElemento(ListaCodigo **lista, Tipo dadoToken)
{
	// Criar um novo nó
	ListaCodigo *newNode = (ListaCodigo *)malloc(sizeof(ListaCodigo));

	// Atribuir os valores do dadoToken ao novo nó
	newNode->dadoToken.token = strdup(dadoToken.token);
	newNode->dadoToken.lexema = strdup(dadoToken.lexema);
	newNode->dadoToken.linha = dadoToken.linha;
	newNode->dadoToken.valorAtribuido = strdup(dadoToken.valorAtribuido);
	newNode->dadoToken.enderecoMemoria = dadoToken.enderecoMemoria;
	newNode->dadoToken.escopoVariavel = strdup(dadoToken.escopoVariavel);
	newNode->dadoToken.tipoVariavel = strdup(dadoToken.tipoVariavel);
	newNode->dadoToken.tamanhoByte = dadoToken.tamanhoByte;
	newNode->next = NULL;

	// Se a lista estiver vazia, o novo nó será o primeiro nó (cabeça)
	if (*lista == NULL)
	{
		*lista = newNode;
	}
	// Caso contrário, percorrer a lista até o último nó e inserir o novo nó no final
	else
	{
		ListaCodigo *atual = *lista;
		while (atual->next != NULL)
		{
			atual = atual->next;
		}
		atual->next = newNode;
	}
}

ListaCodigo *listaAnalisadorSinatico(FILE *ficheiro)
{
	ListaCodigo *lista = NULL;
	Tipo t;
	do
	{
		t = analex(ficheiro);
		adicionarElemento(&lista, t);
	} while (t.token != TK_END);
	return lista;
}

Tipo analex(FILE *ficheiro)
{

	int estado = 0;
	char caractere;
	zerarVetor();

	while (!feof(ficheiro))
	{
		k.linha = contLinha;
		if (caractere == '\n')
		{
			contLinha++;
		}
		switch (estado)
		{
		case 0:
			caractere = lerCaractere();
			if (caractere == '#')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 84;
			}
			else if ((caractere >= 'A' && caractere <= 'Z') || (caractere >= 'a' && caractere <= 'z') || caractere == '_')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 1;
			}
			else if (caractere >= '0' && caractere <= '9')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 3;
			}
			else if (caractere == ';')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 77;
			}
			else if (caractere == '=')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 45;
					VoltarCaractere();
				}
				else
				{
					estado = 31;
				}
			}
			else if (caractere == '+')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '+')
				{
					estado = 22;
					VoltarCaractere();
				}
				else if (prox == '=')
				{
					estado = 27;
					VoltarCaractere();
				}
				else
				{
					estado = 18;
				}
			}
			else if (caractere == '-')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '-')
				{
					estado = 24;
					VoltarCaractere();
				}
				else if (prox == '=')
				{
					estado = 29;
					VoltarCaractere();
				}
				else
				{
					estado = 19;
				}
			}
			else if (caractere == '*')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 34;
					VoltarCaractere();
				}
				else
					estado = 26;
			}
			else if (caractere == '/')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '/')
				{
					estado = 12;
					VoltarCaractere();
				}
				else if (prox == '*')
				{
					estado = 15;
					VoltarCaractere();
				}
				else if (prox == '=')
				{
					estado = 36;
					VoltarCaractere();
				}
				else
				{
					estado = 20;
				}
			}
			else if (caractere == '%')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 36;
					VoltarCaractere();
				}
				else
				{
					estado = 21;
				}
			}
			else if (caractere == '<')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 53;
					VoltarCaractere();
				}
				else if (prox == '<')
				{
					estado = 63;
					VoltarCaractere();
				}
				else
				{
					estado = 49;
				}
			}
			else if (caractere == '>')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 50;
					VoltarCaractere();
				}
				else if (prox == '>')
				{
					estado = 68;
					VoltarCaractere();
				}
				else
				{
					estado = 54;
				}
			}
			else if (caractere == '(')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 75;
			}
			else if (caractere == ')')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 76;
			}
			else if (caractere == '{')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 73;
			}
			else if (caractere == '}')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 74;
			}
			else if (caractere == ',')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 78;
			}
			else if (caractere == '!')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 59;
			}
			else if (caractere == '?')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 71;
			}
			else if (caractere == '&')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();

				if (prox == '&')
				{
					estado = 55;
					VoltarCaractere();
				}
				else
				{
					estado = 60;
				}
			}
			else if (caractere == '~')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 66;
			}
			else if (caractere == '|')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '|')
				{
					estado = 57;
					VoltarCaractere();
				}
				else
				{
					estado = 62;
				}
			}
			else if (caractere == '^')
			{
				vetor[cont] = caractere;
				cont++;
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 40;
					VoltarCaractere();
				}
				else
				{
					estado = 61;
				}
			}
			else if (caractere == '.')
			{
			}
			else if (caractere == '"')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 80;
			}
			break;

		case 1:
			caractere = lerCaractere();
			while ((caractere >= 'A' && caractere <= 'Z') || (caractere >= 'a' && caractere <= 'z') || caractere == '_' || (caractere >= '0' && caractere <= '9'))
			{
				vetor[cont] = caractere;
				cont++;
				caractere = lerCaractere();
			}
			estado = 2;
			break;
		case 2:

			k.lexema = vetor;
			char *palavras_reservadas[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
			for (int i = 0; i < 32; i++)
			{
				if (strcmp(palavras_reservadas[i], k.lexema) == 0)
				{
					k.token = TK_PRV;
					return k;
				}
			}
			k.token = TK_ID;
			return k;
			break;
		case 3:
			caractere = lerCaractere();
			while (caractere >= '0' && caractere <= '9')
			{
				vetor[cont] = caractere;
				cont++;
				caractere = lerCaractere();
			}
			if (caractere == '.')
			{
				vetor[cont] = caractere;
				cont++;
				estado = 6;
			}
			else
				estado = 4;
			break;
		case 4:
			k.lexema = vetor;
			k.token = TK_INT;

			return k;
			break;

		case 5:
			k.lexema = vetor;
			k.token = TK_CM;

			return k;
			break;
		case 6:
			caractere = lerCaractere();
			while (caractere >= '0' && caractere <= '9')
			{
				vetor[cont] = caractere;
				cont++;
				caractere = lerCaractere();
			}
			estado = 7;
			break;
		case 7:
			k.lexema = vetor;
			k.token = TK_FLOAT;
			return k;
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere = '/')
			{
				estado = 13;
			}
			break;
		case 13:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			while (isascii(caractere) && caractere != '\n')
			{
				caractere = lerCaractere();
				vetor[cont] = caractere;
				cont++;
			}
			estado = 14;
			break;
		case 14:
			k.lexema = vetor;
			k.token = TK_CM;
			return k;
			break;
		case 15:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			while (isascii(caractere) && caractere != '*')
			{
				caractere = lerCaractere();
				vetor[cont] = caractere;
				cont++;
			}
			estado = 16;
			break;
		case 16:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			while (!isascii(caractere))
			{
				caractere = lerCaractere();
				vetor[cont] = caractere;
				cont++;
			}
			if (caractere == '/')
			{
				estado = 17;
			}
			else
			{
				estado = 15;
			}
			break;
		case 17:
			k.lexema = vetor;
			k.token = TK_CM;

			return k;
			break;
		case 18:
			k.lexema = vetor;
			k.token = TK_AD;

			return k;
			break;
		case 19:
			k.lexema = vetor;
			k.token = TK_SUB;

			return k;
			break;
		case 20:
			k.lexema = vetor;
			k.token = TK_DIV;

			return k;
			break;
		case 21:
			k.lexema = vetor;
			k.token = TK_MOD;

			return k;
			break;
		case 22:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '+')
				estado = 23;
			break;
		case 23:
			k.lexema = vetor;
			k.token = TK_INCREMENT;

			return k;
			break;
		case 24:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '-')
				estado = 25;
			break;
		case 25:
			k.lexema = vetor;
			k.token = TK_DECREMENT;

			return k;
			break;
		case 26:
			k.lexema = vetor;
			k.token = TK_MUL;

			return k;
			break;
		case 27:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 28;
			break;
		case 28:
			k.lexema = vetor;
			k.token = TK_ATR_AD;

			return k;
			break;
		case 29:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 30;
			break;
		case 30:
			k.lexema = vetor;
			k.token = TK_ATR_SUB;

			return k;
			break;
		case 31:
			k.lexema = vetor;
			k.token = TK_ATR;

			return k;
			break;
		case 32:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 35;
			break;
		case 33:
			k.lexema = vetor;
			k.token = TK_IG;

			return k;
			break;
		case 34:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 35;
			break;
		case 35:
			k.lexema = vetor;
			k.token = TK_ATR_MUL;

			return k;
			break;
		case 36:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 37;
			break;
		case 37:
			k.lexema = vetor;
			k.token = TK_ATR_MOD;

			return k;
			break;
		case 38:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 39;
			break;
		case 39:
			k.lexema = vetor;
			k.token = TK_ATR_BOU;

			return k;
			break;
		case 40:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 41;
			break;
		case 41:
			k.lexema = vetor;
			k.token = TK_ATR_BOU_EX;

			return k;
			break;
		case 42:

			break;
		case 43:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 44;
			break;
		case 44:
			k.lexema = vetor;
			k.token = TK_ATR_DES_ES;

			return k;
			break;
		case 45:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 46;
			break;
		case 46:
			k.lexema = vetor;
			k.token = TK_IG;

			return k;
			break;
		case 47:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 48;
			break;
		case 48:
			k.lexema = vetor;
			k.token = TK_DIF;

			return k;
			break;
		case 49:
			k.lexema = vetor;
			k.token = TK_MENOR;

			return k;
			break;
		case 50:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 52;
			break;
		case 51:
			k.lexema = vetor;
			k.token = TK_MENORIG;

			return k;
			break;
		case 52:
			k.lexema = vetor;
			k.token = TK_MAIORIG;

			return k;
			break;
		case 53:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 51;
			break;
		case 54:
			k.lexema = vetor;
			k.token = TK_MAIOR;

			return k;
			break;
		case 55:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '&')
				estado = 56;
			break;
		case 56:
			k.lexema = vetor;
			k.token = TK_AND;

			return k;
			break;
		case 57:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '|')
				estado = 58;
			break;
		case 58:
			k.lexema = vetor;
			k.token = TK_OR;

			return k;
			break;
		case 59:
			k.lexema = vetor;
			k.token = TK_NEG;

			return k;
			break;
		case 60:
			k.lexema = vetor;
			k.token = TK_ADDRESS;

			return k;
			break;
		case 61:
			k.lexema = vetor;
			k.token = TK_BOU_EX;

			return k;
			break;
		case 62:
			k.lexema = vetor;
			k.token = TK_BOU;

			return k;
			break;
		case 63:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '<')
			{
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 43;
					VoltarCaractere();
				}
				else
					estado = 65;
			}
			break;
		case 64:
			k.lexema = vetor;
			k.token = TK_DES_D;

			return k;
			break;
		case 65:
			k.lexema = vetor;
			k.token = TK_DES_ES;

			return k;
			break;
		case 66:
			k.lexema = vetor;
			k.token = TK_BCOMP;

			return k;
			break;
		case 67:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '=')
				estado = 70;
			break;
		case 68:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '>')
			{
				char prox = lerCaractere();
				if (prox == '=')
				{
					estado = 67;
					VoltarCaractere();
				}
				else
					estado = 64;
			}
			break;
		case 69:
			break;
		case 70:
			k.lexema = vetor;
			k.token = TK_ATR_DES_D;

			return k;
			break;
		case 71:
			break;
		case 72:
			break;
		case 73:
			k.lexema = vetor;
			k.token = TK_AC;

			return k;
			break;
		case 74:
			k.lexema = vetor;
			k.token = TK_FC;

			return k;
			break;
		case 75:
			k.lexema = vetor;
			k.token = TK_AP;

			return k;
			break;
		case 76:
			k.lexema = vetor;
			k.token = TK_FP;

			return k;
			break;
		case 77:
			k.lexema = vetor;
			k.token = TK_PV;

			return k;
			break;
		case 78:
			k.lexema = vetor;
			k.token = TK_VG;

			return k;
			break;
		case 79:
			break;
		case 80:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			while (caractere != '"')
			{
				caractere = lerCaractere();
				vetor[cont] = caractere;
				cont++;
			}
			estado = 81;
			break;
		case 81:
			k.lexema = vetor;
			k.token = TK_TEXT;

			return k;
			break;
		case 82:
			caractere = lerCaractere();
			if (isascii(caractere))
			{
				vetor[cont] = caractere;
				cont++;
				estado = 83;
			}
			break;
		case 83:
			k.lexema = vetor;
			k.token = TK_CHAR;

			return k;
			break;
		case 84:
			caractere = lerCaractere();
			while (strstr(vetor, "#include") == NULL)
			{
				vetor[cont] = caractere;
				cont++;
				estado = 91;
				caractere = lerCaractere();
			}

			break;
		case 91:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '"')
			{
				estado = 92;
			}
			else if (caractere == '<')
			{
				estado = 95;
			}
			break;
		case 92:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			while (caractere >= 'a' && caractere <= 'z')
			{
				caractere = lerCaractere();
				vetor[cont] = caractere;
				cont++;
			}
			if (caractere == '.')
			{
				estado = 93;
			}
			break;
		case 93:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == 'h' || caractere == 'c')
			{
				estado = 94;
			}
			break;
		case 94:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '"')
			{
				estado = 96;
			}
			break;
		case 95:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			while (caractere >= 'a' && caractere <= 'z')
			{
				caractere = lerCaractere();
				vetor[cont] = caractere;
				cont++;
			}
			if (caractere == '.')
			{
				estado = 97;
			}
			break;
		case 96:
			k.lexema = vetor;
			k.token = TK_DIRE;

			return k;
			break;
		case 97:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == 'h' || caractere == 'c')
			{
				estado = 98;
			}
			break;
		case 98:
			caractere = lerCaractere();
			vetor[cont] = caractere;
			cont++;
			if (caractere == '>')
			{
				estado = 96;
			}
			break;
		}
	}
	k.lexema = "";
	k.token = TK_END;
	k.linha = contLinha++;
	return k;
}
