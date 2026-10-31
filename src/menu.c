#include<stdio.h>

#include "menu.h"
#include "produto.h"
#include "estoque.h"
#include "arquivo.h"
#include "entrada.h"
#include "cores.h"

static void pausarTela(void) {
    int caractere;

    printf("\nPressione ENTER para continuar...");
    fflush(stdout);

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}
static void limparTela(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}



static void menuAdmin() {
    limparTela();

    printf(COR_AZUL "\n======================================\n");
    printf("         PAINEL ADMINISTRADOR         \n");
    printf("======================================\n\n" COR_RESET);

    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Buscar produto\n");
    printf("4 - Editar produto\n");
    printf("5 - Excluir produto\n");
    printf("6 - Registrar entrada\n");
    printf("7 - Registrar saída\n");
    printf("8 - Produtos com estoque baixo\n");
    printf("9 - Histórico de movimentações\n");
    printf("0 - Voltar ao menu principal\n\n");
}
static void lerOpcaoAdmin(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {

    switch (opcao) {
    case 1:
        cadastrarProduto(produtos, quantidadeProdutos, capacidade);
        break;
    case 2:
        listarProdutos(*produtos, *quantidadeProdutos);
        break;
    case 3:
        buscarProduto(*produtos, *quantidadeProdutos);
        break;
    case 4:
        editarProduto(*produtos, *quantidadeProdutos);
        break;
    case 5:
        excluirProduto(*produtos, quantidadeProdutos);
        break;
    case 6:
        registrarEntrada(*produtos, *quantidadeProdutos);
        break;
    case 7:
        registrarSaida(*produtos, *quantidadeProdutos);
        break;
    case 8:
        listarEstoqueBaixo(*produtos, *quantidadeProdutos);
        break;
    case 9:
        listarMovimentacoes();
        break;
    case 0:
        printf(COR_VERDE "Voltando ao menu principal...\n" COR_RESET);
        break;
    default:
        printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        break;
    }

    if (opcao != 0) {
        pausarTela();
    }
    
}



static void menuUsuario() {
    limparTela();

    printf(COR_AZUL "\n======================================\n");
    printf("           PAINEL USUÁRIO             \n");
    printf("======================================\n\n" COR_RESET);

    printf("1 - Listar produtos\n");
    printf("2 - Buscar produto\n");
    printf("3 - Registrar entrada\n");
    printf("4 - Registrar saída\n");
    printf("5 - Produtos com estoque baixo\n");
    printf("6 - Histórico de movimentações\n");
    printf("0 - Voltar ao menu principal\n\n");

    
}
static void lerOpcaoUsuario(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos
) {
    switch (opcao) {
    case 1:
        listarProdutos(*produtos, *quantidadeProdutos);
        break;
    case 2:
        buscarProduto(*produtos, *quantidadeProdutos);
        break;
    case 3:
        registrarEntrada(*produtos, *quantidadeProdutos);
        break;
    case 4:
        registrarSaida(*produtos, *quantidadeProdutos);
        break;
    case 5:
        listarEstoqueBaixo(*produtos, *quantidadeProdutos);
        break;
    case 6:
        listarMovimentacoes();
        break;
    case 0:
        printf(COR_VERDE "Voltando ao menu principal...\n" COR_RESET);
        break;
    default:
        printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        break;
    }

    if (opcao != 0) {
        pausarTela();
    }
}



void menuPrincipal(void) {
    limparTela();

    printf(COR_AZUL "\n======================================\n");
    printf("    SISTEMA DE CONTROLE DE ESTOQUE    \n");
    printf("======================================\n\n" COR_RESET);

    printf("1 - Administrador\n");
    printf("2 - Usuário\n");
    printf("0 - Sair\n\n");
}
int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    int opAdm = -1;
    int opUser = -1;
    switch (opcao) {
        case 1:
            do {
                menuAdmin();
                
                int resultado = lerInteiro("Escolha uma opção: ", &opAdm);

                if (resultado == 0) {
                    continue;
                }
                
                if (resultado == -1) {
                    return 0;
                }

                lerOpcaoAdmin(
                    opAdm, 
                    produtos, 
                    quantidadeProdutos, 
                    capacidade
                );

            } while (opAdm != 0);

            break;
        case 2:
            do {
                menuUsuario();
                
                int resultado = lerInteiro("Escolha uma opção: ", &opUser);

                if (resultado == 0) {
                    continue;
                }
                
                if (resultado == -1) {
                    return 0;
                }
                
                lerOpcaoUsuario(
                    opUser, 
                    produtos, 
                    quantidadeProdutos
                );

            } while (opUser != 0);

            break;
        case 0:
            printf(COR_VERDE "Saindo...\n" COR_RESET);
            break;
        default:
            printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
            break;
    }
    return 1;
}