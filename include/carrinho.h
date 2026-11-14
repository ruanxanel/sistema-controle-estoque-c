#ifndef CARRINHO_H
#define CARRINHO_H

#include "produto.h"
#include "venda.h"
#include "vendedor.h"
#include "cliente.h"

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

void editarQuantidadeCarrinho(
    Carrinho *carrinho,
    Produto produtos[],
    int totalProdutos
);

void removerDoCarrinho(Carrinho *carrinho);

void limparCarrinho(Carrinho *carrinho);

void visualizarCarrinho(
    Carrinho *carrinho,
    Produto produtos[],
    int totalProdutos
);

int finalizarCompra(
    Carrinho *carrinho,
    Produto produtos[],
    int totalProdutos,
    Vendedor vendedores[],
    int totalVendedores,
    Cliente clientes[],
    int totalClientes
);

#endif