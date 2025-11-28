#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];
    char inFixa[512];
    float Valor;
} CalculadoraExpr;


char * getFormaInFixa(char *Str);
char * getFormaPosFixa(char *StrInFixa);
float getValorPosFixa(char *StrPosFixa);

#endif