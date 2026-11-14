#include "venda.h"

double calcularComissao(int quantidadeItens, double valorTotal) {
    if (quantidadeItens <= 0 || valorTotal <= 0)
        return 0;

    if (quantidadeItens <= 10)
        return valorTotal * 0.05;

    return valorTotal * 0.10;
}