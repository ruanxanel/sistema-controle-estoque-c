#include <stdio.h>
#include <stdlib.h>

#include "carrinho.h"
#include "entrada.h"
#include "produto.h"
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

void adicionarAoCarrinho(Carrinho *carrinho, Produto produtos[], int totalProdutos) {
    int codigo, quantidade;

    listarProdutos(produtos, totalProdutos);

    if (totalProdutos == 0)
        return;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, totalProdutos, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    Produto *produto = &produtos[indice];

    if (lerInteiro("Quantidade: ", &quantidade) != 1 || quantidade <= 0) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    int indiceCarrinho = -1;

    for (int i = 0; i < carrinho->total; i++) {
        if (carrinho->itens[i].codigoProduto == codigo) {
            indiceCarrinho = i;
            break;
        }
    }

    int quantidadeAtual = 0;

    if (indiceCarrinho != -1)
        quantidadeAtual = carrinho->itens[indiceCarrinho].quantidade;

    if (quantidade > produto->quantidade - quantidadeAtual) {
        printf(COR_VERMELHO "Estoque insuficiente!\n" COR_RESET);
        return;
    }

    if (indiceCarrinho != -1) {
        carrinho->itens[indiceCarrinho].quantidade += quantidade;
    } else {
        if (carrinho->total == carrinho->capacidade) {
            int novaCapacidade = carrinho->capacidade * 2;

            ItemVenda *temp = realloc(
                carrinho->itens,
                novaCapacidade * sizeof(ItemVenda)
            );

            if (temp == NULL) {
                printf(COR_VERMELHO "Erro ao aumentar carrinho!\n" COR_RESET);
                return;
            }

            carrinho->itens = temp;
            carrinho->capacidade = novaCapacidade;
        }

        ItemVenda item;

        item.codigoProduto = codigo;
        item.quantidade = quantidade;
        item.precoUnitario = produto->preco;

        carrinho->itens[carrinho->total++] = item;
    }

    printf(COR_VERDE "Produto adicionado ao carrinho!\n" COR_RESET);
}

void editarQuantidadeCarrinho(Carrinho *carrinho, Produto produtos[], int totalProdutos) {
    int codigo, novaQuantidade;

    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho está vazio!\n" COR_RESET);
        return;
    }

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indiceCarrinho = -1;

    for (int i = 0; i < carrinho->total; i++) {
        if (carrinho->itens[i].codigoProduto == codigo) {
            indiceCarrinho = i;
            break;
        }
    }

    if (indiceCarrinho == -1) {
        printf(COR_VERMELHO "Produto não está no carrinho!\n" COR_RESET);
        return;
    }

    int indiceProduto = localizarProdutoPorCodigo(produtos, totalProdutos, codigo);

    if (indiceProduto == -1) {
        printf(COR_VERMELHO "Produto não encontrado no estoque!\n" COR_RESET);
        return;
    }

    printf("Quantidade atual: %d\n", carrinho->itens[indiceCarrinho].quantidade);

    if (lerInteiro("Nova quantidade: ", &novaQuantidade) != 1 || novaQuantidade <= 0) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    if (novaQuantidade > produtos[indiceProduto].quantidade) {
        printf(COR_VERMELHO "Estoque insuficiente!\n" COR_RESET);
        return;
    }

    carrinho->itens[indiceCarrinho].quantidade = novaQuantidade;

    printf(COR_VERDE "Quantidade atualizada com sucesso!\n" COR_RESET);
}

void removerDoCarrinho(Carrinho *carrinho) {
    int codigo;

    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho está vazio!\n" COR_RESET);
        return;
    }

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = -1;

    for (int i = 0; i < carrinho->total; i++) {
        if (carrinho->itens[i].codigoProduto == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não está no carrinho!\n" COR_RESET);
        return;
    }

    for (int i = indice; i < carrinho->total - 1; i++)
        carrinho->itens[i] = carrinho->itens[i + 1];

    carrinho->total--;

    printf(COR_VERDE "Produto removido do carrinho!\n" COR_RESET);
}

void limparCarrinho(Carrinho *carrinho) {
    int confirmar;

    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho já está vazio!\n" COR_RESET);
        return;
    }

    printf(COR_AMARELO "Deseja remover todos os produtos?\n" COR_RESET);

    if (lerInteiro("[1] Sim  [0] Não: ", &confirmar) != 1) {
        printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        return;
    }

    if (confirmar == 1) {
        carrinho->total = 0;
        printf(COR_VERDE "Carrinho esvaziado com sucesso!\n" COR_RESET);
    } else if (confirmar == 0) {
        printf(COR_AMARELO "Operação cancelada!\n" COR_RESET);
    } else {
        printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
    }
}

void visualizarCarrinho(Carrinho *carrinho, Produto produtos[], int totalProdutos) {
    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho está vazio!\n" COR_RESET);
        return;
    }

    double valorTotal = 0;
    int totalUnidades = 0;

    printf(COR_AZUL "\n=================== MEU CARRINHO ===================\n" COR_RESET);
    printf("%-8s %-18s %-6s %-10s %-10s\n",
        "Código", "Produto", "Qtd", "Preço", "Subtotal");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < carrinho->total; i++) {
        ItemVenda item = carrinho->itens[i];

        int indice = localizarProdutoPorCodigo(
            produtos, totalProdutos, item.codigoProduto
        );

        const char *nome = "Produto indisponível";

        if (indice != -1)
            nome = produtos[indice].nome;

        double subtotal = item.quantidade * item.precoUnitario;

        printf("%-8d %-18.18s %-6d R$ %-7.2f R$ %.2f\n",
            item.codigoProduto,
            nome,
            item.quantidade,
            item.precoUnitario,
            subtotal);

        valorTotal += subtotal;
        totalUnidades += item.quantidade;
    }

    printf("----------------------------------------------------\n");
    printf("Total de unidades: %d\n", totalUnidades);
    printf(COR_VERDE "VALOR TOTAL: R$ %.2f\n" COR_RESET, valorTotal);
}

int finalizarCompra(
    Carrinho *carrinho,
    Produto produtos[],
    int totalProdutos,
    Vendedor vendedores[],
    int totalVendedores,
    Cliente clientes[],
    int totalClientes
) {
    int codigoCliente, codigoVendedor;

    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho está vazio!\n" COR_RESET);
        return 0;
    }

    visualizarCarrinho(carrinho, produtos, totalProdutos);

    if (lerInteiro("Código do cliente: ", &codigoCliente) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return 0;
    }

    int indiceCliente = localizarClientePorCodigo(
        clientes, totalClientes, codigoCliente
    );

    if (indiceCliente == -1) {
        printf(COR_VERMELHO "Cliente não cadastrado!\n" COR_RESET);
        return 0;
    }

    printf("\nCliente: %s\n", clientes[indiceCliente].nome);

    printf(COR_AZUL "\n========== SELECIONE O VENDEDOR ==========\n" COR_RESET);

    listarVendedores(vendedores, totalVendedores);

    if (totalVendedores == 0)
        return 0;

    if (lerInteiro("Código do vendedor que te atendeu: ", &codigoVendedor) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return 0;
    }

    int indiceVendedor = localizarVendedorPorCodigo(
        vendedores, totalVendedores, codigoVendedor
    );

    if (indiceVendedor == -1) {
        printf(COR_VERMELHO "Vendedor não encontrado!\n" COR_RESET);
        return 0;
    }

    printf(COR_VERDE "\nVendedor selecionado: %s\n" COR_RESET,
        vendedores[indiceVendedor].nome);

    printf(COR_AMARELO "\nRegistro da venda ainda não implementado.\n" COR_RESET);

    return 0;
}