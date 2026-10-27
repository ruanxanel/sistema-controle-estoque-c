#include<stdio.h>

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

void lerOpcaoAdmin(int opcao) {

    switch (opcao) {
    case 1:
        printf("opcao 1....");
        break;
    case 2:
        printf("opcao 2...");
        break;
    case 3:
        printf("opcao 3...");
        break;
    case 4:
        printf("opcao 4....");
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

void lerOpcaoUsuario(int opcao) {
    switch (opcao) {
    case 1:
        printf("opcao Listar produto");
        break;
    case 2:
        printf("opcao buscar produto");
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

void lerOpcaoPrincipal(int opcao) {
    int opAdm, opUser;
    switch (opcao) {
        case 1:
            do {
                menuAdmin();
                scanf("%d", &opAdm);
                lerOpcaoAdmin(opAdm);
            } while (opAdm != 0);
            break;
        case 2:
            do {
                menuUsuario();
                scanf("%d", &opUser);
                lerOpcaoUsuario(opUser);
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
    do {
        menuPrincipal();
        scanf("%d", &op);
        getchar();
        lerOpcaoPrincipal(op);
    } while (op != 0);

    return 0;
}