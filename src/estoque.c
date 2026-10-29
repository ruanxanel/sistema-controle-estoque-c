#include <stdio.h>
#include "estoque.h"

void registrarEntrada(
    Produto produtos[],
    int quantidadeProdutos
) {
    int codigoBuscado;
    int novaQuantidade;

    printf("Digite o codigo do produto: ");
    scanf("%d", &codigoBuscado);

    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscado
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }
    
    printf("Digite a quantidade de produtos que quer adicionar: ");
    scanf("%d", &novaQuantidade);

    if (novaQuantidade <= 0) {
        printf("A entrada tem que ser maior que zero\n");
        return;
    }
    produtos[indice].quantidade += novaQuantidade;

    printf("Quantidade adicionada\n");
}

void registrarSaida(
    Produto produtos[],
    int quantidadeProdutos
) {
    int codigoBuscado;
    int novaQuantidade;

    printf("Digite o codigo do produto: ");
    scanf("%d", &codigoBuscado);

    int indice = localizarProdutoPorCodigo(
        produtos,
        quantidadeProdutos,
        codigoBuscado
    );

    if (indice == -1) {
        printf("Produto não encontrado!!\n");
        return;
    }

    printf("Digite a quantidade do produto: ");
    scanf("%d", &novaQuantidade);

    if (novaQuantidade <= 0) {
        printf("A quantidade deve ser acima de 0\n");
        return;
    } else if (novaQuantidade > produtos[indice].quantidade) {
        printf("Quantidade invalida. Quantidade disponivel: %d\n", produtos[indice].quantidade);
        return;
    }
    
    produtos[indice].quantidade -= novaQuantidade;
    printf("Saída registrada com sucesso!\n");

    if (produtos[indice].quantidade < produtos[indice].estoqueMinimo) {
        printf("ALERTA: Produto %s está abaixo do estoque mínimo!\n Quantidade atual: %d\n Estoque mínimo: %d\n",
        produtos[indice].nome,
        produtos[indice].quantidade,
        produtos[indice].estoqueMinimo
        );
    }
}

void listarEstoqueBaixo(
    Produto produtos[],
    int quantidadeProdutos
) {
    int encontrouEstoqueBaixo = 0;
    for (int i = 0; i < quantidadeProdutos; i++) {
        if (produtos[i].quantidade < produtos[i].estoqueMinimo) {
            printf(
                "Produto: %s | Quantidade atual: %d | Estoque mínimo: %d | situação: Estoque baixo\n",
                produtos[i].nome,
                produtos[i].quantidade,
                produtos[i].estoqueMinimo
            );
            encontrouEstoqueBaixo = 1;
        }
    }
    if (encontrouEstoqueBaixo == 0) {
        printf("Nenhum produto com estoque baixo\n");
    }
    
}