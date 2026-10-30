#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "entrada.h"

int lerInteiro(
    const char *mensagem,
    int *valor
) {
    char buffer[100];
    char *fim;
    errno = 0;

    printf("%s", mensagem);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    char *quebraLinha = strchr(buffer, '\n');

    if (quebraLinha == NULL) {
        int caractere;

        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            // Descarta os caracteres restantes
        }
        
        printf("Entrada muito longa ou sem quebra de linha!\n");
        return 0;
    }
    

    long numero = strtol(
        buffer,
        &fim,
        10
    );

    if (fim == buffer) {
        printf("Nenhum número encontrado!\n");
        return 0;
    }
    
    if (errno == ERANGE) {
        printf("Número fora do alcance de long\n");
        return 0;
    }
    
    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (*fim != '\0') {
        printf("Entrada inválida! Digite apenas números inteiros.\n");
        return 0;
    }

    if (numero < INT_MIN || numero > INT_MAX) {
        printf("Número fora dos limites de int\n");
        return 0;
    }
    
    *valor = (int) numero;
    return 1;
}


int lerTexto(const char *mensagem, char destino[], size_t tamanho) {
    if (tamanho < 2 || destino == NULL) {
        printf("Espaco insuficiente ou destino invalido!\n");
        return 0;
    }

    printf("%s", mensagem);

    if (fgets(destino, tamanho, stdin) == NULL) {
        return -1;
    }

    char *quebraLinha = strchr(destino, '\n');

    if (quebraLinha == NULL) {
        int caractere;

        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            // Descarta os caracteres restantes
        }

        printf("Texto muito longo ou sem quebra de linha!\n");
        return 0;
    }

    *quebraLinha = '\0';

    int possuiTexto = 0;

    for (size_t i = 0; destino[i] != '\0'; i++) {
        if (!isspace((unsigned char) destino[i])) {
            possuiTexto = 1;
            break;
        }
    }

    if (!possuiTexto) {
        printf("O texto nao pode estar vazio!\n");
        return 0;
    }

    return 1;
}


int lerDouble(const char *mensagem, double *valor) {
    char buffer[100];
    char *fim;

    printf("%s", mensagem);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    char *quebraLinha = strchr(buffer, '\n');

    if (quebraLinha == NULL) {
        int caractere;

        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            // Descarta os caracteres restantes
        }

        printf("Entrada muito longa!\n");
        return 0;
    }

    errno = 0;
    double numero = strtod(buffer, &fim);

    if (fim == buffer) {
        printf("Nenhum numero encontrado!\n");
        return 0;
    }

    if (errno == ERANGE) {
        printf("Numero fora do intervalo permitido!\n");
        return 0;
    }

    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (*fim != '\0') {
        printf("Entrada invalida! Digite apenas numeros decimais.\n");
        return 0;
    }

    if (!isfinite(numero)) {
        printf("Numero invalido!\n");
        return 0;
    }

    *valor = numero;
    return 1;
}