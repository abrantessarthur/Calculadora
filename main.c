#include <stdio.h>
#include <stdlib.h>
#include "expressao.h"
#include <math.h> 

int main(){
    float conta = getValorPosFixa("3 4 + 5 *");
    char *in = getFormaInFixa("3 4 + 5 *");
    
    printf("--- Teste Simples ---\n");
    printf("Expressao Pos-Fixa: 3 4 + 5 *\n");
    
    
    if (!isnan(conta)) 
        printf("Resultado: %.2f\n", conta);
    else 
        printf("Resultado: Erro de avaliacao.\n");

    if (in) {
        printf("Infixa: %s\n", in);
        free(in);
    } else {
        printf("Infixa: Erro na conversao.\n");
    }

    printf("\n--- Teste Avancado ---\n");
    float conta2 = getValorPosFixa("90 sen 5 ^ log");
    char *in2 = getFormaInFixa("90 sen 5 ^ log");

    printf("Expressao Pos-Fixa: 90 sen 5 ^ log\n");

    
    if (!isnan(conta2)) 
        printf("Resultado: %.4f\n", conta2);
    else 
        printf("Resultado: Erro de avaliacao.\n");

    if (in2) {
        printf("Infixa: %s\n", in2);
        free(in2);
    } else {
        printf("Infixa: Erro na conversao.\n");
    }

    return 0;
}