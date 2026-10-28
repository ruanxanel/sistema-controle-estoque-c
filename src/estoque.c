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