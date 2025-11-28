#include <stdio.h>
#include <stdlib.h>
#include "expressao.h"
#include <math.h>
#include <string.h>

void testarExpressao(char *posFixa, char *inFixa, float valorEsperado, int testeNum) {
    char *resultadoPosFixa = NULL;
    char *resultadoInFixa = NULL;
    float valorCalculado = NAN;
    
    char posFixa_entrada[512];
    strcpy(posFixa_entrada, posFixa);

    printf("--- Teste %d ---\n", testeNum);
    printf("Infixa de Entrada: %s\n", inFixa);
    printf("Pos-Fixa de Entrada: %s\n", posFixa);
    printf("Valor Esperado: %.4f\n", valorEsperado);
    
    // 1. Teste de Infixa -> PosFixa
    resultadoPosFixa = getFormaPosFixa(inFixa);
    printf("Pos-Fixa Calculada (In->Pos): %s\n", resultadoPosFixa ? resultadoPosFixa : "Erro na conversao.");

    // 2. Teste de PosFixa -> Infixa
    resultadoInFixa = getFormaInFixa(posFixa_entrada);
    printf("Infixa Calculada (Pos->In): %s\n", resultadoInFixa ? resultadoInFixa : "Erro na conversao.");

    // 3. Teste de Avaliação
    valorCalculado = getValorPosFixa(posFixa_entrada);
    
    if (!isnan(valorCalculado))
        printf("Resultado Calculado: %.4f\n", valorCalculado);
    else
        printf("Resultado Calculado: Erro de avaliacao.\n");

    printf("-----------------\n\n");

    if(resultadoPosFixa) free(resultadoPosFixa);
    if(resultadoInFixa) free(resultadoInFixa);
}

int main(){
    
    // Teste Notação Posfixa Notação Infixa Valor
    // 1 3 4 + 5 * (3 + 4) * 5 35
    testarExpressao("3 4 + 5 *", "(3 + 4) * 5", 35.0, 1);
    
    // 2 7 2 * 4 + 7 * 2 + 4 18
    testarExpressao("7 2 * 4 +", "7 * 2 + 4", 18.0, 2);
    
    // 3 8 5 2 4 + * + 8 + (5 * (2 + 4)) 38
    testarExpressao("8 5 2 4 + * +", "8 + 5 * (2 + 4)", 38.0, 3);
    
    // 4 6 2 / 3 + 4 * (6 / 2 + 3) * 4 24
    testarExpressao("6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", 24.0, 4);
    
    // 5 9 5 2 8 * 4 + * + 9 + (5 * (4 + 8 * 2)) 109
    testarExpressao("9 5 2 8 * 4 + * +", "9 + 5 * (4 + 8 * 2)", 109.0, 5);

    // 6 2 3 + log 5 / log(2 + 3) / 5 Aprox. 0.14
    testarExpressao("2 3 + log 5 /", "log(2 + 3) / 5", 0.1398, 6); // log10(5)/5 = 0.69897/5

    // 7 10 log 3 ^ 2 + (log10)^3 + 2 3
    testarExpressao("10 log 3 ^ 2 +", "log(10) ^ 3 + 2", 3.0, 7); // log10(10)=1 -> 1^3 + 2 = 3

    // 8 45 60 + 30 cos * (45 + 60) * cos(30) Aprox. 90,93
    testarExpressao("45 60 + 30 cos *", "(45 + 60) * cos(30)", 90.9327, 8); // 105 * cos(30) ~ 105 * 0.866

    // 9 0.5 45 sen 2 ^ + sen(45) ^2 + 0,5 1
    testarExpressao("0.5 45 sen 2 ^ +", "0.5 + sen(45) ^ 2", 1.0, 9); // sen(45)^2 + 0.5 = (sqrt(2)/2)^2 + 0.5 = 0.5 + 0.5 = 1

    // Teste adicional com raiz
    testarExpressao("9 raiz 5 * 10 +", "raiz(9) * 5 + 10", 25.0, 10); // 3 * 5 + 10 = 25
    
    return 0;
}