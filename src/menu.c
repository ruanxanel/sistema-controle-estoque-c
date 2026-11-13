#include <stdio.h>

#include "menu.h"
#include "produto.h"
#include "vendedor.h"
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

    printf(COR_AZUL);
    printf("============================================\n");

    if (perfil == 0)
        printf("       CONTROLE DE ESTOQUE E VENDAS\n");
    else if (perfil == 1)
        printf("             PAINEL ADMINISTRADOR\n");
    else
        printf("                PAINEL USUÁRIO\n");

    printf("============================================\n" COR_RESET);

    if (perfil == 0) {
        printf("\n[1] Administrador\n");
        printf("[2] Usuário\n");
        printf("[0] Sair\n");
    } else if (perfil == 1) {
        printf("\n[1] Cadastrar produto\n");
        printf("[2] Listar produtos\n");
        printf("[3] Buscar produto\n");
        printf("[4] Editar produto\n");
        printf("[5] Excluir produto\n");

        printf("\n[6] Registrar entrada\n");
        printf("[7] Registrar saída\n");
        printf("[8] Produtos com estoque baixo\n");
        printf("[9] Histórico de movimentações\n");

        printf("\n[10] Cadastrar vendedor\n");
        printf("[11] Listar vendedores\n");

        printf("\n[0] Voltar ao menu principal\n");
    } else {
        printf("\n[1] Listar produtos\n");
        printf("[2] Buscar produto\n");
        printf("[3] Registrar entrada\n");
        printf("[4] Registrar saída\n");
        printf("[5] Produtos com estoque baixo\n");
        printf("[6] Histórico de movimentações\n");
        printf("\n[0] Voltar ao menu principal\n");
    }

    printf("\n--------------------------------------------\n");
}

static void executarOpcao(
    int perfil,
    int opcao,
    Produto **produtos,
    int *totalProdutos,
    int *capacidadeProdutos,
    Vendedor **vendedores,
    int *totalVendedores,
    int *capacidadeVendedores
) {
    if (perfil == 1) {
        switch (opcao) {
            case 1:
                cadastrarProduto(produtos, totalProdutos, capacidadeProdutos);
                break;
            case 2:
                listarProdutos(*produtos, *totalProdutos);
                break;
            case 3:
                buscarProduto(*produtos, *totalProdutos);
                break;
            case 4:
                editarProduto(*produtos, *totalProdutos);
                break;
            case 5:
                excluirProduto(*produtos, totalProdutos);
                break;
            case 6:
                registrarEntrada(*produtos, *totalProdutos);
                break;
            case 7:
                registrarSaida(*produtos, *totalProdutos);
                break;
            case 8:
                listarEstoqueBaixo(*produtos, *totalProdutos);
                break;
            case 9:
                listarMovimentacoes();
                break;
            case 10:
                cadastrarVendedor(vendedores, totalVendedores, capacidadeVendedores);
                break;
            case 11:
                listarVendedores(*vendedores, *totalVendedores);
                break;
            default:
                printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }
    } else {
        switch (opcao) {
            case 1:
                listarProdutos(*produtos, *totalProdutos);
                break;
            case 2:
                buscarProduto(*produtos, *totalProdutos);
                break;
            case 3:
                registrarEntrada(*produtos, *totalProdutos);
                break;
            case 4:
                registrarSaida(*produtos, *totalProdutos);
                break;
            case 5:
                listarEstoqueBaixo(*produtos, *totalProdutos);
                break;
            case 6:
                listarMovimentacoes();
                break;
            default:
                printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
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
    int *totalProdutos,
    int *capacidadeProdutos,
    Vendedor **vendedores,
    int *totalVendedores,
    int *capacidadeVendedores
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
            totalProdutos,
            capacidadeProdutos,
            vendedores,
            totalVendedores,
            capacidadeVendedores
        );

    } while (escolha != 0);

    printf(COR_VERDE "Voltando ao menu principal...\n" COR_RESET);
    return 1;
}
