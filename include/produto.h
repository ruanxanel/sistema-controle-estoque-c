
#ifndef PRODUTO_H
#define PRODUTO_H

// Proteção contra inclusão múltipla.
// Impede que as definições deste arquivo sejam processadas
// mais de uma vez durante a compilação.

// Define a estrutura que representa um produto.
// Uma struct agrupa diferentes informações em um único tipo.
//
// Em Java, podemos comparar essa estrutura com uma classe Produto,
// mas a struct em C não possui métodos associados diretamente.
typedef struct produto {
    int codigo;          // Identificador do produto
    char nome[100];      // Vetor de caracteres que armazena o nome
    double preco;        // Preço do produto
    int quantidade;      // Quantidade disponível em estoque
    int estoqueMinimo;   // Quantidade mínima desejada
} Produto;

// Cadastra um novo produto.
//
// Produto **produto: ponteiro duplo que permite modificar
// o endereço do vetor durante uma realocação.
//
// int *quantidadesProdutos: permite aumentar o contador.
//
// int *capacidade: permite atualizar a capacidade do vetor.
void cadastrarProduto(
    Produto **produto,
    int *quantidadesProdutos,
    int *capacidade
);

// Percorre o vetor e exibe os produtos cadastrados.
void listarProdutos(
    Produto produtos[],
    int quantidadeProdutos
);

// Procura um produto pelo código.
// Retorna o índice encontrado ou -1 caso não exista.
int localizarProdutoPorCodigo(
    Produto produtos[],
    int quantidadeProdutos,
    int codigo
);

// Localiza um produto e mostra suas informações.
void buscarProduto(
    Produto produtos[],
    int quantidadeProdutos
);

// Permite alterar nome, preço e estoque mínimo.
void editarProduto(
    Produto produtos[],
    int quantidadeProdutos
);

// Exclui um produto e atualiza a quantidade cadastrada.
// O ponteiro permite modificar o contador original.
void excluirProduto(
    Produto produtos[],
    int *quantidadeProdutos
);

#endif