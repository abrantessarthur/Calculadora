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

static int getPrecedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

static int ehOperador(char *s) {
    return strcmp(s, "+") == 0 || strcmp(s, "-") == 0 ||
           strcmp(s, "*") == 0 || strcmp(s, "/") == 0 ||
           strcmp(s, "%") == 0 || strcmp(s, "^") == 0;
}

static int ehFuncao(char *s) {
    return strcmp(s, "sen") == 0 || strcmp(s, "cos") == 0 ||
           strcmp(s, "tg") == 0 || strcmp(s, "log") == 0 ||
           strcmp(s, "raiz") == 0;
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

    if (idxTopo != 0) {
        for(i=0; i <= idxTopo; i++) free(pilhaStrings[i]);
        return NULL;
    }

    char* finalResult = pilhaStrings[0];
    return finalResult;
}

char * getFormaPosFixa(char *expressaoInFixa) {
    if (expressaoInFixa == NULL || strlen(expressaoInFixa) == 0) {
        return strdup("");
    }

    char *pilhaOperadores[300];
    int idxTopo = -1;
    
    char *resultadoPosFixa = (char *)malloc(1024 * sizeof(char));
    if (resultadoPosFixa == NULL) return NULL;
    resultadoPosFixa[0] = '\0';

    char elemento[70];
    int indiceChar = 0;
    int i = 0;

    while (1) {
        
        char caractere = expressaoInFixa[i];

        if (caractere == ' ' || caractere == '\0' || caractere == '(' || caractere == ')' ||
            caractere == '+' || caractere == '-' || caractere == '*' || caractere == '/' ||
            caractere == '%' || caractere == '^') 
        {
            
            if (indiceChar > 0) {
                elemento[indiceChar] = '\0';
                indiceChar = 0;
                
                if (atof(elemento) != 0.0 || strcmp(elemento, "0") == 0) {
                    strcat(resultadoPosFixa, elemento);
                    strcat(resultadoPosFixa, " ");
                }
                else if (ehFuncao(elemento)) {
                    pilhaOperadores[++idxTopo] = strdup(elemento);
                }
            }

            if (caractere == '\0') break;
            
            if (caractere == ' ') {
                i++;
                continue;
            }
            
            if (caractere == '+' || caractere == '-' || caractere == '*' || caractere == '/' || 
                caractere == '%' || caractere == '^') {
                
                char opStr[2] = {caractere, '\0'};
                int precAtual = getPrecedencia(caractere);
                
                while (idxTopo >= 0 && (pilhaOperadores[idxTopo][0] != '(') ) {
                    
                    int precTopo = getPrecedencia(pilhaOperadores[idxTopo][0]);
                    int associatividadeEsq = (precTopo >= precAtual) && (pilhaOperadores[idxTopo][0] != '^');
                    int associatividadeDir = (pilhaOperadores[idxTopo][0] == '^') && (precTopo > precAtual);
                    int ehTopoFuncao = ehFuncao(pilhaOperadores[idxTopo]);
                    
                    if (associatividadeEsq || associatividadeDir || ehTopoFuncao) {
                        strcat(resultadoPosFixa, pilhaOperadores[idxTopo]);
                        strcat(resultadoPosFixa, " ");
                        free(pilhaOperadores[idxTopo]);
                        idxTopo--;
                    } else {
                        break;
                    }
                }
                
                pilhaOperadores[++idxTopo] = strdup(opStr);
            }
            
            else if (caractere == '(') {
                pilhaOperadores[++idxTopo] = strdup("(");
            }
            else if (caractere == ')') {
                while (idxTopo >= 0 && strcmp(pilhaOperadores[idxTopo], "(") != 0) {
                    strcat(resultadoPosFixa, pilhaOperadores[idxTopo]);
                    strcat(resultadoPosFixa, " ");
                    free(pilhaOperadores[idxTopo]);
                    idxTopo--;
                }
                
                if (idxTopo < 0) {
                    printf("Erro: Parenteses desbalanceados.\n");
                    free(resultadoPosFixa);
                    return NULL;
                }
                free(pilhaOperadores[idxTopo]);
                idxTopo--;
            }

        } 
        
        else {
            elemento[indiceChar++] = caractere;
        }

        i++;
    }

    while (idxTopo >= 0) {
        if (strcmp(pilhaOperadores[idxTopo], "(") == 0) {
            printf("Erro: Parenteses desbalanceados.\n");
            free(resultadoPosFixa);
            return NULL;
        }
        
        strcat(resultadoPosFixa, pilhaOperadores[idxTopo]);
        strcat(resultadoPosFixa, " ");
        free(pilhaOperadores[idxTopo]);
        idxTopo--;
    }
    
    if (strlen(resultadoPosFixa) > 0) {
        resultadoPosFixa[strlen(resultadoPosFixa) - 1] = '\0';
    }

    return resultadoPosFixa;
}