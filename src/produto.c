#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "produto.h"

static void exibirProduto(Produto produto) {
    printf("Codigo: %d\n", produto.codigo);
    printf("Nome: %s\n", produto.nome);
    printf("Preço: R$ %.2f\n", produto.preco);
    printf("Quantidade: %d\n", produto.quantidade);
    printf("Estoque minimo: %d\n", produto.estoqueMinimo);
}
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
        exibirProduto(produtos[i]);
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
    exibirProduto(produtos[indice]);
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

void excluirProduto(
    Produto produtos[],
    int *quantidadeProdutos
) {
    int codigoBuscador;

    printf("Digite o codigo do produto que deseja excluir: ");
    scanf("%d", &codigoBuscador);

    int indice = localizarProdutoPorCodigo(
        produtos,
        *quantidadeProdutos,
        codigoBuscador
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    for (int i = indice; i < *quantidadeProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    (*quantidadeProdutos)--;
    
    printf("Produto excluído com sucesso!\n");
}
