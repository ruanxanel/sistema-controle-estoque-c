
#include <stdio.h>
#include <errno.h>

#include "venda.h"
#include "entrada.h"
#include "cores.h"

double calcularComissao(int quantidadeItens, double valorTotal) {
    if (quantidadeItens <= 0 || valorTotal <= 0)
        return 0;

    if (quantidadeItens <= 10)
        return valorTotal * 0.05;

    return valorTotal * 0.10;
}

void listarVendasVendedor(Vendedor vendedores[], int totalVendedores) {
    int codigo;

    if (totalVendedores == 0) {
        printf(COR_AMARELO "Nenhum vendedor cadastrado!\n" COR_RESET);
        return;
    }

    listarVendedores(vendedores, totalVendedores);

    if (lerInteiro("Código do vendedor: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarVendedorPorCodigo(
        vendedores, totalVendedores, codigo
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Vendedor não encontrado!\n" COR_RESET);
        return;
    }

    FILE *arquivo = fopen("data/vendas.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT)
            printf(COR_AMARELO "Nenhuma venda registrada!\n" COR_RESET);
        else
            perror("Erro ao abrir vendas");
        return;
    }

    Venda venda;
    int quantidadeVendas = 0;
    double totalVendido = 0;

    printf(COR_AZUL "\n========== MINHAS VENDAS ==========\n" COR_RESET);
    printf("Vendedor: %s\n\n", vendedores[indice].nome);
    printf("%-8s %-18s %-10s %-12s\n",
        "Venda", "Data", "Unidades", "Total");
    printf("----------------------------------------------------\n");

    while (fread(&venda, sizeof(Venda), 1, arquivo) == 1) {
        if (venda.codigoVendedor == codigo) {
            printf("%-8d %-18s %-10d R$ %.2f\n",
                venda.codigo,
                venda.dataHora,
                venda.quantidadeItens,
                venda.valorTotal
            );

            quantidadeVendas++;
            totalVendido += venda.valorTotal;
        }
    }

    if (ferror(arquivo)) {
        printf(COR_VERMELHO "Erro ao ler vendas!\n" COR_RESET);
    } else if (quantidadeVendas == 0) {
        printf(COR_AMARELO "Este vendedor ainda não realizou vendas.\n" COR_RESET);
    } else {
        printf("----------------------------------------------------\n");
        printf("Quantidade de vendas: %d\n", quantidadeVendas);
        printf(COR_VERDE "Total vendido: R$ %.2f\n" COR_RESET, totalVendido);
    }

    fclose(arquivo);
}

void listarComissoesVendedor(Vendedor vendedores[], int totalVendedores) {
    int codigo;

    if (totalVendedores == 0) {
        printf(COR_AMARELO "Nenhum vendedor cadastrado!\n" COR_RESET);
        return;
    }

    listarVendedores(vendedores, totalVendedores);

    if (lerInteiro("Código do vendedor: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarVendedorPorCodigo(
        vendedores, totalVendedores, codigo
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Vendedor não encontrado!\n" COR_RESET);
        return;
    }

    FILE *arquivo = fopen("data/vendas.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT)
            printf(COR_AMARELO "Nenhuma venda registrada!\n" COR_RESET);
        else
            perror("Erro ao abrir vendas");
        return;
    }

    Venda venda;
    int quantidadeVendas = 0;
    double totalComissoes = 0;

    printf(COR_AZUL "\n========== MINHAS COMISSÕES ==========\n" COR_RESET);
    printf("Vendedor: %s\n\n", vendedores[indice].nome);
    printf("%-8s %-18s %-14s\n",
        "Venda", "Valor da venda", "Comissão");
    printf("----------------------------------------------------\n");

    while (fread(&venda, sizeof(Venda), 1, arquivo) == 1) {
        if (venda.codigoVendedor == codigo) {
            printf("%-8d R$ %-15.2f R$ %.2f\n",
                venda.codigo,
                venda.valorTotal,
                venda.comissao
            );

            quantidadeVendas++;
            totalComissoes += venda.comissao;
        }
    }

    if (ferror(arquivo)) {
        printf(COR_VERMELHO "Erro ao ler comissões!\n" COR_RESET);
    } else if (quantidadeVendas == 0) {
        printf(COR_AMARELO "Este vendedor ainda não possui comissões.\n" COR_RESET);
    } else {
        printf("----------------------------------------------------\n");
        printf("Quantidade de vendas: %d\n", quantidadeVendas);
        printf(COR_VERDE "Total de comissões: R$ %.2f\n" COR_RESET,
            totalComissoes);
    }

    fclose(arquivo);
}
