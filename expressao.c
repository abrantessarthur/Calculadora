#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


static float converteGrausParaRad(float angulo) {
    return angulo * M_PI / 180.0;
}


float getValorPosFixa(char *expressaoPosFixa) {
    if (expressaoPosFixa == NULL || strlen(expressaoPosFixa) == 0) {
        printf("Erro: Expressao em pos-fixa invalida ou vazia.\n");
        return NAN; 
    }

    float valores[300];
    int idxTopo = -1;
    char elemento[70];
    int indiceChar = 0;
    int i = 0;

    while (1) {
        
        if (expressaoPosFixa[i] == ' ' || expressaoPosFixa[i] == '\0') {

            if (indiceChar > 0) {
                elemento[indiceChar] = '\0';
                indiceChar = 0;

                if (strcmp(elemento, "+") == 0 ||
                    strcmp(elemento, "-") == 0 ||
                    strcmp(elemento, "*") == 0 ||
                    strcmp(elemento, "/") == 0 ||
                    strcmp(elemento, "^") == 0 ||
                    strcmp(elemento, "%") == 0)
                {
                    if (idxTopo < 1) {
                        printf("Erro: Operadores insuficientes para operacao binaria.\n");
                        return NAN; 
                    }
                    
                    float op2 = valores[idxTopo--];
                    float op1 = valores[idxTopo--];

                    if (strcmp(elemento, "+") == 0) valores[++idxTopo] = op1 + op2;
                    else if (strcmp(elemento, "-") == 0) valores[++idxTopo] = op1 - op2;
                    else if (strcmp(elemento, "*") == 0) valores[++idxTopo] = op1 * op2;
                    else if (strcmp(elemento, "/") == 0) valores[++idxTopo] = op1 / op2;
                    else if (strcmp(elemento, "^") == 0) valores[++idxTopo] = pow(op1, op2);
                    else if (strcmp(elemento, "%") == 0) valores[++idxTopo] = fmod(op1, op2);
                }
                
                else if (strcmp(elemento, "sen") == 0 ||
                         strcmp(elemento, "cos") == 0 ||
                         strcmp(elemento, "tg") == 0 ||
                         strcmp(elemento, "log") == 0 ||
                         strcmp(elemento, "raiz") == 0)
                {
                    if (idxTopo < 0) {
                        printf("Erro: Operando faltando para funcao unaria.\n");
                        return NAN; 
                    }
                    
                    float operando = valores[idxTopo--];

                    if (strcmp(elemento, "sen") == 0)
                        valores[++idxTopo] = sin(converteGrausParaRad(operando));

                    else if (strcmp(elemento, "cos") == 0)
                        valores[++idxTopo] = cos(converteGrausParaRad(operando));

                    else if (strcmp(elemento, "tg") == 0)
                        valores[++idxTopo] = tan(converteGrausParaRad(operando));

                    else if (strcmp(elemento, "log") == 0)
                        valores[++idxTopo] = log10(operando);

                    else if (strcmp(elemento, "raiz") == 0)
                        valores[++idxTopo] = sqrt(operando);
                }
                
                else {
                    valores[++idxTopo] = atof(elemento);
                }
            }
            
            if (expressaoPosFixa[i] == '\0') break;
        }
        
        else {
            elemento[indiceChar++] = expressaoPosFixa[i];
        }
        i++;
    }

    if (idxTopo != 0) {
        printf("Erro: Expressao pos-fixa mal formada (operandos/operadores em excesso).\n");
        return NAN; 
    }

    return valores[0];
}


char * getFormaInFixa(char *expressaoPosFixa) {
    if (!expressaoPosFixa) return NULL;

    char *pilhaStrings[300];
    int pilhaPrecedencia[300];
    int idxTopo = -1;
    char elemento[70];
    int indiceChar = 0;

    #define OBTER_PRECEDENCIA(op) ( \
        (op=='+'||op=='-') ? 1 : \
        (op=='*'||op=='/'||op=='%') ? 2 : \
        (op=='^') ? 3 : \
        10 \
    )

    int i = 0;
    while (1) {
        if (expressaoPosFixa[i] == ' ' || expressaoPosFixa[i] == '\0') {

            if (indiceChar > 0) {
                elemento[indiceChar] = '\0';
                indiceChar = 0;

                if (strcmp(elemento, "+")==0 || strcmp(elemento, "-")==0 ||
                    strcmp(elemento, "*")==0 || strcmp(elemento, "/")==0 ||
                    strcmp(elemento, "%")==0 || strcmp(elemento, "^")==0)
                {
                    if (idxTopo < 1) return NULL;

                    char *expr2 = pilhaStrings[idxTopo];
                    int prec2 = pilhaPrecedencia[idxTopo];
                    idxTopo--;

                    char *expr1 = pilhaStrings[idxTopo];
                    int prec1 = pilhaPrecedencia[idxTopo];
                    idxTopo--;

                    int p = OBTER_PRECEDENCIA(elemento[0]);

                    char Buffer1[512], Buffer2[512];

                    if (prec1 < p)
                        sprintf(Buffer1, "(%s)", expr1);
                    else
                        sprintf(Buffer1, "%s", expr1);

                    if (prec2 < p ||
                       (prec2 == p && (elemento[0]=='-' || elemento[0]=='/')))
                        sprintf(Buffer2, "(%s)", expr2);
                    else
                        sprintf(Buffer2, "%s", expr2);

                    char bufResultado[1024];
                    sprintf(bufResultado, "%s%s%s", Buffer1, elemento, Buffer2);

                    free(expr1);
                    free(expr2);

                    pilhaStrings[++idxTopo] = strdup(bufResultado);
                    pilhaPrecedencia[idxTopo] = p;
                }
                
                else if (!strcmp(elemento,"sen") || !strcmp(elemento,"cos") ||
                         !strcmp(elemento,"tg")  || !strcmp(elemento,"log") ||
                         !strcmp(elemento,"raiz"))
                {
                    if (idxTopo < 0) return NULL;

                    char *expr1 = pilhaStrings[idxTopo];
                    idxTopo--;

                    char bufResultado[1024];
                    sprintf(bufResultado, "%s(%s)", elemento, expr1);

                    free(expr1);

                    pilhaStrings[++idxTopo] = strdup(bufResultado);
                    pilhaPrecedencia[idxTopo] = 4;
                }

                else {
                    pilhaStrings[++idxTopo] = strdup(elemento);
                    pilhaPrecedencia[idxTopo] = 10;
                }
            }

            if (expressaoPosFixa[i] == '\0') break;
        }
        
        else {
            elemento[indiceChar++] = expressaoPosFixa[i];
        }
        i++;
    }

    if (idxTopo != 0)
        return NULL;

    return pilhaStrings[0];
}