#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "produto.h"
#include "entrada.h"

static void exibirProduto(Produto produto) {
    printf("Codigo: %d\n", produto.codigo);
    printf("Nome: %s\n", produto.nome);
    printf("Preço: R$ %.2f\n", produto.preco);
    printf("Quantidade: %d\n", produto.quantidade);
    printf("Estoque minimo: %d\n", produto.estoqueMinimo);
}

void cadastrarProduto(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    Produto produto;
    
    if (lerInteiro("Digite o codigo do produto: ", &produto.codigo) != 1) {
        printf("Código inválido!\n");
        return;
    }
    
    if (produto.codigo <= 0) {
        printf("O código deve ser maior que zero!\n");
        return;
    }
    
    if (localizarProdutoPorCodigo(
        *produtos, *quantidadeProdutos, produto.codigo
    ) != -1) {
        printf("Produto ja cadastrado!\n");
        return;
    }

    if (lerTexto("Digite o nome do produto: ", 
                produto.nome,
                sizeof(produto.nome)) != 1) {
        printf("Nome inválido!\n");
        return;
    }
    
    if (lerDouble("Digite o preço: ", &produto.preco) != 1) {
        printf("Preço inválido!\n");
        return;
    }
    
    if (produto.preco < 0) {
        printf("O preço não pode ser negativo!\n");
        return;
    }
    
    if (lerInteiro("Digite a quantidade: ", &produto.quantidade) != 1) {
        printf("Quantidae inválida!\n");
        return;
    }
    
    if (produto.quantidade < 0) {
        printf("A quantidade não pode ser negativa!\n");
        return;
    }

    if (lerInteiro("Digite o estoque mínimo: ", &produto.estoqueMinimo) != 1) {
        printf("Estoque mínimo inválido!\n");
        return;
    }
    
    if (produto.estoqueMinimo < 0) {
        printf("O estoque mínimo não pode ser negativo!\n");
        return;
    }
    
    if (*quantidadeProdutos == *capacidade) {
        if (*capacidade > INT_MAX / 2) {
            printf("Limite de produtos atingido!\n");
            return;
        }
        
        int novaCapacidade = *capacidade * 2;

        Produto *temp = realloc(
            *produtos,
            (size_t) novaCapacidade * sizeof(Produto)
        );

        if (temp == NULL) {
            printf("Erro ao realocar memoria!\n");
            return;
        }
        
        *produtos = temp;
        *capacidade = novaCapacidade;
    }
    
    (*produtos)[*quantidadeProdutos] = produto;
    (*quantidadeProdutos)++;

    printf("Produto %s cadastrado com sucesso!\n", produto.nome);
}



void listarProdutos(Produto produtos[], int quantidadeProdutos) {
    if (quantidadeProdutos == 0) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }
    
    for (int i = 0; i < quantidadeProdutos; i++) {
        exibirProduto(produtos[i]);
    }
}


int localizarProdutoPorCodigo(
    Produto produtos[],
    int quantidadeProdutos,
    int codigo
) {
    for (int i = 0; i < quantidadeProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            return i;
        }
    }

    return -1;

}


void buscarProduto(Produto produtos[], int quantidadeProdutos) {
    int codigoBuscador;
    
    if (lerInteiro("Digite o codigo do produto: ", &codigoBuscador) != 1) {
        printf("Código inválido!\n");
        return;
    }

    int indice = localizarProdutoPorCodigo(
        produtos, quantidadeProdutos, codigoBuscador
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    exibirProduto(produtos[indice]);
}


void editarProduto(Produto produtos[],int quantidadeProdutos) {
    int codigoBuscador;
    char novoNome[100];
    double novoPreco;
    int novoEstoqueMinimo;

    if (lerInteiro("Digite o codigo do produto que deseja editar: \n", &codigoBuscador) != 1) {
        printf("Código inválido!\n");
        return;
    }
    

    int indice = localizarProdutoPorCodigo(
        produtos, quantidadeProdutos, codigoBuscador
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    int opcao = -1;

    do {

        printf("\n====================================\n");
        printf("   EDITAR PRODUTO\n");
        printf("====================================\n\n");
        
        exibirProduto(produtos[indice]);

        printf("\n1 - Alterar nome\n");
        printf("2 - Alterar preço\n");
        printf("3 - Alterar estoque mínimo\n");
        printf("4 - Alterar todos os dados\n");
        printf("0 - Voltar\n\n");

        int resultado = lerInteiro("Escolha uma opção: ", &opcao);

        if (resultado == 0) {
            continue;
        }

        if (resultado == -1) {
            return;
        }
        
        switch (opcao) {
            case 1:
                if (lerTexto(
                    "Digite o novo nome: ", 
                    novoNome, 
                    sizeof(novoNome)
                ) != 1) {
                    printf("Nome inválido!\n");
                    break;
                }
                
                strcpy(produtos[indice].nome, novoNome);
                printf("Nome atualizado com sucesso!\n");
                break;
            case 2:
                if (lerDouble("Digite o novo preço: ", &novoPreco) != 1) {
                    printf("Preço inválido!\n");
                    break;
                }

                if (novoPreco < 0) {
                    printf("O preço não pode ser negativo!\n");
                    break;
                }
                
                produtos[indice].preco = novoPreco;
                printf("Preço atualizado com sucesso!\n");
                break;
            case 3:
                if (lerInteiro(
                    "Digite o novo estoque mínimo: ",
                    &novoEstoqueMinimo
                ) != 1) {
                    printf("Estoque mínimo inválido!\n");
                    break;
                }

                if (novoEstoqueMinimo < 0) {
                    printf("O estoque mínimo não pode ser negativo!\n");
                    break;
                }
                
                produtos[indice].estoqueMinimo = novoEstoqueMinimo;
                printf("Estoque mínimo atualizado com sucesso!\n");
                break;
            case 4:
                if (lerTexto(
                    "Digite o novo nome: ", 
                    novoNome, 
                    sizeof(novoNome)
                ) != 1) {
                    printf("Nome inválido!\n");
                    break;
                }
                
                if (lerDouble("Digite o novo preço: ", &novoPreco) != 1) {
                    printf("Preço inválido!\n");
                    break;
                }

                if (novoPreco < 0) {
                    printf("O preço não pode ser negativo!\n");
                    break;
                }

                if (lerInteiro(
                    "Digite o novo estoque mínimo: ",
                    &novoEstoqueMinimo
                ) != 1) {
                    printf("Estoque mínimo inválido!\n");
                    break;
                }

                if (novoEstoqueMinimo < 0) {
                    printf("O estoque mínimo não pode ser negativo!\n");
                    break;
                }

                strcpy(produtos[indice].nome, novoNome);
                produtos[indice].preco = novoPreco;
                produtos[indice].estoqueMinimo = novoEstoqueMinimo;
                printf("Dados atualizados com sucesso!\n");

                break;
            case 0:
                printf("Voltando ao menu Administrador...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
            }
    } while (opcao != 0);
    
}


void excluirProduto(Produto produtos[], int *quantidadeProdutos) {
    int codigoBuscador;

    if (lerInteiro("Digite o código do produto: ", &codigoBuscador) != 1) {
        printf("Código inválido!\n");
        return;
    }
    
    int indice = localizarProdutoPorCodigo(
        produtos, *quantidadeProdutos, codigoBuscador
    );

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    for (int i = indice; i < *quantidadeProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    (*quantidadeProdutos)--;

    printf("Produto excluido com sucesso!\n");
}
