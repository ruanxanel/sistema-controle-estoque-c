#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "estoque.h"
#include "arquivo.h"
#include "data.h"
#include "entrada.h"

void registrarEntrada(
    Produto produtos[],
    int quantidadeProdutos
) {

    Movimentacao movimentacao;
    int codigoBuscado;
    int novaQuantidade;

    if (lerInteiro("Digite o codigo do produto: ", &codigoBuscado) != 1) {
        printf("Código inválido!\n");
        return;
    }
    
    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscado
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }
    
    if (lerInteiro("Digite a quantidade para adicionar: ", &novaQuantidade) != 1) {
        printf("Quantidade inválida!\n");
        return;
    }
    
    if (novaQuantidade <= 0) {
        printf("A entrada tem que ser maior que zero\n");
        return;
    }
    
    if (novaQuantidade > INT_MAX - produtos[indice].quantidade) {
        printf("Erro: a quantidade ultrapassa o limite permitido!\n");
        return;
    }

    movimentacao.codigoProduto = codigoBuscado;
    strcpy(movimentacao.tipo, "ENTRADA");
    movimentacao.quantidade = novaQuantidade;
    
    if (!obterDataHora(
        movimentacao.dataHora,
        sizeof(movimentacao.dataHora)
    )) {
        printf("Erro ao obter data e hora!\n");
        return;
    }

    if(!salvarMovimentacao(movimentacao)) {
        printf("Não foi possível registrar a movimentação!\n");
        return;
    }

    produtos[indice].quantidade += novaQuantidade;


    printf("Quantidade adicionada\n");
}

void registrarSaida(
    Produto produtos[],
    int quantidadeProdutos
) {
    int codigoBuscado;
    int novaQuantidade;
    Movimentacao movimentacao;

    if (lerInteiro("Digite o codigo do produto: ", &codigoBuscado) != 1) {
        printf("Código inválido!\n");
        return;
    }

    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscado
    );

    if (indice == -1) {
        printf("Produto não encontrado!!\n");
        return;
    }

    if (lerInteiro("Digite a quantidade para saída: ", &novaQuantidade) != 1) {
        printf("Quantidade inválida!\n");
        return;
    }

    if (novaQuantidade <= 0) {
        printf("A quantidade deve ser acima de 0\n");
        return;
    }

    if (novaQuantidade > produtos[indice].quantidade) {
        printf("Estoque insuficiente! Disponível: %d\n",
            produtos[indice].quantidade);
        return;
    }
    
    
    movimentacao.codigoProduto = codigoBuscado;
    strcpy(movimentacao.tipo, "SAIDA");
    movimentacao.quantidade = novaQuantidade;

    if (!obterDataHora(
        movimentacao.dataHora,
        sizeof(movimentacao.dataHora)
    )) {
        printf("Erro ao obter data e hora!\n");
        return;
    }
    
    if (!salvarMovimentacao(movimentacao)) {
        printf("Não foi possível registrar a movimentação!\n");
        return;
    }

    produtos[indice].quantidade -= novaQuantidade;
    
    printf("Saída registrada com sucesso!\n");

    if (produtos[indice].quantidade < produtos[indice].estoqueMinimo) {
        printf("ALERTA: Produto %s está abaixo do estoque mínimo!\n Quantidade atual: %d\n Estoque mínimo: %d\n",
        produtos[indice].nome,
        produtos[indice].quantidade,
        produtos[indice].estoqueMinimo
        );
    }
}

void listarEstoqueBaixo(
    Produto produtos[],
    int quantidadeProdutos
) {
    int encontrouEstoqueBaixo = 0;
    for (int i = 0; i < quantidadeProdutos; i++) {
        if (produtos[i].quantidade < produtos[i].estoqueMinimo) {
            printf(
                "Produto: %s | Quantidade atual: %d | Estoque mínimo: %d | situação: Estoque baixo\n",
                produtos[i].nome,
                produtos[i].quantidade,
                produtos[i].estoqueMinimo
            );
            encontrouEstoqueBaixo = 1;
        }
    }
    if (encontrouEstoqueBaixo == 0) {
        printf("Nenhum produto com estoque baixo\n");
    }
    
}