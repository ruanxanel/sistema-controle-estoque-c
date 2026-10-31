
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "produto.h"
#include "entrada.h"
#include "cores.h"

// Exibe os dados de um único produto.
//
// static restringe o uso da função a este arquivo.
// O parâmetro Produto p é uma cópia da estrutura recebida.
static void exibirProduto(Produto p) {
    printf("\n--------------------------------------\n");
    printf("Código: %d\nNome: %s\nPreço: R$ %.2f\nQuantidade: %d\nEstoque mínimo: %d\n",
            p.codigo, p.nome, p.preco, p.quantidade, p.estoqueMinimo);
    printf("--------------------------------------\n");
}

// Lê e valida o preço digitado.
//
// O ponteiro double *preco permite que lerDouble()
// armazene o valor na variável original.
//
// Retorna 1 quando o preço é válido e 0 quando é inválido.
static int lerPrecoValido(double *preco) {
    // O operador || significa OU.
    //
    // Se a leitura falhar ou o preço for negativo,
    // a condição será verdadeira.
    if (lerDouble("Digite o preço: ", preco) != 1 || *preco < 0) {
        printf(COR_VERMELHO "Preço inválido!\n" COR_RESET);
        return 0;
    }

    return 1;
}

// Percorre o vetor procurando um produto com o código informado.
//
// Retorna a posição do produto no vetor.
// Quando não encontra, retorna -1.
int localizarProdutoPorCodigo(Produto produtos[], int total, int codigo) {
    for (int i = 0; i < total; i++) {

        // O operador . acessa um campo de uma struct.
        if (produtos[i].codigo == codigo) return i;
    }

    return -1;
}

// Cadastra um novo produto no vetor dinâmico.
void cadastrarProduto(Produto **produtos, int *total, int *capacidade) {

    // Cria uma estrutura temporária para receber os dados.
    Produto p;

    // Lê o código e exige um inteiro positivo.
    //
    // &p.codigo passa o endereço do campo para lerInteiro().
    if (lerInteiro("Código: ", &p.codigo) != 1 || p.codigo <= 0) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    // Verifica se já existe um produto com o mesmo código.
    //
    // *produtos acessa o vetor apontado pelo ponteiro duplo.
    // *total acessa o valor do contador.
    if (localizarProdutoPorCodigo(*produtos, *total, p.codigo) != -1) {
        printf(COR_VERMELHO "Produto já cadastrado!\n" COR_RESET);
        return;
    }

    // Lê os demais campos.
    //
    // lerTexto() recebe o vetor de caracteres que guarda o nome.
    // sizeof(p.nome) informa o tamanho desse vetor.
    //
    // O operador || interrompe as avaliações assim que
    // uma condição verdadeira é encontrada.
    //
    // Portanto, se uma leitura falhar, as seguintes não
    // serão executadas e o cadastro será cancelado.
    if (lerTexto("Nome: ", p.nome, sizeof(p.nome)) != 1 ||
        !lerPrecoValido(&p.preco) ||
        lerInteiro("Quantidade: ", &p.quantidade) != 1 ||
        p.quantidade < 0 ||
        lerInteiro("Estoque mínimo: ", &p.estoqueMinimo) != 1 ||
        p.estoqueMinimo < 0) {
        printf(COR_VERMELHO "Dados inválidos!\n" COR_RESET);
        return;
    }

    // Se a quantidade de produtos alcançou a capacidade,
    // precisamos aumentar o espaço reservado na memória.
    if (*total == *capacidade) {

        // Impede que a multiplicação da capacidade por 2
        // ultrapasse o limite do tipo int.
        if (*capacidade > INT_MAX / 2) {
            printf(COR_VERMELHO "Limite de produtos atingido!\n" COR_RESET);
            return;
        }

        // Duplica a capacidade do vetor.
        int novaCapacidade = *capacidade * 2;

        // realloc() tenta redimensionar a memória alocada.
        //
        // Pode manter o mesmo endereço ou mover os dados
        // para outra região da memória.
        //
        // Usamos temp para não perder o ponteiro original
        // caso a realocação falhe.
        Produto *temp = realloc(*produtos, (size_t)novaCapacidade * sizeof(Produto));

        if (temp == NULL) {
            printf(COR_VERMELHO "Erro ao alocar memória!\n" COR_RESET);
            return;
        }

        // Atualiza o ponteiro original e a capacidade.
        *produtos = temp;
        *capacidade = novaCapacidade;
    }

    // Insere o produto na próxima posição disponível.
    //
    // (*produtos)[...] acessa uma posição do vetor.
    // (*total)++ utiliza o índice atual e depois aumenta
    // a quantidade de produtos cadastrados.
    (*produtos)[(*total)++] = p;

    printf(COR_VERDE "Produto %s cadastrado com sucesso!\n" COR_RESET, p.nome);
}

// Exibe os produtos cadastrados em formato de tabela.
void listarProdutos(Produto produtos[], int total) {
    if (total == 0) {
        printf(COR_AMARELO "Nenhum produto cadastrado!\n" COR_RESET);
        return;
    }

    printf(COR_AZUL "\n=================== PRODUTOS CADASTRADOS ===================\n" COR_RESET);

    // Os números do printf definem a largura das colunas.
    //
    // %-8s: texto alinhado à esquerda em 8 posições.
    // %10s: texto alinhado à direita em 10 posições.
    printf("%-8s %-20s %10s %8s %8s\n",
        "Código", "Produto", "Preço", "Qtd.", "Mín.");

    printf("------------------------------------------------------------\n");

    // Percorre todas as posições ocupadas do vetor.
    for (int i = 0; i < total; i++) {

        // Copia o produto atual para uma variável local.
        Produto p = produtos[i];

        // %-20.20s limita a exibição do nome a 20 caracteres.
        // %10.2f mostra o preço com duas casas decimais.
        printf("%-8d %-20.20s %10.2f %8d %8d\n",
            p.codigo, p.nome, p.preco, p.quantidade, p.estoqueMinimo);
    }

    printf("------------------------------------------------------------\n");
    printf("Total de produtos: %d\n", total);
}

// Solicita um código e exibe o produto correspondente.
void buscarProduto(Produto produtos[], int total) {
    int codigo;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    // Reutiliza a função de busca pelo código.
    int indice = localizarProdutoPorCodigo(produtos, total, codigo);

    // -1 indica que o código não existe no vetor.
    if (indice == -1)
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
    else
        exibirProduto(produtos[indice]);
}

// Permite alterar os atributos de um produto.
void editarProduto(Produto produtos[], int total) {
    int codigo, opcao = -1, minimo;
    char nome[100];
    double preco;

    printf("\n========== EDITAR PRODUTO ==========\n");

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    // Localiza o produto antes de abrir o submenu.
    int indice = localizarProdutoPorCodigo(produtos, total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    // Guarda o endereço do produto dentro do vetor.
    //
    // Assim, podemos alterar seus campos diretamente,
    // sem precisar copiar a estrutura e depois devolvê-la.
    Produto *p = &produtos[indice];

    do {
        printf("\n========== OPÇÕES DE EDIÇÃO ==========\n");

        // *p acessa a estrutura apontada pelo ponteiro.
        exibirProduto(*p);

        printf("\n1 - Alterar nome\n2 - Alterar preço\n");
        printf("3 - Alterar estoque mínimo\n4 - Alterar todos os dados\n0 - Voltar\n");

        int resultado = lerInteiro("Opção: ", &opcao);

        // Entrada inválida: volta ao início do submenu.
        if (resultado == 0) continue;

        // Fim da entrada: sai da função.
        if (resultado == -1) return;

        switch (opcao) {

            case 1:
                // Lê primeiro para uma variável temporária.
                // Só substitui o nome depois de validar.
                if (lerTexto("Novo nome: ", nome, sizeof(nome)) == 1) {

                    // strcpy() copia a string para o campo nome.
                    //
                    // O operador -> acessa o campo de uma struct
                    // por meio de um ponteiro.
                    strcpy(p->nome, nome);

                    printf(COR_VERDE "Nome atualizado!\n" COR_RESET);
                } else {
                    printf(COR_VERMELHO "Nome inválido!\n" COR_RESET);
                }
                break;

            case 2:
                if (lerPrecoValido(&preco)) {
                    p->preco = preco;
                    printf(COR_VERDE "Preço atualizado!\n" COR_RESET);
                }
                break;

            case 3:
                if (lerInteiro("Novo estoque mínimo: ", &minimo) == 1 && minimo >= 0) {
                    p->estoqueMinimo = minimo;
                    printf(COR_VERDE "Estoque mínimo atualizado!\n" COR_RESET);
                } else {
                    printf(COR_VERMELHO "Estoque mínimo inválido!\n" COR_RESET);
                }
                break;

            case 4:
                // Na edição completa, os novos valores são
                // validados antes de modificar o produto.
                //
                // Se algum dado for inválido, a operação
                // é interrompida e os dados originais permanecem.
                if (lerTexto("Novo nome: ", nome, sizeof(nome)) != 1 ||
                    !lerPrecoValido(&preco) ||
                    lerInteiro("Novo estoque mínimo: ", &minimo) != 1 ||
                    minimo < 0) {
                    printf(COR_VERMELHO "Dados inválidos!\n" COR_RESET);
                    break;
                }

                // Aplica as alterações depois de todas as validações.
                strcpy(p->nome, nome);
                p->preco = preco;
                p->estoqueMinimo = minimo;

                printf(COR_VERDE "Dados atualizados!\n" COR_RESET);
                break;

            case 0:
                // Encerra o submenu e volta ao menu anterior.
                break;

            default:
                printf(COR_VERMELHO "Opção inválida!\n" COR_RESET);
        }

    } while (opcao != 0);
}

// Exclui um produto do vetor.
void excluirProduto(Produto produtos[], int *total) {
    int codigo;

    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    int indice = localizarProdutoPorCodigo(produtos, *total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    // Desloca os elementos posteriores uma posição à esquerda.
    //
    // Exemplo:
    // Antes: [Arroz, Feijão, Leite]
    // Excluindo Feijão: [Arroz, Leite, Leite]
    //
    // O último elemento deixa de fazer parte do vetor lógico
    // quando o contador é reduzido.
    for (int i = indice; i < *total - 1; i++)
        produtos[i] = produtos[i + 1];

    // Diminui a quantidade de produtos cadastrados.
    // Não é necessário diminuir a memória alocada.
    (*total)--;

    printf(COR_VERDE "Produto excluído com sucesso!\n" COR_RESET);
}