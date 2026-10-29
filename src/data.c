#include <stdio.h>
#include "data.h"

int obterDataHora(
    char destino[],
    size_t tamanho
) {
    time_t agora = time(NULL);
    struct tm *data = localtime(&agora);

    if (data == NULL) {
        printf("Erro ao obter data e hora!\n");
        return 0;
    }
    size_t resultado = strftime(
        destino,
        tamanho,
        "%d/%m/%Y %H:%M",
        data
    );

    if (resultado == 0) {
        printf("Erro ao formatar data e hora!\n");
        return 0;
    }
    return 1;
}