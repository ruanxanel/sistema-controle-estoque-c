#ifndef MENU_H
#define MENU_H

#include "produto.h"

void menuPrincipal(void);

int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

#endif