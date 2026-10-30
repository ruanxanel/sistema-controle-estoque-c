#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#include "arquivo.h"

int salvarMovimentacao(
    Movimentacao movimentacao
) {
    FILE *arquivo = fopen("data/movimentacao.dat", "ab");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }
    
    size_t gravados = fwrite(
        &movimentacao,
        sizeof(movimentacao),
        1,
        arquivo
    );

    if (gravados != 1) {
        printf("Erro em salvar a movimentação!\n");
        fclose(arquivo);
        return 0;
    }
    
    if(fclose(arquivo) == EOF) {
        printf("Erro ao fechar o arquivo!\n");
        return 0;
    }

    return 1;
}

void listarMovimentacoes(
    void
) {
    FILE *arquivo = fopen("data/movimentacao.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT) {
            printf("Nenhuma movimentação registrada!\n");
        } else {
            perror("Erro ao abrir o historico");
        }
        return;
    }

    Movimentacao movimentacao;
    size_t lidos = fread(
        &movimentacao,
        sizeof(movimentacao),
        1,
        arquivo
    );
    int totalMovimentacoes = 0;

    while (lidos == 1) {
        printf(
            "Código: %d\nTipo: %s\nQuantidade: %d\nData: %s\n\n",
            movimentacao.codigoProduto,
            movimentacao.tipo,
            movimentacao.quantidade,
            movimentacao.dataHora
        );

        lidos = fread(
            &movimentacao,
            sizeof(movimentacao),
            1,
            arquivo
        );
        
        totalMovimentacoes++;
    }

    if (totalMovimentacoes == 0) {
        printf("Nenhuma movimentação registrada!\n");
    }
    
    if (ferror(arquivo)) {
        printf("Erro ao ler o histórico!\n");
    }
    
    fclose(arquivo);
}


int salvarProdutos(
    Produto produtos[],
    int quantidadeProdutos
) {
    FILE *arquivo = fopen("data/produtos.tmp", "wb");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo temporário");
        return 0;
    }

    size_t gravados = fwrite(
        produtos,
        sizeof(Produto),
        quantidadeProdutos,
        arquivo
    );

    if (gravados != (size_t) quantidadeProdutos) {
        printf("Erro ao salvar os produtos!\n");
        fclose(arquivo);
        remove("data/produtos.tmp");
        return 0;
    }

    if (fclose(arquivo) == EOF) {
        printf("Erro ao fechar o arquivo!\n");
        remove("data/produtos.tmp");
        return 0;
    }

    if (rename("data/produtos.tmp", "data/produtos.dat") != 0) {
        perror("Erro ao substituir arquivo de produtos");
        remove("data/produtos.tmp");
        return 0;
    }

    return 1;
}


int carregarProdutos(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    FILE *arquivo = fopen("data/produtos.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT) {
            return 1;
        }
        
        perror("Erro ao abrir arquivo de produtos");
        return 0;
    }
    
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        printf("Erro ao acessar o final do arquivo!\n");
        fclose(arquivo);
        return 0;
    }

    long tamanhoArquivo = ftell(arquivo);

    if (tamanhoArquivo == -1L) {
        printf("Erro ao obter o tamanho do arquivo!\n");
        fclose(arquivo);
        return 0;
    }

    if ((size_t)tamanhoArquivo % sizeof(Produto) != 0) {
        printf("Erro: arquivo de produtos inválido!\n");
        fclose(arquivo);
        return 0;
    }
    
    size_t totalProdutos = (size_t)tamanhoArquivo / sizeof(Produto);

    if (totalProdutos > INT_MAX) {
        printf("Erro: quantidade de produtos excede o limite!\n");
        fclose(arquivo);
        return 0;
    }
    
    if (totalProdutos > (size_t)*capacidade) {
        Produto *temp = realloc(
            *produtos,
            totalProdutos * sizeof(Produto)
        );

        if (temp == NULL) {
            printf("Erro ao relocar memoria!\n");
            fclose(arquivo);
            return 0;
        }
        
        *produtos = temp;
        *capacidade = (int) totalProdutos;
    }

    rewind(arquivo);
    
    size_t lidos = fread(
        *produtos,
        sizeof(Produto),
        totalProdutos,
        arquivo
    );

    if (lidos != totalProdutos) {
        printf("Erro ao carregar os produtos!\n");
        fclose(arquivo);
        return 0;
    }
    
    *quantidadeProdutos = (int) totalProdutos;

    if (fclose(arquivo) == EOF) {
        printf("Erro ao fechar o arquivo!\n");
        return 0;
    }

    return 1;
}