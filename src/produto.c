#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "produto.h"
#include "entrada.h"
#include "cores.h"

static void exibirProduto(Produto p) {
    printf("\n--------------------------------------\n");
    printf("Código: %d\nNome: %s\nPreço: R$ %.2f\nQuantidade: %d\nEstoque mínimo: %d\n",
            p.codigo, p.nome, p.preco, p.quantidade, p.estoqueMinimo);
    printf("--------------------------------------\n");
}
static int lerPrecoValido(double *preco) {
    if (lerDouble("Digite o preço: ", preco) != 1 || *preco < 0) {
        printf(COR_VERMELHO "Preço inválido!\n" COR_RESET);
        return 0;
    }
    return 1;
}

int localizarProdutoPorCodigo(Produto produtos[], int total, int codigo) {
    for (int i = 0; i < total; i++) {
        if (produtos[i].codigo == codigo) return i;
    }
    return -1;
}

void cadastrarProduto(Produto **produtos, int *total, int *capacidade) {
    Produto p;

    if (lerInteiro("Código: ", &p.codigo) != 1 || p.codigo <= 0) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    if (localizarProdutoPorCodigo(*produtos, *total, p.codigo) != -1) {
        printf(COR_VERMELHO "Produto já cadastrado!\n" COR_RESET);
        return;
    }

    if (lerTexto("Nome: ", p.nome, sizeof(p.nome)) != 1 ||
        !lerPrecoValido(&p.preco) ||
        lerInteiro("Quantidade: ", &p.quantidade) != 1 ||
        p.quantidade < 0 ||
        lerInteiro("Estoque mínimo: ", &p.estoqueMinimo) != 1 ||
        p.estoqueMinimo < 0) {
        printf(COR_VERMELHO "Dados inválidos!\n" COR_RESET);
        return;
    }

    if (*total == *capacidade) {
        if (*capacidade > INT_MAX / 2) {
            printf(COR_VERMELHO "Limite de produtos atingido!\n" COR_RESET);
            return;
        }

        int novaCapacidade = *capacidade * 2;
        Produto *temp = realloc(*produtos, (size_t)novaCapacidade * sizeof(Produto));

        if (temp == NULL) {
            printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
            return;
        }

        *produtos = temp;
        *capacidade = novaCapacidade;
    }

    (*produtos)[(*total)++] = p;
    printf(COR_VERDE "Produto %s cadastrado com sucesso!\n" COR_RESET, p.nome);
}

void listarProdutos(Produto produtos[], int total) {
    if (total == 0) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }

    printf("\n========== PRODUTOS CADASTRADOS ==========\n");
    for (int i = 0; i < total; i++) exibirProduto(produtos[i]);
}

void buscarProduto(Produto produtos[], int total) {
    int codigo;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, total, codigo);

    if (indice == -1)
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
    else
        exibirProduto(produtos[indice]);
}

void editarProduto(Produto produtos[], int total) {
    int codigo, opcao = -1, minimo;
    char nome[100];
    double preco;

    printf("\n========== EDITAR PRODUTO ==========\n");

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    Produto *p = &produtos[indice];

    do {
        printf("\n========== OPÇÕES DE EDIÇÃO ==========\n");
        exibirProduto(*p);
        printf("\n1 - Alterar nome\n2 - Alterar preço\n");
        printf("3 - Alterar estoque mínimo\n4 - Alterar todos os dados\n0 - Voltar\n");

        int resultado = lerInteiro("Opção: ", &opcao);
        if (resultado == 0) continue;
        if (resultado == -1) return;

        switch (opcao) {
            case 1:
                if (lerTexto("Novo nome: ", nome, sizeof(nome)) == 1) {
                    strcpy(p->nome, nome);
                    printf(COR_VERDE "Nome atualizado!\n" COR_RESET);
                } else {
                    printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
                }
                break;

            case 2:
                if (lerPrecoValido(&preco)) {
                    p->preco = preco;
                    printf(COR_VERDE "Preço atualizado!\n" COR_RESET);
                }
                break;

            case 3:
                if (lerInteiro("Novo estoque mínimo: ", &minimo) == 1 && minimo >= 0) {
                    p->estoqueMinimo = minimo;
                    printf(COR_VERDE "Estoque mínimo atualizado!\n" COR_RESET);
                } else {
                    printf(COR_VERMELHO "Estoque mínimo inválido!\n" COR_RESET);
                }
                break;

            case 4:
                if (lerTexto("Novo nome: ", nome, sizeof(nome)) != 1 ||
                    !lerPrecoValido(&preco) ||
                    lerInteiro("Novo estoque mínimo: ", &minimo) != 1 ||
                    minimo < 0) {
                    printf(COR_VERMELHO "Dados inválidos!\n" COR_RESET);
                    break;
                }

                strcpy(p->nome, nome);
                p->preco = preco;
                p->estoqueMinimo = minimo;
                printf(COR_VERDE "Dados atualizados!\n" COR_RESET);
                break;

            case 0:
                break;

            default:
                printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }
    } while (opcao != 0);
}

void excluirProduto(Produto produtos[], int *total) {
    int codigo;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, *total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    for (int i = indice; i < *total - 1; i++)
        produtos[i] = produtos[i + 1];

    (*total)--;
    printf(COR_VERDE "Produto excluído com sucesso!\n" COR_RESET);
}
