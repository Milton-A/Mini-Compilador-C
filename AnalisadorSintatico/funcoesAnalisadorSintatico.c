#include "../AnalisadorLexico/token.h"
#include "headerSintatico.h"

Tipo program(Tipo *t)
{

}

void decl1()
{
    if(T == TK_PV)
    {
        T = analex();
    }
    else if(T == TK_APR)
    {
        T = analex();
        exp();
        if(T == ']')
        {
            T = analex();
            if(T == TK_PV)
            {
                T = analex();
            }
        }
    }
    else if(T == TK_AP)
    {
        parms();
        if( T == TK_FP)
        {
            T = analex();
            comando();            
        }
    }
}

void decl()
{
    if(T == TK_INT || T == TK_FLOAT || T == TK_VOID)
    {
        T = analex();
        if(T == TK_ID)
        {
            T = analex();
            decl1();
        }
        else
        {
            printf("Erro");
        }
    }
    else
    {
        printf("Erro");
    }
}

void comando()
{
    if(T == TK_AC)
    {
        localDecl();
        stmtList();
        if(T == TK_FC)
        {
            T = analex();
        }
        else
        {
            printf("Erro Fechar chave");
        }
    }
}