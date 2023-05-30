#include "../AnalisadorLexico/analisadorLexico.c"

char tipoVariavelActual[MAX];
int ctrScopo = 0;
int linhaTmp = 0;
int ctrDire = 0;
int ctrRecursivo = 0;
char idActual[MAX];
char msg[MAX];

Tipo tipoTokenActual;
Erro *listaErro = NULL;
ListaCodigo *lista = NULL;
listaVariaveis *listaVar = NULL;

void program(FILE *ficheiro)
{
   lista = listaAnalisadorSinatico(ficheiro);
   if (lista == NULL)
   {
      printf("Ficheiro Vazio!");
   }
   else
   {
      declaration_definition();
   }
}

void display(Tipo t)
{
   printf("-----------------------------------------------------------------\n");
   printf("| Token:              | %s\n", t.token);
   printf("| Lexema:             | %s\n", t.lexema);
   printf("| Linha:              | %d\n", t.linha);
   printf("| Valor Atribuido:    | %s\n", t.valorAtribuido);
   printf("| Endereco de Memoria:| %p\n", (void *)t.enderecoMemoria);
   printf("| Escopo da Variavel: | %s\n", t.escopoVariavel);
   printf("| Tipo da Variavel:   | %s\n", t.tipoVariavel);
   printf("| Tamanho em Bytes:   | %zu\n", t.tamanhoByte);
   printf("-----------------------------------------------------------------\n");
}

void adicionarErro(char *mensagem, int linha)
{
   Erro *novoErro = (Erro *)malloc(sizeof(Erro));
   novoErro->mensagem = mensagem;
   novoErro->linha = linha;
   novoErro->next = NULL;

   if (listaErro == NULL)
   {
      listaErro = novoErro;
   }
   else
   {
      Erro *atual = listaErro;
      while (atual->next != NULL)
      {
         atual = atual->next;
      }
      atual->next = novoErro;
   }
}

void adicionarVariavel(char *variavel, int linha)
{
   listaVariaveis *novaVar = (listaVariaveis *)malloc(sizeof(listaVariaveis));
   novaVar->varivel=variavel;
   novaVar->linha = linha;
   novaVar->next = NULL;

   if (listaVar == NULL)
      listaVar = novaVar;
   else
   {
      listaVariaveis *actual = listaVar;
      while (actual->next != NULL)
      {
         actual = actual->next;
      }
      actual->next = novaVar;
   }
}

void declaration_definition()
{
   // Verifica se foi lido um comentario
   if (strcmp(lista->dadoToken.token, TK_CM) == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
   }
   // verifica se foi lido uma directiva de biblioteca
   if (strcmp(lista->dadoToken.token, TK_DIRE) == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      ctrRecursivo = 1;
      declaration_definition();
      // Verifica se foi lido uma declaração de variavel ou funcao
      declaration_specifier();
      ctrRecursivo = 1;
      declaration_definition();
      declaration_definition1();
   }
   else if (ctrRecursivo == 1)
   {
      ctrDire = 1;
   }
   else
   {
      // Verifica se foi lido uma declaração de variavel ou funcao
      declaration_specifier();
      ctrRecursivo = 1;
      declaration_definition();
      ctrRecursivo = 0;
      declaration_definition1();
   }
}

void declaration_specifier()
{
   if (is_storage_class_specifier() == 1)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      if (is_type_specifier() == 1)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
      }
      else
      {
         strcpy(msg , "[Atencao] Esperava encontrar um tipo de declaracao");
      }
   }
   else
   {
      if (is_type_specifier() == 1)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
      }
      else if (is_type_qualifier() == 1)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
         if (is_type_specifier() == 1)
         {
            display(lista->dadoToken);
            lista = lista->next;
            linhaTmp = lista->dadoToken.linha;
         }
         else
         {
            strcpy(msg, "[Atencao] Esperava encontrar um tipo de declaracao");
         }
      }
      strcpy(msg, "[Atencao] Esperava encontrar um tipo de declaracao");
   }
}

void declaration_definition1()
{
   struct_or_union_specifier();
   declarator();
}

void struct_or_union_specifier()
{
   if (is_struct_or_union() == 1)
   {
      strcpy(tipoVariavelActual, lista->dadoToken.lexema);
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      struct_or_union_specifier1();
   }
   else
   {
      declarator();
   }
}

void declarator()
{
   pointer();
   if(is_identifier() == 1)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
   }
}
/*
void declarator1()
{
   function_definition();
   parameter_list();
   direct_declaration();
}
/*
void declaration()
{
    while (1)
    {
        if (is_init_declarator())
        {
            process_init_declarator();
        }
        else
        {
            break;
        }
    }
    if (is_declaration1())
    {
        process_declaration1();
    }
}

void init_declarator()
{
    if (is_assignment_operator())
    {
        process_initializer();
    }
}

int is_pointer()
{
    // Verificar se há um <pointer>
    // Implemente a lógica de verificação aqui
}

void process_pointer()
{
    // Processar o <pointer>
    // Implemente a lógica de processamento aqui
}
*/

int is_identifier()
{
   if (strcmp(lista->dadoToken.token, TK_ID) == 0)
   {
      return 1;
   }
   else
      return 0;
}
/*
void process_identifier()
{
    // Processar o "ID"
    // Implemente a lógica de processamento aqui
}

int is_declarator1()
{
    // Verificar se há um <declarator1>
    // Implemente a lógica de verificação aqui
}

int direct_declaration()
{
    // Verificar se há uma <direct-declaration>
    if (strcmp(tipoTokenActual, TK_IG) == 0)
    {
        adicionarElemento(tipoTokenActual);
        tipoTokenActual = analex(ficheiro);
        return true;
    }
    else
        return false;
}

void process_direct_declaration()
{
    // Processar a <direct-declaration>
    // Implemente a lógica de processamento aqui
}

int is_init_declarator()
{
    // Verificar se há um <init-declarator>
    // Implemente a lógica de verificação aqui
}

void process_init_declarator()
{
    // Processar o <init-declarator>
    // Implemente a lógica de processamento aqui
}

int is_declaration1()
{
    // Verificar se há um <declaration1>
    // Implemente a lógica de verificação aqui
}

void process_declaration1()
{
    // Processar o <declaration1>
    // Implemente a lógica de processamento aqui
}

int is_assignment_operator()
{
    // Verificar se há um operador de atribuição
    // Implemente a lógica de verificação aqui
}

void process_initializer()
{
    // Processar o <initializer>
    // Implemente a lógica de processamento aqui
}

void function_definition()
{
    function_declaration();
    compound_statement();
}

void function_declaration()
{
    if (strcmp(tipoTokenActual.token, TK_AP) == 0)
    {
        adicionarElemento(tipoTokenActual);
        tipoTokenActual = analex(ficheiro);
        parameter_list();

        if (strcmp(tipoTokenActual.token, TK_FP) == 0)
        {
            adicionarElemento(tipoTokenActual);
            tipoTokenActual = analex(ficheiro);
        }
        else
        {
            // Erro: fechamento de parênteses esperado
        }
    }
    else
    {
        // Erro: abertura de parênteses esperada
    }
}

void direct_declaration()
{
    if (is_open_bracket())
    {
        if (is_constant_expression())
        {
            // Processar a expressão constante
        }
        if (is_close_bracket())
        {
            direct_declarator();
        }
        else
        {
            // Erro: fechamento de colchete esperado
        }
    }
    else if (is_declaration())
    {
        // Processar a declaração
    }
}
*/
int is_storage_class_specifier()
{
   if (strcmp(lista->dadoToken.lexema, "auto") == 0 || strcmp(lista->dadoToken.lexema, "static") == 0 || strcmp(lista->dadoToken.lexema, "extern") == 0 || strcmp(lista->dadoToken.lexema, "typedef") == 0)
   {
      strcpy(tipoVariavelActual, lista->dadoToken.lexema);
      return 1;
   }
   else
   {
      return 0;
   }
}

int is_type_specifier()
{
   // Verifica se há um <declaration-specifier>
   if (strcmp(lista->dadoToken.lexema, "void") == 0 ||
       strcmp(lista->dadoToken.lexema, "char") == 0 ||
       strcmp(lista->dadoToken.lexema, "short") == 0 ||
       strcmp(lista->dadoToken.lexema, "int") == 0 ||
       strcmp(lista->dadoToken.lexema, "long") == 0 ||
       strcmp(lista->dadoToken.lexema, "float") == 0 ||
       strcmp(lista->dadoToken.lexema, "double") == 0 ||
       strcmp(lista->dadoToken.lexema, "signed") == 0 ||
       strcmp(lista->dadoToken.lexema, "unsigned") == 0 ||
       strcmp(lista->dadoToken.lexema, "enum") == 0)
   {
      strcpy(tipoVariavelActual, lista->dadoToken.lexema);
      return 1;
   } /*
    else if (is_enum_specifier())
    {
        // Processar o especificador de enumeração
    }
    else if (is_typedef_name())
    {
        // Processar o nome de um typedef
    }*/

   else
   {
      return 0;
   }
}

int is_type_qualifier()
{
   if (strcmp(lista->dadoToken.lexema, "const") == 0 ||
       strcmp(lista->dadoToken.lexema, "volatile") == 0)
   {
      strcpy(tipoVariavelActual, lista->dadoToken.lexema);
      return 1;
   }
   else
   {
      return 0;
   }
}

void struct_or_union_specifier1()
{
   if (is_identifier() == 1)
   {
      strcpy(lista->dadoToken.tipoVariavel, tipoVariavelActual);
      printf("TESTE \n");
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      struct_or_union_specifier2();
   }
   else if (strcmp(tipoTokenActual.token, TK_AC) == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      struct_declaration();
      if (strcmp(tipoTokenActual.token, TK_FC) == 0)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
         struct_fim();
      }
      else
      {
         // Erro: fechamento de chave esperado
      }
   }
   else
   {
      // Erro: identificador ou abertura de chave esperada
   }
}

void struct_or_union_specifier2()
{
   if (strcmp(lista->dadoToken.token, TK_AC) == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      struct_declaration();
      if (strcmp(lista->dadoToken.token, TK_FC) == 0)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
         struct_fim();
      }
      else
      {
         strcpy(msg, "[Erro] esperava '}' em falta");
         adicionarErro(msg, lista->dadoToken.linha);
      }
   }
   else
   {
      // Lista de declarações vazia
   }
}

void struct_fim()
{
   if (strcmp(lista->dadoToken.token, TK_ID) == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      if (strcmp(lista->dadoToken.token, TK_PV) == 0)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
         return;
      }
      else
      {
         strcpy(msg, "[Erro] eponto e vírgula esperado ';'");
         adicionarErro(msg, lista->dadoToken.linha);
         return;
      }
   }
   else if (strcmp(lista->dadoToken.token, TK_PV) == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      return;
   }
   else
   {
      strcpy(msg, "[Erro] identificador ou ponto e vírgula esperado  ';'");
      adicionarErro(msg, lista->dadoToken.linha);
      return;
   }
}

// VERIFICA O TIPO DA ESTRUTURA OU UNIO
int is_struct_or_union()
{
   if (strcmp(lista->dadoToken.lexema, "struct") == 0)
   {
      return 1;
   }
   else if (strcmp(lista->dadoToken.lexema, "union") == 0)
   {
      return 1;
   }
   else
      return 0;
}

// verifica a declaração da estrutura
void struct_declaration()
{
   specifier_qualifier();
   struct_declarator();
   struct_declaration();
}

// verifica o tipo de dado
void specifier_qualifier()
{
   if (is_type_qualifier() == 1)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      if (is_type_specifier() == 1)
      {
         display(lista->dadoToken);
         lista = lista->next;
         linhaTmp = lista->dadoToken.linha;
      }
      else
         strcpy(msg, "[Erro] especificador ou qualificador inválido");
      return;
   }
   if (is_type_specifier() == 1)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      return;
   }
   strcpy(msg, "[Erro] especificador ou qualificador inválido");
}


void struct_declarator()
{
   declarator();
}
/*
void struct_declarator1()
{
if (strcmp(":", lista->dadoToken.lexema) == 0)
{
   constant_expression();
}
else
{
   // Nenhuma expressão constante encontrada
}
}

void constant_expression()
{
conditional_expression();
}

void conditional_expression()
{
logical_or_expression();
conditional_expression1();
}

void conditional_expression1()
{
if (is_question_mark())
{
   expression();
   if (is_colon())
   {
       conditional_expression1();
   }
   else
   {
       // Erro: dois pontos esperado
   }
}
else
{
   // Expressão condicional opcional não encontrada
}
}

void logical_or_expression()
{
logical_and_expression();
logical_or_expression1();
}

void logical_or_expression1()
{
if (is_logical_or_operator())
{
   logical_and_expression();
   logical_or_expression1();
}
else
{
   // Operador lógico OU opcional não encontrado
}
}

void logical_and_expression()
{
inclusive_or_expression();
logical_and_expression1();
}

void logical_and_expression1()
{
if (is_logical_and_operator())
{
   inclusive_or_expression();
   logical_and_expression1();
}
else
{
   // Operador lógico E opcional não encontrado
}
}

void inclusive_or_expression()
{
exclusive_or_expression();
inclusive_or_expression1();
}

void inclusive_or_expression1()
{
if (is_inclusive_or_operator())
{
   exclusive_or_expression();
   inclusive_or_expression1();
}
else
{
   // Operador lógico OU inclusivo opcional não encontrado
}
}

void exclusive_or_expression()
{
and_expression();
exclusive_or_expression1();
}

void exclusive_or_expression1()
{
if (is_exclusive_or_operator())
{
   and_expression();
   exclusive_or_expression1();
}
else
{
   // Operador XOR opcional não encontrado
}
}

void and_expression()
{
equality_expression();
and_expression1();
}

void and_expression1()
{
if (is_and_operator())
{
   equality_expression();
   and_expression1();
}
else
{
   // Operador E opcional não encontrado
}
}

void equality_expression()
{
relational_expression();
equality_expression1();
}

void equality_expression1()
{
if (is_equality_operator())
{
   relational_expression();
   equality_expression1();
}
else
{
   // Operador de igualdade opcional não encontrado
}
}

void relational_expression()
{
shift_expression();
relational_expression1();
}

void relational_expression1()
{
if (is_relational_operator())
{
   shift_expression();
   relational_expression1();
}
else
{
   // Operador relacional opcional não encontrado
}
}

void shift_expression()
{
additive_expression();
shift_expression1();
}

void shift_expression1()
{
if (is_shift_operator())
{
   additive_expression();
   shift_expression1();
}
else
{
   // Operador de deslocamento opcional não encontrado
}
}

void additive_expression()
{
multiplicative_expression();
additive_expression1();
}

void additive_expression1()
{
if (is_additive_operator())
{
   multiplicative_expression();
   additive_expression1();
}
else
{
   // Operador aditivo opcional não encontrado
}
}

void multiplicative_expression()
{
cast_expression();
multiplicative_expression1();
}

void multiplicative_expression1()
{
if (is_multiplicative_operator())
{
   cast_expression();
   multiplicative_expression1();
}
else
{
   // Operador multiplicativo opcional não encontrado
}
}

void cast_expression()
{
if (is_unary_expression())
{
   cast_expression();
}
else
{
   // Vazio (ε)
}
}

void unary_expression()
{
if (is_postfix_expression())
{
   // Nada a fazer, já está tratado na regra postfix_expression
}
else if (is_unary_operator())
{
   cast_expression();
}
else if (is_sizeof_keyword())
{
   unary_expression1();
}
else if (is_increment_operator())
{
   unary_expression();
}
else if (is_decrement_operator())
{
   unary_expression();
}
else
{
   // Expressão unária inválida
}
}

void unary_expression1()
{
if (is_unary_expression())
{
   // Nada a fazer, já está tratado na regra unary_expression
}
else if (is_type_name())
{
   // Nada a fazer, tipo de nome tratado em outra parte do código
}
else
{
   // Expressão unária inválida
}
}

void postfix_expression()
{
if (is_primary_expression())
{
   postfix_expression1();
}
else
{
   // Postfix expression inválida
}
}

void postfix_expression1()
{
if (is_array_access_operator())
{
   expression();
   // Nada a fazer, array access tratado em outra parte do código
   postfix_expression1();
}
else if (is_function_call_operator())
{
   // Nada a fazer, função chamada tratada em outra parte do código
   postfix_expression1();
}
else if (is_member_access_operator())
{
   // Nada a fazer, acesso a membro tratado em outra parte do código
   postfix_expression1();
}
else if (is_increment_operator())
{
   // Nada a fazer, incremento tratado em outra parte do código
   postfix_expression1();
}
else if (is_decrement_operator())
{
   // Nada a fazer, decremento tratado em outra parte do código
   postfix_expression1();
}
else
{
   // Vazio (ε)
}
}

void primary_expression()
{
if (is_identifier())
{
   // Nada a fazer, identificador tratado em outra parte do código
}
else if (is_constant())
{
   // Nada a fazer, constante tratada em outra parte do código
}
else if (is_string())
{
   // Nada a fazer, string tratada em outra parte do código
}
else
{
   // Expressão primária inválida
}
}

void desp1()
{
if (is_expression())
{
   // Nada a fazer, expressão tratada em outra parte do código
   desp1();
}
else if (is_type_name())
{
   // Nada a fazer, tipo de nome tratado em outra parte do código
   desp1();
}
else if (is_closing_parenthesis())
{
   // Vazio (ε)
}
else
{
   // Expressão desp1 inválida
}
}

void expression()
{
assignment_expression();
expression1();
}

void expression1()
{
if (is_comma_operator())
{
   assignment_expression();
   expression1();
}
else
{
   // Vazio (ε)
}
}

void assignment_expression()
{
if (is_conditional_expression())
{
   // Nada a fazer, já está tratado na regra conditional_expression
}
else if (is_unary_expression())
{
   if (is_assignment_operator())
   {
       assignment_expression();
   }
   else
   {
       // Expressão de atribuição inválida
   }
}
else
{
   // Expressão de atribuição inválida
}
}

void assignment_operator()
{
if (is_assignment_operator())
{
   // Nada a fazer, operador de atribuição válido
}
else
{
   // Operador de atribuição inválido
}
}

void unary_operator()
{
if (is_unary_operator())
{
   // Nada a fazer, operador unário válido
}
else
{
   // Operador unário inválido
}
}

void type_name()
{
if (is_specifier_qualifier())
{
   // Nada a fazer, especificador/qualificador de tipo tratado em outra parte do código
   while (is_specifier_qualifier())
   {
       // Loop para lidar com múltiplos especificadores/qualificadores de tipo
   }
   if (is_abstract_declarator())
   {
       // Nada a fazer, declarador abstrato tratado em outra parte do código
   }
}
else
{
   // Nome de tipo inválido
}
}

void parameter_list()
{
declaration_definition();
parameter_list1();
parameter_list_id();
}

void parameter_list1()
{
if (strcmp(tipoTokenActual.token, TK_VG)==0)
{
   declaration_definition();
   parameter_list1();
}
else
{
   // Vazio (ε)
}
}

void parameter_list_id()
{
if (is_identifier())
{
   parm1();
}
else
{
   // Vazio (ε)
}
}

void parm1()
{
if (is_comma_operator())
{
   // Nada a fazer, vírgula tratada em outra parte do código
   identifier();
   parm1();
}
else
{
   // Vazio (ε)
}
}

void abstract_declarator()
{
if (is_pointer())
{
   abstract_declarator1();
}
else if (is_direct_abstract_declarator())
{
   // Nada a fazer, declarador abstrato direto tratado em outra parte do código
}
else
{
   // Declarador abstrato inválido
}
}

void abstract_declarator1()
{
if (is_direct_abstract_declarator())
{
   // Nada a fazer, declarador abstrato direto tratado em outra parte do código
}
else
{
   // Vazio (ε)
}
}

void direct_abstract_declarator()
{
if (is_opening_parenthesis())
{
   abstract_declarator();
   // Verificar se é necessário fechar parêntese aqui
}
else if (is_direct_abstract_declarator1())
{
   // Nada a fazer, declarador abstrato direto tratado em outra parte do código
}
else
{
   // Declarador abstrato direto inválido
}
}

void direct_abstract_declarator1()
{
if (is_array_access_operator())
{
   // Nada a fazer, operador de acesso a array tratado em outra parte do código
   if (is_constant_expression())
   {
       // Nada a fazer, expressão constante tratada em outra parte do código
   }
   direct_abstract_declarator1();
}
else if (is_function_call_operator())
{
   // Nada a fazer, operador de chamada de função tratado em outra parte do código
   if (is_parameter_type_list())
   {
       // Nada a fazer, lista de tipos de parâmetros tratada em outra parte do código
   }
   direct_abstract_declarator1();
}
else
{
   // Vazio (ε)
}
}

void enum_specifier()
{
if (is_enum())
{
   enum_specifier1();
}
else
{
   // Enum specifier inválido
}
}

void enum_specifier1()
{
if (is_identifier())
{
   enum_specifier2();
}
else if (is_opening_brace())
{
   enumeration_list();
   if (is_closing_brace())
   {
       // Nada a fazer, fechamento de chaves tratado em outra parte do código
   }
   else
   {
       // Falta fechar a chave '}' do enum specifier
   }
}
else
{
   // Enum specifier inválido
}
}

void enum_specifier2()
{
if (is_opening_brace())
{
   enumeration_list();
   if (is_closing_brace())
   {
       // Nada a fazer, fechamento de chaves tratado em outra parte do código
   }
   else
   {
       // Falta fechar a chave '}' do enum specifier
   }
}
else
{
   // Vazio (ε)
}
}

void enumeration_list()
{
if (is_enumerator())
{
   enumeration_list1();
}
else
{
   // Enumeração vazia
}
}

void enumeration_list1()
{
if (is_comma_operator())
{
   enumerator();
   enumeration_list1();
}
else
{
   // Vazio (ε)
}
}

void enumerator()
{
if (is_identifier())
{
   enumerator1();
}
else
{
   // Enumerator inválido
}
}

void enumerator1()
{
if (is_assignment_operator())
{
   constant_expression();
}
else
{
   // Vazio (ε)
}
}

void typedef_name()
{
if (is_identifier())
{
   // Identificador válido para typedef-name
}
else
{
   // Identificador inválido para typedef-name
}
}

void initializer()
{
if (is_assignment_expression())
{
   // Inicialização com assignment-expression
}
else if (is_initializer_list())
{
   initializer_list();
   initializer1();
}
else
{
   // Inicializador inválido
}
}

void initializer1()
{
if (is_comma_operator())
{
   if (is_closing_brace())
   {
       // Nada a fazer, fechamento de chaves tratado em outra parte do código
   }
   else
   {
       initializer();
   }
}
else if (!is_closing_brace())
{
   // Esperado fechamento de chaves '}' ou vírgula ','
}
}

void initializer_list()
{
initializer();
initializer_list1();
}

void initializer_list1()
{
if (is_comma_operator())
{
   initializer();
   initializer_list1();
}
else
{
   // Vazio (ε)
}
}
*/

//Verifica se é um ponteiro
void pointer()
{
   if (strcmp(lista->dadoToken.lexema, "*") == 0)
   {
      display(lista->dadoToken);
      lista = lista->next;
      linhaTmp = lista->dadoToken.linha;
      pointer();
   }
}
/*
void compound_statement()
{
if (is_opening_brace())
{
   // Processar declarações (declaration-definition) opcionais
   w)
   {
       // Processar declaration-definition
   }
   // Processar statements (statement) opcionais
   while (is_statement())
   {
       // Processar statement
   }
   if (is_closing_brace())
   {
       // Nada a fazer, fechamento de chaves tratado em outra parte do código
   }
   else
   {
       // Falta fechar a chave '}' do compound statement
   }
}
else
{
   // Declaração de compound statement inválida
}
}

void statement()
{
if (is_labeled_statement())
{
   // Processar labeled statement
}
else if (is_expression_statement())
{
   // Processar expression statement
}
else if (is_compound_statement())
{
   // Processar compound statement
}
else if (is_selection_statement())
{
   // Processar selection statement
}
else if (is_iteration_statement())
{
   // Processar iteration statement
}
else if (is_jump_statement())
{
   // Processar jump statement
}
else if (is_function_statement())
{
   // Processar function statement
}
else
{
   // Statement inválido
}
}

void labeled_statement()
{
if (is_identifier())
{
   if (is_colon_operator())
   {
       statement();
   }
   else
   {
       // Falta o operador de dois pontos ':' após o identificador
   }
}
else if (is_case_keyword())
{
   constant_expression();
   if (is_colon_operator())
   {
       statement();
   }
   else
   {
       // Falta o operador de dois pontos ':' após a expressão constante do case
   }
}
else if (is_default_keyword())
{
   if (is_colon_operator())
   {
       statement();
   }
   else
   {
       // Falta o operador de dois pontos ':' após a keyword default
   }
}
else
{
   // Labeled statement inválido
}
}

void expression_statement()
{
if (is_expression())
{
   if (is_semicolon_operator())
   {
       // Nada a fazer, ponto e vírgula tratado em outra parte do código
   }
   else
   {
       // Falta o ponto e vírgula ';'
   }
}
else if (is_semicolon_operator())
{
   // Expressão vazia (ε)
}
else
{
   // Expression statement inválido
}
}

void selection_statement()
{
if (is_if_keyword())
{
   if (is_opening_parenthesis())
   {
       expression();
       if (is_closing_parenthesis())
       {
           statement();
           selection_statement1();
       }
       else
       {
           // Falta fechar o parêntese ')'
       }
   }
   else
   {
       // Falta abrir o parêntese '('
   }
}
else if (is_switch_keyword())
{
   if (is_opening_parenthesis())
   {
       expression();
       if (is_closing_parenthesis())
       {
           statement();
       }
       else
       {
           // Falta fechar o parêntese ')'
       }
   }
   else
   {
       // Falta abrir o parêntese '('
   }
}
else
{
   // Selection statement inválido
}
}

void selection_statement1()
{
if (is_else_keyword())
{
   statement();
}
else
{
   // Vazio (ε)
}
}

void iteration_statement()
{
if (is_while_keyword())
{
   if (is_opening_parenthesis())
   {
       expression();
       if (is_closing_parenthesis())
       {
           statement();
       }
       else
       {
           // Falta fechar o parêntese ')'
       }
   }
   else
   {
       // Falta abrir o parêntese '('
   }
}
else if (is_do_keyword())
{
   statement();
   if (is_while_keyword())
   {
       if (is_opening_parenthesis())
       {
           expression();
           if (is_closing_parenthesis())
           {
               if (is_semicolon_operator())
               {
                   // Nada a fazer, ponto e vírgula tratado em outra parte do código
               }
               else
               {
                   // Falta o ponto e vírgula ';'
               }
           }
           else
           {
               // Falta fechar o parêntese ')'
           }
       }
       else
       {
           // Falta abrir o parêntese '('
       }
   }
   else
   {
       // Falta a keyword while
   }
}
else if (is_for_keyword())
{
   if (is_opening_parenthesis())
   {
       if (is_expression())
       {
           if (is_semicolon_operator())
           {
               // Nada a fazer, ponto e vírgula tratado em outra parte do código
           }
           else
           {
               // Falta o ponto e vírgula ';'
           }
       }
       if (is_expression())
       {
           if (is_semicolon_operator())
           {
               // Nada a fazer, ponto e vírgula tratado em outra parte do código
           }
           else
           {
               // Falta o ponto e vírgula ';'
           }
       }
       if (is_expression())
       {
           // Nada a fazer
       }
       if (is_closing_parenthesis())
       {
           statement();
       }
       else
       {
           // Falta fechar o parêntese ')'
       }
   }
   else
   {
       // Falta abrir o parêntese '('
   }
}
else
{
   // Iteration statement inválido
}
}

void jump_statement()
{
if (is_goto_keyword())
{
   if (is_identifier())
   {
       if (is_semicolon_operator())
       {
           // Nada a fazer, ponto e vírgula tratado em outra parte do código
       }
       else
       {
           // Falta o ponto e vírgula ';'
       }
   }
   else
   {
       // Falta o identificador após a keyword goto
   }
}
else if (is_continue_keyword())
{
   if (is_semicolon_operator())
   {
       // Nada a fazer, ponto e vírgula tratado em outra parte do código
   }
   else
   {
       // Falta o ponto e vírgula ';'
   }
}
else if (is_break_keyword())
{
   if (is_semicolon_operator())
   {
       // Nada a fazer, ponto e vírgula tratado em outra parte do código
   }
   else
   {
       // Falta o ponto e vírgula ';'
   }
}
else if (is_return_keyword())
{
   if (is_expression())
   {
       // Nada a fazer
   }
   if (is_semicolon_operator())
   {
       // Nada a fazer, ponto e vírgula tratado em outra parte do código
   }
   else
   {
       // Falta o ponto e vírgula ';'
   }
}
else
{
   // Jump statement inválido
}
}

void function_statement()
{
if (is_printf_keyword() || is_scanf_keyword() || is_free_keyword() ||
   is_strcpy_keyword() || is_malloc_keyword() || is_gets_keyword() ||
   is_putchar_keyword() || is_getchar_keyword() || is_fgets_keyword() ||
   is_calloc_keyword() || is_realloc_keyword() || is_identifier())
{
   if (is_function_compound())
   {
       if (is_semicolon_operator())
       {
           // Nada a fazer, ponto e vírgula tratado em outra parte do código
       }
       else
       {
           // Falta o ponto e vírgula ';'
       }
   }
   else
   {
       // Function compound inválido
   }
}
else
{
   // Function statement inválido
}
}

void function_compound()
{
if (is_opening_parenthesis())
{
   expression();
   if (is_closing_parenthesis())
   {
       if (is_semicolon_operator())
       {
           // Nada a fazer, ponto e vírgula tratado em outra parte do código
       }
       else
       {
           // Falta o ponto e vírgula ';'
       }
   }
   else
   {
       // Falta fechar o parêntese ')'
   }
}
else
{
   // Falta abrir o parêntese '('
}
}*/
