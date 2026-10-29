#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "produto.h"

void registrarEntrada(
    Produto produtos[],
    int quantidadeProdutos
);
void registrarSaida(
    Produto produtos[],
    int quantidadeProdutos
);

void listarEstoqueBaixo(
    Produto produtos[],
    int quantidadeProdutos
);

#endif