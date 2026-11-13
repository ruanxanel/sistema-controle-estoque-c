#ifndef PRODUTO_H
#define PRODUTO_H

typedef struct produto {
    int codigo;
    char nome[100];
    char categoria[50];
    double preco;
    int quantidade;
    int estoqueMinimo;
} Produto;

void cadastrarProduto(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

void listarProdutos(
    Produto produtos[],
    int quantidadeProdutos
);

int localizarProdutoPorCodigo(
    Produto produtos[],
    int quantidadeProdutos,
    int codigo
);

void buscarProduto(
    Produto produtos[],
    int quantidadeProdutos
);

void editarProduto(
    Produto produtos[],
    int quantidadeProdutos
);

void excluirProduto(
    Produto produtos[],
    int *quantidadeProdutos
);

#endif
