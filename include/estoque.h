#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "produto.h"

typedef struct movimentacao {
    int codigoProduto;
    char tipo[10];
    int quantidade;
    char dataHora[20];
} Movimentacao;

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