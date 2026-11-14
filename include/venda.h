#ifndef VENDA_H
#define VENDA_H

#include "vendedor.h"

typedef struct {
    int codigoProduto;
    int quantidade;
    double precoUnitario;
} ItemVenda;

typedef struct {
    int codigo;
    int codigoVendedor;
    int codigoCliente;
    int quantidadeItens;
    double valorTotal;
    double comissao;
    char dataHora[20];
} Venda;

typedef struct {
    int codigoVenda;
    ItemVenda item;
} ItemVendaRegistrado;

double calcularComissao(int quantidadeItens, double valorTotal);

void listarVendasVendedor(
    Vendedor vendedores[],
    int totalVendedores
);

void listarComissoesVendedor(
    Vendedor vendedores[],
    int totalVendedores
);

#endif