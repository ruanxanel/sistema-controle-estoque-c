
#ifndef ARQUIVO_H
#define ARQUIVO_H

// Impede que as declarações deste arquivo sejam incluídas
// mais de uma vez na mesma unidade de compilação.

// Importa a definição de Produto e Movimentacao.
// estoque.h já inclui produto.h.
#include "estoque.h"

// Salva uma movimentação no arquivo de histórico.
//
// Recebe uma estrutura Movimentacao por valor,
// ou seja, uma cópia da estrutura original.
//
// Retorna 1 se a gravação funcionar e 0 em caso de erro.
int salvarMovimentacao(
    Movimentacao movimentacao
);

// Lê o histórico armazenado no arquivo binário
// e exibe as movimentações em formato de tabela.
//
// void indica que a função não recebe parâmetros.
void listarMovimentacoes(
    void
);

// Salva o vetor de produtos no arquivo binário.
//
// produtos[]: vetor contendo os produtos cadastrados.
// quantidadeProdutos: número de posições utilizadas.
//
// Retorna 1 em caso de sucesso ou 0 em caso de erro.
int salvarProdutos(
    Produto produtos[],
    int quantidadeProdutos
);

// Carrega os produtos salvos anteriormente.
//
// Produto **produtos: permite alterar o ponteiro do vetor
// caso seja necessário aumentar sua capacidade.
//
// int *quantidadeProdutos: permite atualizar a quantidade
// de produtos carregados.
//
// int *capacidade: permite atualizar o espaço disponível.
//
// Retorna 1 quando a operação funciona e 0 em caso de erro.
int carregarProdutos(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

#endif
