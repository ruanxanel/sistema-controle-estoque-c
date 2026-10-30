#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

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