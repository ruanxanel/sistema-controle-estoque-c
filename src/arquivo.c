#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include "arquivo.h"
#include "cores.h"

int salvarMovimentacao(Movimentacao movimentacao) {
    FILE *arquivo = fopen("data/movimentacao.dat", "ab");

    if (arquivo == NULL) {
        perror("Erro ao abrir histórico");
        return 0;
    }

    int sucesso = fwrite(&movimentacao, sizeof(Movimentacao), 1, arquivo) == 1;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso)
        printf("Erro ao salvar movimentação!\n");

    return sucesso;
}

void listarMovimentacoes(void) {
    FILE *arquivo = fopen("data/movimentacao.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT)
            printf(COR_AMARELO "Nenhuma movimentação registrada!\n" COR_RESET);
        else
            perror("Erro ao abrir histórico");
        return;
    }

    Movimentacao m;
    int total = 0;

    printf(COR_AZUL "\n================ HISTÓRICO DE MOVIMENTAÇÕES ================\n" COR_RESET);
    printf("%-10s %-12s %-12s %-20s\n",
        "Código", "Tipo", "Quantidade", "Data e hora");
    printf("-----------------------------------------------------------\n");

    while (fread(&m, sizeof(Movimentacao), 1, arquivo) == 1) {
        const char *cor = strcmp(m.tipo, "ENTRADA") == 0
                        ? COR_VERDE : COR_VERMELHO;

        printf("%-10d %s%-12s%s %-12d %-20s\n",
            m.codigoProduto, cor, m.tipo, COR_RESET,
            m.quantidade, m.dataHora);
        total++;
    }

    printf("-----------------------------------------------------------\n");

    if (ferror(arquivo))
        printf(COR_VERMELHO "Erro ao ler histórico!\n" COR_RESET);
    else
        printf("Total de movimentações: %d\n", total);

    fclose(arquivo);
}

int salvarProdutos(Produto produtos[], int quantidadeProdutos) {
    FILE *arquivo = fopen("data/produtos.tmp", "wb");

    if (arquivo == NULL) {
        perror("Erro ao criar arquivo temporário");
        return 0;
    }

    int sucesso = fwrite(produtos, sizeof(Produto), quantidadeProdutos, arquivo)
                == (size_t)quantidadeProdutos;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (sucesso)
        sucesso = rename("data/produtos.tmp", "data/produtos.dat") == 0;

    if (!sucesso) {
        printf("Erro ao salvar produtos!\n");
        remove("data/produtos.tmp");
    }

    return sucesso;
}

int carregarProdutos(Produto **produtos, int *total, int *capacidade) {
    FILE *arquivo = fopen("data/produtos.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT) return 1;
        perror("Erro ao abrir produtos");
        return 0;
    }

    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }

    long tamanho = ftell(arquivo);

    if (tamanho < 0 || (size_t)tamanho % sizeof(Produto) != 0) {
        printf("Arquivo de produtos inválido!\n");
        fclose(arquivo);
        return 0;
    }

    size_t quantidade = (size_t)tamanho / sizeof(Produto);

    if (quantidade > INT_MAX) {
        printf("Limite de produtos excedido!\n");
        fclose(arquivo);
        return 0;
    }

    if (quantidade > (size_t)*capacidade) {
        Produto *temp = realloc(*produtos, quantidade * sizeof(Produto));

        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            fclose(arquivo);
            return 0;
        }

        *produtos = temp;
        *capacidade = (int)quantidade;
    }

    rewind(arquivo);

    int sucesso = fread(*produtos, sizeof(Produto), quantidade, arquivo)
                == quantidade;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso) {
        printf("Erro ao carregar produtos!\n");
        return 0;
    }

    *total = (int)quantidade;
    return 1;
}

int salvarVendedores(Vendedor vendedores[], int total) {
    FILE *arquivo = fopen("data/vendedores.tmp", "wb");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de vendedores");
        return 0;
    }

    int sucesso = fwrite(vendedores, sizeof(Vendedor), total, arquivo)
                == (size_t)total;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (sucesso)
        sucesso = rename("data/vendedores.tmp", "data/vendedores.dat") == 0;

    if (!sucesso) {
        printf("Erro ao salvar vendedores!\n");
        remove("data/vendedores.tmp");
    }

    return sucesso;
}

int carregarVendedores(Vendedor **vendedores, int *total, int *capacidade) {
    FILE *arquivo = fopen("data/vendedores.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT) return 1;

        perror("Erro ao abrir arquivo de vendedores");
        return 0;
    }

    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }

    long tamanho = ftell(arquivo);

    if (tamanho < 0 || (size_t)tamanho % sizeof(Vendedor) != 0) {
        printf("Arquivo de vendedores inválido!\n");
        fclose(arquivo);
        return 0;
    }

    size_t quantidade = (size_t)tamanho / sizeof(Vendedor);

    if (quantidade > INT_MAX) {
        printf("Limite de vendedores excedido!\n");
        fclose(arquivo);
        return 0;
    }

    if (quantidade > (size_t)*capacidade) {
        Vendedor *temp = realloc(
            *vendedores,
            quantidade * sizeof(Vendedor)
        );

        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            fclose(arquivo);
            return 0;
        }

        *vendedores = temp;
        *capacidade = (int)quantidade;
    }

    rewind(arquivo);

    int sucesso = fread(*vendedores, sizeof(Vendedor), quantidade, arquivo)
                == quantidade;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso) {
        printf("Erro ao carregar vendedores!\n");
        return 0;
    }

    *total = (int)quantidade;
    return 1;
}