
#ifndef MENU_H
#define MENU_H

// Evita que este arquivo seja incluído mais de uma vez
// durante a compilação.

#include "produto.h"

// Declara a função responsável por exibir o menu principal.
//
// void dentro dos parênteses indica que a função
// não recebe nenhum parâmetro.
//
// O primeiro void indica que ela não retorna um valor.
void menuPrincipal(void);

// Recebe a opção escolhida no menu principal e
// controla a navegação entre Administrador e Usuário.
//
// opcao: opção digitada no menu principal.
//
// Produto **produtos: ponteiro duplo que permite
// atualizar o endereço do vetor de produtos.
//
// int *quantidadeProdutos: endereço do contador de produtos.
//
// int *capacidade: endereço da capacidade do vetor.
//
// Retorna:
// 1 = execução normal
// 0 = encerramento solicitado por fim da entrada (EOF).
int lerOpcaoPrincipal(
    int opcao,
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
);

#endif
