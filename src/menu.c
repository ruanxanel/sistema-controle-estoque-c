#include <stdio.h>

#include "menu.h"
#include "produto.h"
#include "estoque.h"
#include "arquivo.h"
#include "entrada.h"
#include "cores.h"

static void limparTela(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

static void pausarTela(void) {
    int c;
    printf("\nPressione ENTER para continuar...");
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void exibirMenu(int perfil) {
    limparTela();

    if (perfil == 0) {
        printf(COR_AZUL "\n====== SISTEMA DE CONTROLE DE ESTOQUE ======\n\n" COR_RESET);
        printf("1 - Administrador\n2 - Usuário\n0 - Sair\n");
    } else {
        printf(COR_AZUL "\n========== PAINEL %s ==========\n" COR_RESET,
            perfil == 1 ? "ADMINISTRADOR" : "USUÁRIO");

        if (perfil == 1) {
            printf("1 - Cadastrar produto\n");
            printf("2 - Listar produtos\n");
            printf("3 - Buscar produto\n");
            printf("4 - Editar produto\n");
            printf("5 - Excluir produto\n");
            printf("6 - Registrar entrada\n");
            printf("7 - Registrar saída\n");
            printf("8 - Produtos com estoque baixo\n");
            printf("9 - Histórico de movimentações\n");
        } else {
            printf("1 - Listar produtos\n");
            printf("2 - Buscar produto\n");
            printf("3 - Registrar entrada\n");
            printf("4 - Registrar saída\n");
            printf("5 - Produtos com estoque baixo\n");
            printf("6 - Histórico de movimentações\n");
        }

        printf("0 - Voltar ao menu principal\n");
    }

    printf("\n");
}

static void executarOpcao(
    int perfil,
    int opcao,
    Produto **produtos,
    int *total,
    int *capacidade
) {
    if (perfil == 1) {
        switch (opcao) {
            case 1: cadastrarProduto(produtos, total, capacidade); break;
            case 2: listarProdutos(*produtos, *total); break;
            case 3: buscarProduto(*produtos, *total); break;
            case 4: editarProduto(*produtos, *total); break;
            case 5: excluirProduto(*produtos, total); break;
            case 6: registrarEntrada(*produtos, *total); break;
            case 7: registrarSaida(*produtos, *total); break;
            case 8: listarEstoqueBaixo(*produtos, *total); break;
            case 9: listarMovimentacoes(); break;
            default: printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }
    } else {
        switch (opcao) {
            case 1: listarProdutos(*produtos, *total); break;
            case 2: buscarProduto(*produtos, *total); break;
            case 3: registrarEntrada(*produtos, *total); break;
            case 4: registrarSaida(*produtos, *total); break;
            case 5: listarEstoqueBaixo(*produtos, *total); break;
            case 6: listarMovimentacoes(); break;
            default: printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }
    }

    pausarTela();
}

void menuPrincipal(void) {
    exibirMenu(0);
}

int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    if (opcao == 0) {
        printf(COR_VERDE "Saindo...\n" COR_RESET);
        return 1;
    }

    if (opcao != 1 && opcao != 2) {
        printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        return 1;
    }

    int escolha = -1;

    do {
        exibirMenu(opcao);

        int resultado = lerInteiro("Escolha uma opção: ", &escolha);

        if (resultado == -1) return 0;
        if (resultado == 0) continue;
        if (escolha == 0) break;

        executarOpcao(
            opcao,
            escolha,
            produtos,
            quantidadeProdutos,
            capacidade
        );

    } while (escolha != 0);

    printf(COR_VERDE "Voltando ao menu principal...\n" COR_RESET);
    return 1;
}