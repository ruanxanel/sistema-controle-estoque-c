#ifndef ENTRADA_H
#define ENTRADA_H

#include <stddef.h>

int lerInteiro(const char *mensagem, int *valor);

int lerTexto(const char *mensagem, char destino[], size_t tamanho);

int lerDouble(const char *mensagem, double *valor);

#endif