#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "entrada.h"
#include "produto.h"
#include "arquivo.h"
#include "cores.h"

int main(void) {
    int opcao, total = 0, capacidade = 2;
    Produto *produtos = malloc(capacidade * sizeof(Produto));

    if (produtos == NULL) {
        printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
        return 1;
    }

    if (!carregarProdutos(&produtos, &total, &capacidade)) {
        free(produtos);
        return 1;
    }

    do {
        menuPrincipal();

        int resultado = lerInteiro("Escolha uma opção: ", &opcao);

        if (resultado == 0) continue;
        if (resultado == -1) break;

        if (!lerOpcaoPrincipal(opcao, &produtos, &total, &capacidade))
            break;

    } while (opcao != 0);

    int sucesso = salvarProdutos(produtos, total);
    free(produtos);

    if (!sucesso) {
        printf(COR_VERMELHO "Erro ao salvar produtos!\n" COR_RESET);
        return 1;
    }

    return 0;
}