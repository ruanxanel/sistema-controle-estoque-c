#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "estoque.h"
#include "arquivo.h"
#include "data.h"
#include "entrada.h"
#include "cores.h"

static void movimentarEstoque(Produto produtos[], int total, int entrada) {
    int codigo, quantidade;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    if (lerInteiro("Quantidade: ", &quantidade) != 1 || quantidade <= 0) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    Produto *p = &produtos[indice];

    if (entrada && quantidade > INT_MAX - p->quantidade) {
        printf(COR_VERMELHO "Limite do estoque excedido!\n" COR_RESET);
        return;
    }

    if (!entrada && quantidade > p->quantidade) {
        printf(COR_VERMELHO "Estoque insuficiente!\n" COR_RESET);
        return;
    }

    Movimentacao m;
    m.codigoProduto = codigo;
    strcpy(m.tipo, entrada ? "ENTRADA" : "SAIDA");
    m.quantidade = quantidade;

    if (!obterDataHora(m.dataHora, sizeof(m.dataHora)) ||
        !salvarMovimentacao(m)) {
        printf(COR_VERMELHO "Erro ao registrar movimentação!\n" COR_RESET);
        return;
    }

    if (entrada)
        p->quantidade += quantidade;
    else
        p->quantidade -= quantidade;

    printf(COR_VERDE "Movimentação registrada com sucesso!\n" COR_RESET);

    if (p->quantidade < p->estoqueMinimo) {
        printf(COR_AMARELO
            "ALERTA: %s abaixo do estoque mínimo! Atual: %d | Mínimo: %d\n"
            COR_RESET, p->nome, p->quantidade, p->estoqueMinimo);
    }
}

void registrarEntrada(Produto produtos[], int quantidadeProdutos) {
    movimentarEstoque(produtos, quantidadeProdutos, 1);
}

void registrarSaida(Produto produtos[], int quantidadeProdutos) {
    movimentarEstoque(produtos, quantidadeProdutos, 0);
}

void listarEstoqueBaixo(Produto produtos[], int total) {
    int encontrados = 0;

    printf(COR_AZUL "\n================ PRODUTOS COM ESTOQUE BAIXO ================\n" COR_RESET);
    printf("%-8s %-20s %10s %10s\n",
        "Código", "Produto", "Atual", "Mínimo");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < total; i++) {
        Produto p = produtos[i];

        if (p.quantidade < p.estoqueMinimo) {
            printf(COR_AMARELO "%-8d %-20.20s %10d %10d\n" COR_RESET,
                p.codigo, p.nome, p.quantidade, p.estoqueMinimo);
            encontrados++;
        }
    }

    printf("------------------------------------------------------------\n");

    if (encontrados == 0)
        printf(COR_VERDE "Nenhum produto com estoque baixo!\n" COR_RESET);
    else
        printf("Total de produtos com estoque baixo: %d\n", encontrados);
}