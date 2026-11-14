#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "carrinho.h"
#include "entrada.h"
#include "produto.h"
#include "arquivo.h"
#include "venda.h"
#include "data.h"
#include "cores.h"

int inicializarCarrinho(Carrinho *carrinho) {
    carrinho->total = 0;
    carrinho->capacidade = 2;
    carrinho->codigoCliente = 0;

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
    carrinho->codigoCliente = 0;
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
            if (carrinho->capacidade > INT_MAX / 2) {
                printf(COR_VERMELHO "Limite do carrinho atingido!\n" COR_RESET);
                return;
            }

            int novaCapacidade = carrinho->capacidade * 2;

            ItemVenda *temp = realloc(
                carrinho->itens,
                (size_t)novaCapacidade * sizeof(ItemVenda)
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

    if (lerInteiro("Nova quantidade: ", &novaQuantidade) != 1 ||
        novaQuantidade <= 0) {
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

static int validarEstoqueCarrinho(
    Carrinho *carrinho,
    Produto produtos[],
    int totalProdutos
) {
    for (int i = 0; i < carrinho->total; i++) {
        ItemVenda item = carrinho->itens[i];

        int indice = localizarProdutoPorCodigo(
            produtos, totalProdutos, item.codigoProduto
        );

        if (indice == -1) {
            printf(COR_VERMELHO "Produto %d não encontrado!\n" COR_RESET,
                item.codigoProduto);
            return 0;
        }

        if (item.quantidade <= 0 ||
            item.quantidade > produtos[indice].quantidade) {
            printf(COR_VERMELHO "Estoque insuficiente para %s!\n" COR_RESET,
                produtos[indice].nome);
            return 0;
        }
    }

    return 1;
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
    int codigoCliente = carrinho->codigoCliente;
    int codigoVendedor;

    if (carrinho->total == 0) {
        printf(COR_AMARELO "O carrinho está vazio!\n" COR_RESET);
        return 0;
    }

    visualizarCarrinho(carrinho, produtos, totalProdutos);

    if (codigoCliente <= 0) {
        printf(COR_VERMELHO "Nenhum cliente identificado!\n" COR_RESET);
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

    if (!validarEstoqueCarrinho(carrinho, produtos, totalProdutos))
        return 0;

    int totalUnidades = 0;
    double valorTotal = 0;

    for (int i = 0; i < carrinho->total; i++) {
        ItemVenda item = carrinho->itens[i];

        if (item.quantidade > INT_MAX - totalUnidades ||
            !isfinite(item.precoUnitario) ||
            item.precoUnitario < 0) {
            printf(COR_VERMELHO "Dados da compra inválidos!\n" COR_RESET);
            return 0;
        }

        totalUnidades += item.quantidade;
        valorTotal += item.quantidade * item.precoUnitario;
    }

    if (!isfinite(valorTotal) || valorTotal <= 0) {
        printf(COR_VERMELHO "Valor total inválido!\n" COR_RESET);
        return 0;
    }

    double comissao = calcularComissao(totalUnidades, valorTotal);

    printf(COR_AZUL "\n========== RESUMO DA COMPRA ==========\n" COR_RESET);
    printf("Cliente: %s\n", clientes[indiceCliente].nome);
    printf("Vendedor: %s\n", vendedores[indiceVendedor].nome);
    printf("Total de unidades: %d\n", totalUnidades);
    printf("Valor total: R$ %.2f\n", valorTotal);
    printf("Comissão do vendedor: R$ %.2f\n", comissao);

    int confirmar;

    if (lerInteiro("\n[1] Confirmar compra  [0] Cancelar: ", &confirmar) != 1 ||
        confirmar != 1) {
        printf(COR_AMARELO "Compra não confirmada.\n" COR_RESET);
        return 0;
    }

    Venda venda = {0};

    if (!proximoCodigoVenda(&venda.codigo)) {
        printf(COR_VERMELHO "Erro ao gerar código da venda!\n" COR_RESET);
        return 0;
    }

    venda.codigoCliente = codigoCliente;
    venda.codigoVendedor = codigoVendedor;
    venda.quantidadeItens = totalUnidades;
    venda.valorTotal = valorTotal;
    venda.comissao = comissao;

    if (!obterDataHora(venda.dataHora, sizeof(venda.dataHora))) {
        printf(COR_VERMELHO "Erro ao registrar data da venda!\n" COR_RESET);
        return 0;
    }

    if (!registrarCompraCompleta(
        venda,
        carrinho->itens,
        carrinho->total,
        produtos,
        totalProdutos
    )) {
        printf(COR_VERMELHO "Não foi possível concluir a compra!\n" COR_RESET);
        return 0;
    }

    carrinho->total = 0;

    printf(COR_VERDE "\n========== COMPRA FINALIZADA ==========\n" COR_RESET);
    printf("Código da venda: %d\n", venda.codigo);
    printf("Cliente: %s\n", clientes[indiceCliente].nome);
    printf("Vendedor: %s\n", vendedores[indiceVendedor].nome);
    printf("Data: %s\n", venda.dataHora);
    printf("Total de unidades: %d\n", venda.quantidadeItens);
    printf("Valor total: R$ %.2f\n", venda.valorTotal);
    printf("Comissão do vendedor: R$ %.2f\n", venda.comissao);
    printf(COR_VERDE "\nCompra realizada com sucesso!\n" COR_RESET);

    return 1;
}