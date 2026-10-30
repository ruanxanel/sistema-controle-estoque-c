#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
#include "produto.h"
#include "arquivo.h"

#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_RESET "\033[0m"

int main(void) {

    int op;
    int quantidadeProdutos = 0;
    int capacidade = 2;

    Produto *produtos = malloc(capacidade * sizeof(Produto));

    if (produtos == NULL) {
        printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
        return 1;
    }
    
    if (!carregarProdutos(&produtos, &quantidadeProdutos, &capacidade)) {
        printf("Erro ao carregar os produtos!\n");
        free(produtos);
        return 1;
    }
    
    do {
        menuPrincipal();

        scanf("%d", &op);

        lerOpcaoPrincipal(
            op,
            &produtos,
            &quantidadeProdutos,
            &capacidade
        );

    } while (op != 0);

    if (!salvarProdutos(produtos, quantidadeProdutos)) {
        printf("Erro ao salvar os produtos!\n");
        free(produtos);
        return 1;
    }

    free(produtos);
    return 0;
}