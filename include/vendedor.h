#ifndef VENDEDOR_H
#define VENDEDOR_H

typedef struct {
    int codigo;
    char nome[100];
    double percentualComissao;
} Vendedor;

void cadastrarVendedor(Vendedor **vendedores, int *total, int *capacidade);

void listarVendedores(Vendedor vendedores[], int total);

int localizarVendedorPorCodigo(Vendedor vendedores[], int total, int codigo);

#endif