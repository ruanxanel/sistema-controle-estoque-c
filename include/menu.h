#ifndef MENU_H
#define MENU_H

#include "produto.h"
#include "vendedor.h"
#include "cliente.h"
#include "carrinho.h"

void menuPrincipal(void);

int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *totalProdutos,
    int *capacidadeProdutos,
    Vendedor **vendedores,
    int *totalVendedores,
    int *capacidadeVendedores,
    Cliente **clientes,
    int *totalClientes,
    int *capacidadeClientes,
    Carrinho *carrinho
);

#endif