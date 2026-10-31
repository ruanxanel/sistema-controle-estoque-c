#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "estoque.h"
#include "arquivo.h"
#include "data.h"
#include "entrada.h"
#include "cores.h"

void registrarEntrada(
    Produto produtos[],
    int quantidadeProdutos
) {

    Movimentacao movimentacao;
    int codigoBuscado;
    int novaQuantidade;

    if (lerInteiro("Digite o codigo do produto: ", &codigoBuscado) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }
    
    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscado
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }
    
    if (lerInteiro("Digite a quantidade para adicionar: ", &novaQuantidade) != 1) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }
    
    if (novaQuantidade <= 0) {
        printf(COR_VERMELHO "A entrada tem que ser maior que zero\n" COR_RESET);
        return;
    }
    
    if (novaQuantidade > INT_MAX - produtos[indice].quantidade) {
        printf(COR_VERMELHO "Erro: a quantidade ultrapassa o limite permitido!\n" COR_RESET);
        return;
    }

    movimentacao.codigoProduto = codigoBuscado;
    strcpy(movimentacao.tipo, "ENTRADA");
    movimentacao.quantidade = novaQuantidade;
    
    if (!obterDataHora(
        movimentacao.dataHora,
        sizeof(movimentacao.dataHora)
    )) {
        printf(COR_VERMELHO "Erro ao obter data e hora!\n" COR_RESET);
        return;
    }

    if(!salvarMovimentacao(movimentacao)) {
        printf(COR_VERMELHO "Não foi possível registrar a movimentação!\n" COR_RESET);
        return;
    }

    produtos[indice].quantidade += novaQuantidade;


    printf(COR_VERDE "Quantidade adicionada com sucesso!\n" COR_RESET);
}

void registrarSaida(
    Produto produtos[],
    int quantidadeProdutos
) {
    int codigoBuscado;
    int novaQuantidade;
    Movimentacao movimentacao;

    if (lerInteiro("Digite o codigo do produto: ", &codigoBuscado) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscado
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    if (lerInteiro("Digite a quantidade para saída: ", &novaQuantidade) != 1) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    if (novaQuantidade <= 0) {
        printf(COR_VERMELHO "A quantidade deve ser maior que zero!\n" COR_RESET);
        return;
    }

    if (novaQuantidade > produtos[indice].quantidade) {
        printf(COR_VERMELHO "Estoque insuficiente! Disponível: %d\n" COR_RESET,
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
        printf(COR_VERMELHO "Erro ao obter data e hora!\n" COR_RESET);
        return;
    }
    
    if (!salvarMovimentacao(movimentacao)) {
        printf(COR_VERMELHO "Não foi possível registrar a movimentação!\n" COR_RESET);
        return;
    }

    produtos[indice].quantidade -= novaQuantidade;
    
    printf(COR_VERDE "Saída registrada com sucesso!\n" COR_RESET);

    if (produtos[indice].quantidade < produtos[indice].estoqueMinimo) {
        printf(
            COR_AMARELO
            "ALERTA: Produto %s está abaixo do estoque mínimo!\n"
            "Quantidade atual: %d\n"
            "Estoque mínimo: %d\n"
            COR_RESET,
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
                COR_AMARELO
                "Produto: %s | Quantidade atual: %d | Estoque mínimo: %d | situação: Estoque baixo\n"
                COR_AMARELO,
                produtos[i].nome,
                produtos[i].quantidade,
                produtos[i].estoqueMinimo
            );
            encontrouEstoqueBaixo = 1;
        }
    }
    if (encontrouEstoqueBaixo == 0) {
        printf(COR_VERDE "Nenhum produto com estoque baixo\n" COR_RESET);
    }
    
}