#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "produto.h"
#include "entrada.h"
#include "cores.h"

static void exibirProduto(Produto produto) {
    printf("\n--------------------------------------\n");
    printf("Codigo: %d\n", produto.codigo);
    printf("Nome: %s\n", produto.nome);
    printf("Preço: R$ %.2f\n", produto.preco);
    printf("Quantidade: %d\n", produto.quantidade);
    printf("Estoque minimo: %d\n", produto.estoqueMinimo);
    printf("--------------------------------------\n");
}

void cadastrarProduto(
    Produto **produtos,
    int *quantidadeProdutos,
    int *capacidade
) {
    Produto produto;
    
    if (lerInteiro("Digite o codigo do produto: ", &produto.codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }
    
    if (produto.codigo <= 0) {
        printf(COR_VERMELHO "O código deve ser maior que zero!\n" COR_RESET);
        return;
    }
    
    if (localizarProdutoPorCodigo(
        *produtos, *quantidadeProdutos, produto.codigo
    ) != -1) {
        printf(COR_VERMELHO "Produto ja cadastrado!\n" COR_RESET);
        return;
    }

    if (lerTexto("Digite o nome do produto: ", 
                produto.nome,
                sizeof(produto.nome)) != 1) {
        printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
        return;
    }
    
    if (lerDouble("Digite o preço: ", &produto.preco) != 1) {
        printf(COR_VERMELHO "Preço inválido!\n" COR_RESET);
        return;
    }
    
    if (produto.preco < 0) {
        printf(COR_VERMELHO "O preço não pode ser negativo!\n" COR_RESET);
        return;
    }
    
    if (lerInteiro("Digite a quantidade: ", &produto.quantidade) != 1) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }
    
    if (produto.quantidade < 0) {
        printf(COR_VERMELHO "A quantidade não pode ser negativa!\n" COR_RESET);
        return;
    }

    if (lerInteiro("Digite o estoque mínimo: ", &produto.estoqueMinimo) != 1) {
        printf(COR_VERMELHO "Estoque mínimo inválido!\n" COR_RESET);
        return;
    }
    
    if (produto.estoqueMinimo < 0) {
        printf(COR_VERMELHO "O estoque mínimo não pode ser negativo!\n" COR_RESET);
        return;
    }
    
    if (*quantidadeProdutos == *capacidade) {
        if (*capacidade > INT_MAX / 2) {
            printf(COR_VERMELHO "Limite de produtos atingido!\n" COR_RESET);
            return;
        }
        
        int novaCapacidade = *capacidade * 2;

        Produto *temp = realloc(
            *produtos,
            (size_t) novaCapacidade * sizeof(Produto)
        );

        if (temp == NULL) {
            printf(COR_VERMELHO "Erro ao realocar memoria!\n" COR_RESET);
            return;
        }
        
        *produtos = temp;
        *capacidade = novaCapacidade;
    }
    
    (*produtos)[*quantidadeProdutos] = produto;
    (*quantidadeProdutos)++;

    printf(COR_VERDE "Produto %s cadastrado com sucesso!\n" COR_RESET,
        produto.nome);
}


void listarProdutos(Produto produtos[], int quantidadeProdutos) {
    if (quantidadeProdutos == 0) {
        printf(COR_VERMELHO "Nenhum produto cadastrado!\n" COR_RESET);
        return;
    }
    printf("\n========== PRODUTOS CADASTRADOS ==========\n");
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
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(
        produtos, quantidadeProdutos, codigoBuscador
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
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
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }
    

    int indice = localizarProdutoPorCodigo(
        produtos, quantidadeProdutos, codigoBuscador
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
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
                    printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
                    break;
                }
                
                strcpy(produtos[indice].nome, novoNome);
                printf(COR_VERDE "Nome atualizado com sucesso!\n" COR_RESET);
                break;
            case 2:
                if (lerDouble("Digite o novo preço: ", &novoPreco) != 1) {
                    printf(COR_VERMELHO "Preço inválido!\n" COR_RESET);
                    break;
                }

                if (novoPreco < 0) {
                    printf(COR_VERMELHO "O preço não pode ser negativo!\n" COR_RESET);
                    break;
                }
                
                produtos[indice].preco = novoPreco;
                printf(COR_VERDE "Preço atualizado com sucesso!\n" COR_RESET);
                break;
            case 3:
                if (lerInteiro(
                    "Digite o novo estoque mínimo: ",
                    &novoEstoqueMinimo
                ) != 1) {
                    printf(COR_VERMELHO "Estoque mínimo inválido!\n" COR_RESET);
                    break;
                }

                if (novoEstoqueMinimo < 0) {
                    printf(COR_VERMELHO "O estoque mínimo não pode ser negativo!\n" COR_RESET);
                    break;
                }
                
                produtos[indice].estoqueMinimo = novoEstoqueMinimo;
                printf(COR_VERDE "Estoque mínimo atualizado com sucesso!\n" COR_RESET);
                break;
            case 4:
                if (lerTexto(
                    "Digite o novo nome: ", 
                    novoNome, 
                    sizeof(novoNome)
                ) != 1) {
                    printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
                    break;
                }
                
                if (lerDouble("Digite o novo preço: ", &novoPreco) != 1) {
                    printf(COR_VERMELHO "Preço inválido!\n" COR_RESET);
                    break;
                }

                if (novoPreco < 0) {
                    printf(COR_VERMELHO "O preço não pode ser negativo!\n" COR_RESET);
                    break;
                }

                if (lerInteiro(
                    "Digite o novo estoque mínimo: ",
                    &novoEstoqueMinimo
                ) != 1) {
                    printf(COR_VERMELHO "Estoque mínimo inválido!\n" COR_RESET);
                    break;
                }

                if (novoEstoqueMinimo < 0) {
                    printf(COR_VERMELHO "O estoque mínimo não pode ser negativo!\n" COR_RESET);
                    break;
                }

                strcpy(produtos[indice].nome, novoNome);
                produtos[indice].preco = novoPreco;
                produtos[indice].estoqueMinimo = novoEstoqueMinimo;
                printf(COR_VERDE "Dados atualizados com sucesso!\n" COR_RESET);

                break;
            case 0:
                printf(COR_VERDE "Voltando ao menu Administrador...\n" COR_RESET);
                break;
            default:
                printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
                break;
            }
    } while (opcao != 0);
    
}


void excluirProduto(Produto produtos[], int *quantidadeProdutos) {
    int codigoBuscador;

    if (lerInteiro("Digite o código do produto: ", &codigoBuscador) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }
    
    int indice = localizarProdutoPorCodigo(
        produtos, *quantidadeProdutos, codigoBuscador
    );

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    for (int i = indice; i < *quantidadeProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    (*quantidadeProdutos)--;

    printf(COR_VERDE "Produto excluido com sucesso!\n" COR_RESET);
}