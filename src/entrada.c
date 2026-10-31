#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "entrada.h"

static int lerLinha(const char *mensagem, char buffer[], size_t tamanho) {
    printf("%s", mensagem);

    if (fgets(buffer, tamanho, stdin) == NULL)
        return -1;

    char *fim = strchr(buffer, '\n');

    if (fim == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Entrada muito longa!\n");
        return 0;
    }

    *fim = '\0';
    return 1;
}

int lerInteiro(const char *mensagem, int *valor) {
    char buffer[100], *fim;
    int resultado = lerLinha(mensagem, buffer, sizeof(buffer));

    if (resultado != 1) return resultado;

    errno = 0;
    long numero = strtol(buffer, &fim, 10);

    while (isspace((unsigned char)*fim)) fim++;

    if (fim == buffer || *fim != '\0' || errno == ERANGE ||
        numero < INT_MIN || numero > INT_MAX) {
        printf("Número inteiro inválido!\n");
        return 0;
    }

    *valor = (int)numero;
    return 1;
}

int lerTexto(const char *mensagem, char destino[], size_t tamanho) {
    if (destino == NULL || tamanho < 2) {
        printf("Destino inválido!\n");
        return 0;
    }

    int resultado = lerLinha(mensagem, destino, tamanho);

    if (resultado != 1) return resultado;

    for (size_t i = 0; destino[i] != '\0'; i++) {
        if (!isspace((unsigned char)destino[i]))
            return 1;
    }

    printf("O texto não pode estar vazio!\n");
    return 0;
}

int lerDouble(const char *mensagem, double *valor) {
    char buffer[100], *fim;
    int resultado = lerLinha(mensagem, buffer, sizeof(buffer));

    if (resultado != 1) return resultado;

    errno = 0;
    double numero = strtod(buffer, &fim);

    while (isspace((unsigned char)*fim)) fim++;

    if (fim == buffer || *fim != '\0' || errno == ERANGE || !isfinite(numero)) {
        printf("Número decimal inválido!\n");
        return 0;
    }

    *valor = numero;
    return 1;
}