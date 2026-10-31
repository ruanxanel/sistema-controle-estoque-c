
#ifndef ESTOQUE_H
#define ESTOQUE_H

// Evita que o conteúdo deste arquivo seja incluído mais de uma vez.
// Importa a definição da struct Produto.
#include "produto.h"

// Estrutura que representa uma movimentação de estoque.
//
// Cada movimentação armazena qual produto foi movimentado,
// o tipo da operação, a quantidade e a data/hora.
//
// Em Java, seria semelhante a uma classe Movimentacao
// contendo esses quatro atributos.
typedef struct movimentacao {
    int codigoProduto;  // Código do produto movimentado
    char tipo[10];      // "ENTRADA" ou "SAIDA"
    int quantidade;     // Quantidade movimentada
    char dataHora[20];  // Data e hora da operação
} Movimentacao;

// Registra a entrada de unidades de um produto.
//
// Recebe o vetor de produtos e a quantidade cadastrada.
// A entrada aumenta a quantidade do produto existente.
void registrarEntrada(
    Produto produtos[],
    int quantidadeProdutos
);

// Registra a saída de unidades de um produto.
//
// Antes de diminuir o estoque, o sistema verifica
// se existe quantidade suficiente para a operação.
void registrarSaida(
    Produto produtos[],
    int quantidadeProdutos
);

// Percorre o vetor e mostra os produtos cuja quantidade
// está abaixo do estoque mínimo configurado.
void listarEstoqueBaixo(
    Produto produtos[],
    int quantidadeProdutos
);

#endif
