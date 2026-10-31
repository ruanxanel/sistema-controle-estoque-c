
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include "arquivo.h"
#include "cores.h"

// Registra uma movimentação no histórico.
//
// O arquivo é aberto no modo "ab":
// a = append (adicionar ao final)
// b = binary (arquivo binário)
//
// Assim, as movimentações anteriores são preservadas.
int salvarMovimentacao(Movimentacao movimentacao) {

    // FILE representa uma estrutura utilizada pela
    // biblioteca padrão para controlar um arquivo.
    //
    // fopen() retorna um ponteiro FILE* quando consegue
    // abrir o arquivo, ou NULL quando falha.
    FILE *arquivo = fopen("data/movimentacao.dat", "ab");

    // Verifica se o arquivo foi aberto corretamente.
    if (arquivo == NULL) {

        // perror() mostra uma mensagem acompanhada
        // da descrição do erro do sistema.
        perror("Erro ao abrir histórico");
        return 0;
    }

    // fwrite() grava dados binários no arquivo.
    //
    // &movimentacao: endereço da estrutura que será gravada.
    // sizeof(Movimentacao): tamanho da estrutura em bytes.
    // 1: quantidade de elementos que queremos gravar.
    // arquivo: destino da gravação.
    //
    // fwrite() retorna a quantidade de elementos gravados.
    // A comparação == 1 verifica se a gravação foi completa.
    int sucesso = fwrite(&movimentacao, sizeof(Movimentacao), 1, arquivo) == 1;

    // fclose() fecha o arquivo e descarrega os dados
    // pendentes do buffer da biblioteca.
    //
    // EOF indica uma falha no fechamento.
    if (fclose(arquivo) == EOF)
        sucesso = 0;

    // Exibe uma mensagem se ocorreu erro na gravação
    // ou no fechamento do arquivo.
    if (!sucesso)
        printf("Erro ao salvar movimentação!\n");

    return sucesso;
}

// Lê todas as movimentações registradas no histórico.
void listarMovimentacoes(void) {

    // Abre o arquivo para leitura binária.
    //
    // r = read (leitura)
    // b = binary (binário)
    FILE *arquivo = fopen("data/movimentacao.dat", "rb");

    // Verifica se o arquivo existe e se pode ser aberto.
    if (arquivo == NULL) {

        // errno guarda um código de erro da biblioteca/sistema.
        //
        // ENOENT indica que o arquivo ou caminho não existe.
        //
        // Nesse caso, a ausência do histórico é tratada
        // como uma situação normal do primeiro uso.
        if (errno == ENOENT)
            printf(COR_AMARELO "Nenhuma movimentação registrada!\n" COR_RESET);
        else
            perror("Erro ao abrir histórico");

        return;
    }

    // Estrutura que recebe cada registro lido.
    Movimentacao m;

    // Contador de movimentações encontradas.
    int total = 0;

    printf(COR_AZUL "\n================ HISTÓRICO DE MOVIMENTAÇÕES ================\n" COR_RESET);

    // Define a largura e o alinhamento das colunas.
    //
    // %-10s: texto alinhado à esquerda em 10 posições.
    // %-12s: texto alinhado à esquerda em 12 posições.
    // %-20s: texto alinhado à esquerda em 20 posições.
    printf("%-10s %-12s %-12s %-20s\n",
        "Código", "Tipo", "Quantidade", "Data e hora");

    printf("-----------------------------------------------------------\n");

    // fread() lê registros binários do arquivo.
    //
    // &m: endereço onde cada registro será armazenado.
    // sizeof(Movimentacao): tamanho de cada registro.
    // 1: quantidade de registros por leitura.
    //
    // Enquanto fread() retornar 1, existe uma
    // movimentação completa para exibir.
    while (fread(&m, sizeof(Movimentacao), 1, arquivo) == 1) {

        // strcmp() compara duas strings.
        //
        // Retorna 0 quando elas são iguais.
        //
        // Se o tipo for "ENTRADA", escolhe verde.
        // Caso contrário, escolhe vermelho.
        //
        // O operador ternário substitui um if/else
        // simples que selecionaria a cor.
        const char *cor = strcmp(m.tipo, "ENTRADA") == 0
                        ? COR_VERDE : COR_VERMELHO;

        // Exibe uma linha da tabela.
        //
        // %s: imprime uma string.
        // %d: imprime um inteiro.
        //
        // cor altera a cor do tipo de movimentação.
        // COR_RESET restaura a cor padrão do terminal.
        printf("%-10d %s%-12s%s %-12d %-20s\n",
            m.codigoProduto, cor, m.tipo, COR_RESET,
            m.quantidade, m.dataHora);

        // Incrementa o contador.
        total++;
    }

    printf("-----------------------------------------------------------\n");

    // ferror() verifica se ocorreu erro durante a leitura.
    //
    // Se não houve erro, mostra o total encontrado,
    // inclusive quando o histórico está vazio.
    if (ferror(arquivo))
        printf(COR_VERMELHO "Erro ao ler histórico!\n" COR_RESET);
    else
        printf("Total de movimentações: %d\n", total);

    // Fecha o arquivo após a leitura.
    fclose(arquivo);
}

// Salva todos os produtos no arquivo binário.
//
// Diferentemente do histórico, que acrescenta registros,
// os produtos são salvos como uma coleção completa.
//
// Primeiro grava em produtos.tmp e depois substitui
// produtos.dat, evitando substituir diretamente o arquivo
// principal antes da conclusão da gravação.
int salvarProdutos(Produto produtos[], int quantidadeProdutos) {

    // "wb" abre para escrita binária.
    //
    // Se o arquivo temporário já existir, ele será
    // sobrescrito. Caso contrário, será criado.
    FILE *arquivo = fopen("data/produtos.tmp", "wb");

    if (arquivo == NULL) {
        perror("Erro ao criar arquivo temporário");
        return 0;
    }

    // Grava quantidadeProdutos estruturas Produto.
    //
    // fwrite() retorna quantos elementos foram gravados.
    // O resultado é comparado com a quantidade esperada.
    //
    // O cast para size_t ajusta o tipo do contador
    // para o tipo retornado por fwrite().
    int sucesso = fwrite(produtos, sizeof(Produto), quantidadeProdutos, arquivo)
                == (size_t)quantidadeProdutos;

    // Verifica se o arquivo foi fechado corretamente.
    if (fclose(arquivo) == EOF)
        sucesso = 0;

    // Se a gravação funcionou, rename() substitui
    // o arquivo principal pelo temporário.
    //
    // rename() retorna 0 quando a operação funciona.
    if (sucesso)
        sucesso = rename("data/produtos.tmp", "data/produtos.dat") == 0;

    // Em caso de falha, remove o arquivo temporário.
    //
    // A intenção é preservar a versão anterior do
    // arquivo principal quando a gravação não terminou.
    if (!sucesso) {
        printf("Erro ao salvar produtos!\n");
        remove("data/produtos.tmp");
    }

    return sucesso;
}

// Carrega os produtos salvos em produtos.dat.
//
// Pode precisar aumentar a memória reservada por main()
// para armazenar todos os produtos do arquivo.
int carregarProdutos(Produto **produtos, int *total, int *capacidade) {

    // Abre o arquivo principal em modo de leitura binária.
    FILE *arquivo = fopen("data/produtos.dat", "rb");

    if (arquivo == NULL) {

        // Se o arquivo ainda não existe, considera que
        // o programa está sendo executado pela primeira vez.
        //
        // Nesse caso, retorna sucesso e mantém o vetor vazio.
        if (errno == ENOENT) return 1;

        perror("Erro ao abrir produtos");
        return 0;
    }

    // Move o indicador de posição para o final do arquivo.
    //
    // SEEK_END representa o final do arquivo.
    //
    // Isso permite descobrir quantos bytes estão armazenados.
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }

    // ftell() informa a posição atual do arquivo.
    //
    // Como estamos no final, esse valor corresponde
    // ao tamanho do arquivo em bytes.
    long tamanho = ftell(arquivo);

    // Verifica se ftell() falhou.
    //
    // Também verifica se o tamanho do arquivo é
    // múltiplo do tamanho de uma estrutura Produto.
    //
    // Por exemplo, se cada Produto ocupar 128 bytes,
    // um arquivo com 256 bytes contém 2 registros.
    //
    // O tamanho real depende da plataforma e do
    // alinhamento dos campos da struct.
    if (tamanho < 0 || (size_t)tamanho % sizeof(Produto) != 0) {
        printf("Arquivo de produtos inválido!\n");
        fclose(arquivo);
        return 0;
    }

    // Calcula quantos registros Produto existem no arquivo.
    size_t quantidade = (size_t)tamanho / sizeof(Produto);

    // Verifica se a quantidade cabe no tipo int,
    // utilizado pelo sistema para armazenar o contador.
    if (quantidade > INT_MAX) {
        printf("Limite de produtos excedido!\n");
        fclose(arquivo);
        return 0;
    }

    // Se a quantidade carregada for maior que a
    // capacidade atual, será necessário realocar.
    if (quantidade > (size_t)*capacidade) {

        // realloc() redimensiona a memória do vetor.
        //
        // *produtos é o ponteiro original do vetor.
        // quantidade * sizeof(Produto) determina os bytes.
        //
        // Usamos temp para não perder o endereço original
        // caso a realocação falhe.
        Produto *temp = realloc(*produtos, quantidade * sizeof(Produto));

        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            fclose(arquivo);
            return 0;
        }

        // Atualiza o ponteiro original e a capacidade.
        //
        // Como recebemos Produto **produtos, podemos
        // modificar o ponteiro armazenado em main().
        *produtos = temp;
        *capacidade = (int)quantidade;
    }

    // Volta o indicador de posição para o início
    // do arquivo, preparando a leitura dos registros.
    rewind(arquivo);

    // Lê todos os registros diretamente para o vetor.
    //
    // fread() retorna quantos elementos foram lidos.
    // O resultado é comparado com a quantidade esperada.
    int sucesso = fread(*produtos, sizeof(Produto), quantidade, arquivo)
                == quantidade;

    // Fecha o arquivo e verifica se ocorreu erro.
    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso) {
        printf("Erro ao carregar produtos!\n");
        return 0;
    }

    // Atualiza o contador de produtos no main().
    // O cast é seguro porque verificamos INT_MAX acima.
    *total = (int)quantidade;

    return 1;
}
