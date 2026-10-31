#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include "arquivo.h"

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
            printf("Nenhuma movimentação registrada!\n");
        else
            perror("Erro ao abrir histórico");
        return;
    }

    Movimentacao m;
    int total = 0;

    while (fread(&m, sizeof(Movimentacao), 1, arquivo) == 1) {
        printf("Código: %d\nTipo: %s\nQuantidade: %d\nData: %s\n\n",
            m.codigoProduto, m.tipo, m.quantidade, m.dataHora);
        total++;
    }

    if (ferror(arquivo))
        printf("Erro ao ler histórico!\n");
    else if (total == 0)
        printf("Nenhuma movimentação registrada!\n");

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