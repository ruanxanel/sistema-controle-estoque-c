#include <stdio.h>
#include <stdlib.h>

#include "carrinho.h"
#include "cores.h"

int inicializarCarrinho(Carrinho *carrinho) {
    carrinho->total = 0;
    carrinho->capacidade = 2;

    carrinho->itens = malloc(carrinho->capacidade * sizeof(ItemVenda));

    if (carrinho->itens == NULL) {
        printf(COR_VERMELHO "Erro ao criar carrinho!\n" COR_RESET);
        return 0;
    }

    return 1;
}

void liberarCarrinho(Carrinho *carrinho) {
    free(carrinho->itens);

    carrinho->itens = NULL;
    carrinho->total = 0;
    carrinho->capacidade = 0;
}