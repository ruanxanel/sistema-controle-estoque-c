#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "entrada.h"
#include "produto.h"
#include "vendedor.h"
#include "arquivo.h"
#include "cores.h"

int main(void) {
    int opcao;
    int totalProdutos = 0, capacidadeProdutos = 2;
    int totalVendedores = 0, capacidadeVendedores = 2;

    Produto *produtos = malloc(capacidadeProdutos * sizeof(Produto));
    Vendedor *vendedores = malloc(capacidadeVendedores * sizeof(Vendedor));

    if (produtos == NULL || vendedores == NULL) {
        printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
        free(produtos);
        free(vendedores);
        return 1;
    }

    if (!carregarProdutos(&produtos, &totalProdutos, &capacidadeProdutos) ||
        !carregarVendedores(&vendedores, &totalVendedores, &capacidadeVendedores)) {
        free(produtos);
        free(vendedores);
        return 1;
    }

    do {
        menuPrincipal();

        int resultado = lerInteiro("Escolha uma opção: ", &opcao);

        if (resultado == 0) continue;
        if (resultado == -1) break;

        if (!lerOpcaoPrincipal(opcao, &produtos, &totalProdutos, &capacidadeProdutos))
            break;

    } while (opcao != 0);

    int salvouProdutos = salvarProdutos(produtos, totalProdutos);
    int salvouVendedores = salvarVendedores(vendedores, totalVendedores);

    free(produtos);
    free(vendedores);

    if (!salvouProdutos || !salvouVendedores) {
        printf(COR_VERMELHO "Erro ao salvar os dados!\n" COR_RESET);
        return 1;
    }

    return 0;
}