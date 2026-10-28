#include "produto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cadastrarProduto(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    Produto produto;
    
    printf("Digite o codigo do produto: ");
    scanf("%d", &produto.codigo);

    int indice = localizarProdutoPorCodigo(
        *produtos,
        *quantidadeProdutos,
        produto.codigo
    );

    if (indice != -1) {
        printf("produto já cadastrado!\n");
        return;
    }

    printf("Digite o nome do produto: ");
    scanf("%99s", produto.nome);

    printf("Digite o preço: ");
    scanf("%lf", &produto.preco);

    printf("Digite a quantidade: ");
    scanf("%d", &produto.quantidade);

    printf("Digite o estoque mínimo: ");
    scanf("%d", &produto.estoqueMinimo);

    if (*quantidadeProdutos == *capacidade) {
        int novaCapacidade = (*capacidade) * 2;
        Produto *temp = realloc(
            *produtos,
            novaCapacidade * sizeof(Produto)
        );
        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            return;
        }
        *produtos = temp;
        *capacidade = novaCapacidade;
    }
    

    (*produtos)[*quantidadeProdutos] = produto;
    (*quantidadeProdutos)++;
    printf("Produto %s cadastrado com sucesso\n", produto.nome);
}

void listarProdutos(
    Produto produtos[],
    int quantidadeProdutos
) {
    if (quantidadeProdutos == 0) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }
    for (int i = 0; i < quantidadeProdutos; i++) {
        printf("Codigo: %d\n", produtos[i].codigo);
        printf("Nome: %s\n", produtos[i].nome);
        printf("Preço: R$ %.2f\n", produtos[i].preco);
        printf("Quantidade: %d\n", produtos[i].quantidade);
        printf("Estoque minimo: %d\n", produtos[i].estoqueMinimo);
    }
}

void buscarProduto(
    Produto produtos[],
    int quantidadeProdutos
) {
    int codigoBuscador;
    
    printf("Digite o codigo do produto: ");
    scanf("%d", &codigoBuscador);
    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscador
    );
    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    } 
    printf("Codigo: %d\n", produtos[indice].codigo);
    printf("Nome: %s\n", produtos[indice].nome);
    printf("Preço: R$ %.2f\n", produtos[indice].preco);
    printf("Quantidade: %d\n", produtos[indice].quantidade);
    printf("Estoque minimo: %d\n", produtos[indice].estoqueMinimo);
}

int localizarProdutoPorCodigo(
    Produto produtos[],
    int quantidadeProdutos,
    int codigo
) {
    for (int i = 0; i < quantidadeProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

void editarProduto(
    Produto produtos[],
    int quantidadeProdutos
) {
    int codigoBuscador;
    char novoNome[100];
    double novoPreco;
    int novoEstoqueMinimo;

    printf("Digite o codigo do produto que deseja editar: \n");
    scanf("%d", &codigoBuscador);

    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscador
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    printf("Digite o novo nome do produto: ");
    scanf("%99s", novoNome);
    strcpy(produtos[indice].nome, novoNome);

    printf("Digite o novo preço do produto: ");
    scanf("%lf", &novoPreco);
    produtos[indice].preco = novoPreco;

    printf("Digite a nova quantidade minima: ");
    scanf("%d", &novoEstoqueMinimo);
    produtos[indice].estoqueMinimo = novoEstoqueMinimo;

    printf("Produto editado com sucesso\n");
}