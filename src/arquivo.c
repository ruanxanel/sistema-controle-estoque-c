#include <stdio.h>
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
        printf("Erro ao abrir o arquivo!\n");
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