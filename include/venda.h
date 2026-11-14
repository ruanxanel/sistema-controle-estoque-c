#ifndef VENDA_H
#define VENDA_H

typedef struct {
    int codigo;
    int codigoVendedor;
    int codigoCliente;
    int quantidadeItens;
    double valorTotal;
    double comissao;
    char dataHora[20];
} Venda;

double calcularComissao(int quantidadeItens, double valorTotal);

#endif