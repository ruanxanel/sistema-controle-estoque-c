#ifndef MENU_H
#define MENU_H

#include "produto.h"

void menuPrincipal(void);

void lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

#endif