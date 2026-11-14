#include <stdio.h>
#include <stdlib.h>

#include "carrinho.h"
#include "entrada.h"
#include "produto.h"
#include "cores.h"

int inicializarCarrinho(Carrinho *carrinho) {
    carrinho->total = 0;
    carrinho->capacidade = 2;

    carrinho->itens = malloc(carrinho->capacidade * sizeof(ItemVenda));

    if (carrinho->itens == NULL) {
        printf(COR_VERMELHO "Erro ao criar carrinho!\n" COR_RESET);
        return 0;
    }

    return 1;
}

void liberarCarrinho(Carrinho *carrinho) {
    free(carrinho->itens);

    carrinho->itens = NULL;
    carrinho->total = 0;
    carrinho->capacidade = 0;
}

void adicionarAoCarrinho(Carrinho *carrinho, Produto produtos[], int totalProdutos) {
    int codigo, quantidade;

    listarProdutos(produtos, totalProdutos);

    if (totalProdutos == 0)
        return;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, totalProdutos, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    Produto *produto = &produtos[indice];

    if (lerInteiro("Quantidade: ", &quantidade) != 1 || quantidade <= 0) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    int indiceCarrinho = -1;

    for (int i = 0; i < carrinho->total; i++) {
        if (carrinho->itens[i].codigoProduto == codigo) {
            indiceCarrinho = i;
            break;
        }
    }

    int quantidadeAtual = 0;

    if (indiceCarrinho != -1)
        quantidadeAtual = carrinho->itens[indiceCarrinho].quantidade;

    if (quantidade > produto->quantidade - quantidadeAtual) {
        printf(COR_VERMELHO "Estoque insuficiente!\n" COR_RESET);
        return;
    }

    if (indiceCarrinho != -1) {
        carrinho->itens[indiceCarrinho].quantidade += quantidade;
    } else {
        if (carrinho->total == carrinho->capacidade) {
            int novaCapacidade = carrinho->capacidade * 2;

            ItemVenda *temp = realloc(
                carrinho->itens,
                novaCapacidade * sizeof(ItemVenda)
            );

            if (temp == NULL) {
                printf(COR_VERMELHO "Erro ao aumentar carrinho!\n" COR_RESET);
                return;
            }

            carrinho->itens = temp;
            carrinho->capacidade = novaCapacidade;
        }

        ItemVenda item;

        item.codigoProduto = codigo;
        item.quantidade = quantidade;
        item.precoUnitario = produto->preco;

        carrinho->itens[carrinho->total++] = item;
    }

    printf(COR_VERDE "Produto adicionado ao carrinho!\n" COR_RESET);
}

void editarQuantidadeCarrinho(Carrinho *carrinho, Produto produtos[], int totalProdutos) {
    int codigo, novaQuantidade;

    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho está vazio!\n" COR_RESET);
        return;
    }

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indiceCarrinho = -1;

    for (int i = 0; i < carrinho->total; i++) {
        if (carrinho->itens[i].codigoProduto == codigo) {
            indiceCarrinho = i;
            break;
        }
    }

    if (indiceCarrinho == -1) {
        printf(COR_VERMELHO "Produto não está no carrinho!\n" COR_RESET);
        return;
    }

    int indiceProduto = localizarProdutoPorCodigo(produtos, totalProdutos, codigo);

    if (indiceProduto == -1) {
        printf(COR_VERMELHO "Produto não encontrado no estoque!\n" COR_RESET);
        return;
    }

    printf("Quantidade atual: %d\n", carrinho->itens[indiceCarrinho].quantidade);

    if (lerInteiro("Nova quantidade: ", &novaQuantidade) != 1 || novaQuantidade <= 0) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    if (novaQuantidade > produtos[indiceProduto].quantidade) {
        printf(COR_VERMELHO "Estoque insuficiente!\n" COR_RESET);
        return;
    }

    carrinho->itens[indiceCarrinho].quantidade = novaQuantidade;

    printf(COR_VERDE "Quantidade atualizada com sucesso!\n" COR_RESET);
}