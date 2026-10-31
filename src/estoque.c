
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "estoque.h"
#include "arquivo.h"
#include "data.h"
#include "entrada.h"
#include "cores.h"

// Função auxiliar utilizada tanto na entrada quanto na saída.
//
// static: permite chamar a função somente dentro de estoque.c.
//
// produtos[]: vetor que contém os produtos cadastrados.
// total: quantidade de produtos cadastrados.
// entrada: identifica o tipo da operação:
//          1 = entrada de produtos
//          0 = saída de produtos.
//
// Essa função evita repetir as mesmas validações em
// registrarEntrada() e registrarSaida().
static void movimentarEstoque(Produto produtos[], int total, int entrada) {
    int codigo, quantidade;

    // Solicita o código do produto.
    // &codigo passa o endereço da variável para lerInteiro().
    //
    // Se a leitura falhar, a operação é cancelada.
    if (lerInteiro("Código do produto: ", &codigo) != 1) {
        printf(COR_VERMELHO "Código inválido!\n" COR_RESET);
        return;
    }

    // Procura o produto no vetor e retorna seu índice.
    //
    // Se o código não existir, a função retorna -1.
    int indice = localizarProdutoPorCodigo(produtos, total, codigo);

    if (indice == -1) {
        printf(COR_VERMELHO "Produto não encontrado!\n" COR_RESET);
        return;
    }

    // Solicita a quantidade que será movimentada.
    //
    // Apenas valores inteiros maiores que zero são aceitos.
    if (lerInteiro("Quantidade: ", &quantidade) != 1 || quantidade <= 0) {
        printf(COR_VERMELHO "Quantidade inválida!\n" COR_RESET);
        return;
    }

    // Armazena o endereço do produto encontrado.
    //
    // &produtos[indice] obtém o endereço da posição no vetor.
    // Produto *p declara um ponteiro para uma struct Produto.
    //
    // Assim, as alterações feitas usando p->quantidade
    // modificam diretamente o produto original.
    Produto *p = &produtos[indice];

    // Validação para ENTRADA.
    //
    // O operador && significa E lógico.
    //
    // Se entrada for 1, verifica se adicionar a quantidade
    // ultrapassaria o valor máximo permitido para um int.
    //
    // INT_MAX representa o maior valor do tipo int.
    if (entrada && quantidade > INT_MAX - p->quantidade) {
        printf(COR_VERMELHO "Limite do estoque excedido!\n" COR_RESET);
        return;
    }

    // Validação para SAÍDA.
    //
    // O operador ! inverte o valor lógico.
    // !entrada será verdadeiro quando entrada for 0.
    //
    // A saída é rejeitada caso a quantidade solicitada
    // seja maior que a disponível no estoque.
    if (!entrada && quantidade > p->quantidade) {
        printf(COR_VERMELHO "Estoque insuficiente!\n" COR_RESET);
        return;
    }

    // Cria uma estrutura para armazenar a movimentação.
    Movimentacao m;

    // Relaciona a movimentação ao produto informado.
    m.codigoProduto = codigo;

    // Define o tipo da movimentação.
    //
    // O operador ternário funciona assim:
    // condição ? valorSeVerdadeiro : valorSeFalso
    //
    // Se entrada for 1, escolhe "ENTRADA".
    // Caso contrário, escolhe "SAIDA".
    //
    // strcpy() copia a string escolhida para m.tipo.
    strcpy(m.tipo, entrada ? "ENTRADA" : "SAIDA");

    // Armazena a quantidade movimentada.
    m.quantidade = quantidade;

    // Obtém a data/hora atual e salva a movimentação.
    //
    // obterDataHora() preenche o vetor m.dataHora.
    // sizeof(m.dataHora) informa o tamanho disponível.
    //
    // salvarMovimentacao() grava a estrutura em arquivo binário.
    //
    // O operador || significa OU.
    // Se obterDataHora() falhar, salvarMovimentacao()
    // não será chamada por causa do curto-circuito.
    //
    // Se qualquer uma falhar, a operação é cancelada.
    if (!obterDataHora(m.dataHora, sizeof(m.dataHora)) ||
        !salvarMovimentacao(m)) {
        printf(COR_VERMELHO "Erro ao registrar movimentação!\n" COR_RESET);
        return;
    }

    // Atualiza a quantidade do produto na memória.
    //
    // += soma a quantidade à existente.
    // -= subtrai a quantidade da existente.
    //
    // A alteração ocorre após a tentativa de salvar
    // a movimentação no histórico.
    if (entrada)
        p->quantidade += quantidade;
    else
        p->quantidade -= quantidade;

    printf(COR_VERDE "Movimentação registrada com sucesso!\n" COR_RESET);

    // Verifica se o estoque atual ficou abaixo do mínimo.
    //
    // O operador -> acessa um atributo de uma struct
    // através de um ponteiro.
    //
    // O alerta aparece tanto após uma entrada quanto
    // após uma saída, se o estoque continuar baixo.
    if (p->quantidade < p->estoqueMinimo) {
        printf(COR_AMARELO
            "ALERTA: %s abaixo do estoque mínimo! Atual: %d | Mínimo: %d\n"
            COR_RESET, p->nome, p->quantidade, p->estoqueMinimo);
    }
}

// Função pública responsável por registrar entradas.
//
// Envia o valor 1 para movimentarEstoque(), indicando
// que a quantidade deverá ser adicionada ao estoque.
void registrarEntrada(Produto produtos[], int quantidadeProdutos) {
    movimentarEstoque(produtos, quantidadeProdutos, 1);
}

// Função pública responsável por registrar saídas.
//
// Envia o valor 0 para movimentarEstoque(), indicando
// que a quantidade deverá ser retirada do estoque.
void registrarSaida(Produto produtos[], int quantidadeProdutos) {
    movimentarEstoque(produtos, quantidadeProdutos, 0);
}

// Exibe os produtos abaixo do estoque mínimo.
//
// Mostra uma tabela contendo código, nome,
// quantidade atual e quantidade mínima.
void listarEstoqueBaixo(Produto produtos[], int total) {

    // Conta quantos produtos estão abaixo do mínimo.
    int encontrados = 0;

    printf(COR_AZUL "\n================ PRODUTOS COM ESTOQUE BAIXO ================\n" COR_RESET);

    // Formata os títulos das colunas da tabela.
    //
    // %-8s: texto alinhado à esquerda em 8 posições.
    // %-20s: texto alinhado à esquerda em 20 posições.
    // %10s: texto alinhado à direita em 10 posições.
    printf("%-8s %-20s %10s %10s\n",
        "Código", "Produto", "Atual", "Mínimo");

    printf("------------------------------------------------------------\n");

    // Percorre todos os produtos cadastrados.
    for (int i = 0; i < total; i++) {

        // Copia o produto atual para uma variável local.
        // Essa cópia é utilizada somente para leitura.
        Produto p = produtos[i];

        // Verifica se o estoque atual está abaixo do mínimo.
        //
        // Exemplo:
        // quantidade = 3
        // estoqueMinimo = 10
        //
        // 3 < 10, portanto o produto aparece na lista.
        //
        // Se quantidade == estoqueMinimo, não é considerado
        // estoque baixo pela regra atual do sistema.
        if (p.quantidade < p.estoqueMinimo) {

            // Exibe o produto em amarelo para destacar o alerta.
            //
            // %-20.20s limita a exibição do nome a 20 caracteres.
            printf(COR_AMARELO "%-8d %-20.20s %10d %10d\n" COR_RESET,
                p.codigo, p.nome, p.quantidade, p.estoqueMinimo);

            // Incrementa a quantidade de produtos encontrados.
            encontrados++;
        }
    }

    printf("------------------------------------------------------------\n");

    // Exibe uma mensagem caso nenhum produto tenha estoque baixo.
    // Caso contrário, informa o total encontrado.
    if (encontrados == 0)
        printf(COR_VERDE "Nenhum produto com estoque baixo!\n" COR_RESET);
    else
        printf("Total de produtos com estoque baixo: %d\n", encontrados);
}
