#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "vendedor.h"
#include "entrada.h"
#include "cores.h"

int localizarVendedorPorCodigo(Vendedor vendedores[], int total, int codigo) {
    for (int i = 0; i < total; i++) {
        if (vendedores[i].codigo == codigo)
            return i;
    }

    return -1;
}

void listarVendedores(Vendedor vendedores[], int total) {
    if (total == 0) {
        printf(COR_AMARELO "Nenhum vendedor cadastrado!\n" COR_RESET);
        return;
    }

    printf(COR_AZUL "\n================================================\n");
    printf("             VENDEDORES CADASTRADOS\n");
    printf("================================================\n" COR_RESET);

    printf("%-8s %-20s %12s\n", "Código", "Nome", "Comissão");
    printf("------------------------------------------------\n");

    for (int i = 0; i < total; i++) {
        Vendedor v = vendedores[i];

        printf("%-8d %-20.20s %11.2f%%\n",
            v.codigo, v.nome, v.percentualComissao);
    }

    printf("------------------------------------------------\n");
    printf("Total de vendedores: %d\n", total);
}

void cadastrarVendedor(Vendedor **vendedores, int *total, int *capacidade) {
    Vendedor v;

    if (lerInteiro("Código do vendedor: ", &v.codigo) != 1 || v.codigo <= 0) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    if (localizarVendedorPorCodigo(*vendedores, *total, v.codigo) != -1) {
        printf(COR_VERMELHO "Vendedor já cadastrado!\n" COR_RESET);
        return;
    }

    if (lerTexto("Nome do vendedor: ", v.nome, sizeof(v.nome)) != 1) {
        printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
        return;
    }

    if (lerDouble("Comissão (%): ", &v.percentualComissao) != 1 ||
        v.percentualComissao < 0 || v.percentualComissao > 100) {
        printf(COR_VERMELHO "Comissão inválida! Informe de 0 a 100.\n" COR_RESET);
        return;
    }

    if (*total == *capacidade) {
        if (*capacidade <= 0 || *capacidade > INT_MAX / 2) {
            printf(COR_VERMELHO "Limite de vendedores atingido!\n" COR_RESET);
            return;
        }

        int novaCapacidade = *capacidade * 2;

        Vendedor *temp = realloc(
            *vendedores,
            (size_t)novaCapacidade * sizeof(Vendedor)
        );

        if (temp == NULL) {
            printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
            return;
        }

        *vendedores = temp;
        *capacidade = novaCapacidade;
    }

    (*vendedores)[(*total)++] = v;

    printf(COR_VERDE "Vendedor %s cadastrado com sucesso!\n" COR_RESET,
        v.nome);
}