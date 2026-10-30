#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "estoque.h"

int salvarMovimentacao(
    Movimentacao movimentacao
);

void listarMovimentacoes(
    void
);

int salvarProdutos(
    Produto produtos[],
    int quantidadeProdutos
);

int carregarProdutos(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

#endif