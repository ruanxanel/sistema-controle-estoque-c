#include<stdio.h>

#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_RESET "\033[0m"

void funcionalidadesUsuario() {
    //* menu do usuario
}

void funcionalidadesAdmin() {
    //* menu do admin
    printf("sistema do admin\n");
}

void menuPrincipal() {

    printf("================================\n");
    printf(" SISTEMA DE CONTROLE DE ESTOQUE \n");
    printf("================================\n\n");

    printf("1 - Administrador\n");
    printf("2 - Usuário\n");
    printf("0 - Sair\n");

    printf("\nEscolha uma opção: ");

}

void lerOpcaoPrincipal(int opcao) {

    switch (opcao) {
        case 1:
            funcionalidadesAdmin();
            break;
        case 2:
            funcionalidadesUsuario();
            break;
        case 0:
            printf(COR_VERDE "Saindo...\n" COR_RESET);
            break;
        default:
            printf(COR_VERMELHO "Opção invalida\n" COR_RESET);
            break;
    }
}

int main() {
    int op;
    do {
        menuPrincipal();
        scanf("%d", &op);
        getchar();
        lerOpcaoPrincipal(op);
    } while (op != 0);

    return 0;
}