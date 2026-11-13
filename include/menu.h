#ifndef MENU_H
#define MENU_H

#include "produto.h"
#include "vendedor.h"

void menuPrincipal(void);

int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *totalProdutos,
    int *capacidadeProdutos,
    Vendedor **vendedores,
    int *totalVendedores,
    int *capacidadeVendedores
);

#endif
