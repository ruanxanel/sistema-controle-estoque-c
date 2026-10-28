#include<stdio.h>
#include<stdlib.h>
#include "produto.h"

#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_RESET "\033[0m"

void menuAdmin() {
    printf("\n======================================\n");
    printf("         PAINEL ADMINISTRADOR         \n");
    printf("======================================\n\n");

    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Buscar produto\n");
    printf("4 - Editar produto\n");
    printf("5 - Excluir produto\n");
    printf("6 - Registrar entrada\n");
    printf("7 - Registrar saída\n");
    printf("8 - Produtos com estoque baixo\n");
    printf("9 - Histórico de movimentações\n");
    printf("0 - Voltar ao menu principal\n");

    printf("\nEscolha uma opção: ");
}

void lerOpcaoAdmin(
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
        printf("opcao 5...");
        break;
    case 6:
        printf("opcao 6...");
        break;
    case 7:
        printf("opcao 7....");
        break;
    case 8:
        printf("opcao 8....");
        break;
    case 9:
        printf("opcao 9.....");
        break;
    case 0:
        printf(COR_VERDE "Voltando....\n" COR_RESET);
        break;
    default:
        printf(COR_VERMELHO "Opção inválida" COR_RESET);
        break;
    }
}


void menuUsuario() {
    printf("\n======================================\n");
    printf("           PAINEL USUÁRIO             \n");
    printf("======================================\n\n");

    printf("1 - Listar produtos\n");
    printf("2 - Buscar produto\n");
    printf("3 - Registrar entrada\n");
    printf("4 - Registrar saída\n");
    printf("5 - Produtos com estoque baixo\n");
    printf("6 - Histórico de movimentações\n");
    printf("0 - Voltar ao menu principal\n");

    printf("\nEscolha uma opção: ");
}

void lerOpcaoUsuario(
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
        printf("opcao registrar");
        break;
    case 4:
        printf("opcao 4....");
        break;
    case 5:
        printf("Opcao 5....");
        break;
    case 6:
        printf("opcao 6...");
        break;
    case 0:
        printf(COR_VERDE "Voltando...\n" COR_RESET);
        break;
    default:
        printf(COR_VERMELHO "Opção inválida" COR_RESET);
        break;
    }
}

void menuPrincipal() {

    printf("======================================\n");
    printf("    SISTEMA DE CONTROLE DE ESTOQUE    \n");
    printf("======================================\n\n");

    printf("1 - Administrador\n");
    printf("2 - Usuário\n");
    printf("0 - Sair\n");

    printf("\nEscolha uma opção: ");

}

void lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    int opAdm, opUser;
    switch (opcao) {
        case 1:
            do {
                menuAdmin();
                scanf("%d", &opAdm);
                lerOpcaoAdmin(opAdm, produtos, quantidadeProdutos, capacidade);
            } while (opAdm != 0);
            break;
        case 2:
            do {
                menuUsuario();
                scanf("%d", &opUser);
                lerOpcaoUsuario(opUser, produtos, quantidadeProdutos);
            } while (opUser != 0);
            break;
        case 0:
            printf(COR_VERDE "Saindo...\n" COR_RESET);
            break;
        default:
            printf(COR_VERMELHO "Opção invalida\n" COR_RESET);
            break;
    }
}

int main(void) {

    int op;
    Produto *produtos; 
    int quantidadeProdutos = 0;
    int capacidade = 2;
    produtos = malloc(capacidade * sizeof(Produto));

    if(produtos == NULL) {
        printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
        return 1;
    }
    
    do {
        menuPrincipal();
        scanf("%d", &op);
        getchar();
        lerOpcaoPrincipal(op, &produtos, &quantidadeProdutos, &capacidade);
    } while (op != 0);

    free(produtos);

    return 0;
}