#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "estoque.h"
#include "vendedor.h"
#include "cliente.h"
#include "venda.h"

int salvarMovimentacao(Movimentacao movimentacao);

void listarMovimentacoes(void);

int salvarProdutos(
    Produto produtos[],
    int quantidadeProdutos
);

int carregarProdutos(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

int salvarVendedores(Vendedor vendedores[], int total);

int carregarVendedores(
    Vendedor **vendedores,
    int *total,
    int *capacidade
);

int salvarClientes(Cliente clientes[], int total);

int carregarClientes(
    Cliente **clientes,
    int *total,
    int *capacidade
);

int salvarVenda(Venda venda);

int salvarItensVenda(
    int codigoVenda,
    ItemVenda itens[],
    int totalItens
);

int proximoCodigoVenda(int *codigo);

int registrarVendaArquivo(
    Venda venda,
    ItemVenda itens[],
    int totalItens
);

#endif