#ifndef CARRINHO_H
#define CARRINHO_H

#include "produto.h"
#include "venda.h"

typedef struct {
    ItemVenda *itens;
    int total;
    int capacidade;
} Carrinho;

int inicializarCarrinho(Carrinho *carrinho);
void liberarCarrinho(Carrinho *carrinho);

void adicionarAoCarrinho(
    Carrinho *carrinho,
    Produto produtos[],
    int totalProdutos
);

#endif