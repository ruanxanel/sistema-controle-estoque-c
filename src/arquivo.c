#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "arquivo.h"
#include "cores.h"

int recuperarCompraPendente(void);

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
            printf(COR_AMARELO "Nenhuma movimentação registrada!\n" COR_RESET);
        else
            perror("Erro ao abrir histórico");
        return;
    }

    Movimentacao m;
    int total = 0;

    printf(COR_AZUL "\n================ HISTÓRICO DE MOVIMENTAÇÕES ================\n" COR_RESET);
    printf("%-10s %-12s %-12s %-20s\n",
        "Código", "Tipo", "Quantidade", "Data e hora");
    printf("-----------------------------------------------------------\n");

    while (fread(&m, sizeof(Movimentacao), 1, arquivo) == 1) {
        const char *cor = strcmp(m.tipo, "ENTRADA") == 0
                        ? COR_VERDE : COR_VERMELHO;

        printf("%-10d %s%-12s%s %-12d %-20s\n",
            m.codigoProduto, cor, m.tipo, COR_RESET,
            m.quantidade, m.dataHora);
        total++;
    }

    printf("-----------------------------------------------------------\n");

    if (ferror(arquivo))
        printf(COR_VERMELHO "Erro ao ler histórico!\n" COR_RESET);
    else
        printf("Total de movimentações: %d\n", total);

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
    if (!recuperarCompraPendente())
        return 0;

    FILE *arquivo = fopen("data/produtos.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT)
            return 1;

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

int salvarVendedores(Vendedor vendedores[], int total) {
    FILE *arquivo = fopen("data/vendedores.tmp", "wb");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de vendedores");
        return 0;
    }

    int sucesso = fwrite(vendedores, sizeof(Vendedor), total, arquivo)
                == (size_t)total;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (sucesso)
        sucesso = rename("data/vendedores.tmp", "data/vendedores.dat") == 0;

    if (!sucesso) {
        printf("Erro ao salvar vendedores!\n");
        remove("data/vendedores.tmp");
    }

    return sucesso;
}

int carregarVendedores(Vendedor **vendedores, int *total, int *capacidade) {
    FILE *arquivo = fopen("data/vendedores.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT)
            return 1;

        perror("Erro ao abrir arquivo de vendedores");
        return 0;
    }

    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }

    long tamanho = ftell(arquivo);

    if (tamanho < 0 || (size_t)tamanho % sizeof(Vendedor) != 0) {
        printf("Arquivo de vendedores inválido!\n");
        fclose(arquivo);
        return 0;
    }

    size_t quantidade = (size_t)tamanho / sizeof(Vendedor);

    if (quantidade > INT_MAX) {
        printf("Limite de vendedores excedido!\n");
        fclose(arquivo);
        return 0;
    }

    if (quantidade > (size_t)*capacidade) {
        Vendedor *temp = realloc(
            *vendedores,
            quantidade * sizeof(Vendedor)
        );

        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            fclose(arquivo);
            return 0;
        }

        *vendedores = temp;
        *capacidade = (int)quantidade;
    }

    rewind(arquivo);

    int sucesso = fread(*vendedores, sizeof(Vendedor), quantidade, arquivo)
                == quantidade;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso) {
        printf("Erro ao carregar vendedores!\n");
        return 0;
    }

    *total = (int)quantidade;
    return 1;
}

int salvarClientes(Cliente clientes[], int total) {
    FILE *arquivo = fopen("data/clientes.tmp", "wb");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de clientes");
        return 0;
    }

    int sucesso = fwrite(clientes, sizeof(Cliente), total, arquivo)
                == (size_t)total;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (sucesso)
        sucesso = rename("data/clientes.tmp", "data/clientes.dat") == 0;

    if (!sucesso) {
        printf("Erro ao salvar clientes!\n");
        remove("data/clientes.tmp");
    }

    return sucesso;
}

int carregarClientes(Cliente **clientes, int *total, int *capacidade) {
    FILE *arquivo = fopen("data/clientes.dat", "rb");

    if (arquivo == NULL) {
        if (errno == ENOENT)
            return 1;

        perror("Erro ao abrir arquivo de clientes");
        return 0;
    }

    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }

    long tamanho = ftell(arquivo);

    if (tamanho < 0 || (size_t)tamanho % sizeof(Cliente) != 0) {
        printf("Arquivo de clientes inválido!\n");
        fclose(arquivo);
        return 0;
    }

    size_t quantidade = (size_t)tamanho / sizeof(Cliente);

    if (quantidade > INT_MAX) {
        printf("Limite de clientes excedido!\n");
        fclose(arquivo);
        return 0;
    }

    if (quantidade > (size_t)*capacidade) {
        Cliente *temp = realloc(
            *clientes,
            quantidade * sizeof(Cliente)
        );

        if (temp == NULL) {
            printf("Erro ao realocar memória!\n");
            fclose(arquivo);
            return 0;
        }

        *clientes = temp;
        *capacidade = (int)quantidade;
    }

    rewind(arquivo);

    int sucesso = fread(*clientes, sizeof(Cliente), quantidade, arquivo)
                == quantidade;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso) {
        printf("Erro ao carregar clientes!\n");
        return 0;
    }

    *total = (int)quantidade;
    return 1;
}

int salvarVenda(Venda venda) {
    FILE *arquivo = fopen("data/vendas.dat", "ab");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de vendas");
        return 0;
    }

    int sucesso = fwrite(&venda, sizeof(Venda), 1, arquivo) == 1;

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso)
        printf(COR_VERMELHO "Erro ao salvar venda!\n" COR_RESET);

    return sucesso;
}

int salvarItensVenda(int codigoVenda, ItemVenda itens[], int totalItens) {
    FILE *arquivo = fopen("data/itens_venda.dat", "ab");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de itens");
        return 0;
    }

    int sucesso = 1;

    for (int i = 0; i < totalItens; i++) {
        ItemVendaRegistrado registro;

        registro.codigoVenda = codigoVenda;
        registro.item = itens[i];

        if (fwrite(&registro, sizeof(ItemVendaRegistrado), 1, arquivo) != 1) {
            sucesso = 0;
            break;
        }
    }

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    if (!sucesso)
        printf(COR_VERMELHO "Erro ao salvar itens da venda!\n" COR_RESET);

    return sucesso;
}

int proximoCodigoVenda(int *codigo) {
    if (codigo == NULL)
        return 0;

    FILE *arquivo = fopen("data/vendas.dat", "rb");

    *codigo = 1;

    if (arquivo == NULL) {
        if (errno == ENOENT)
            return 1;

        perror("Erro ao abrir vendas");
        return 0;
    }

    if (fseek(arquivo, 0, SEEK_END) != 0) {
        fclose(arquivo);
        return 0;
    }

    long tamanho = ftell(arquivo);

    if (tamanho < 0 || (size_t)tamanho % sizeof(Venda) != 0) {
        printf(COR_VERMELHO "Arquivo de vendas inválido!\n" COR_RESET);
        fclose(arquivo);
        return 0;
    }

    rewind(arquivo);

    Venda venda;

    while (fread(&venda, sizeof(Venda), 1, arquivo) == 1) {
        if (venda.codigo >= *codigo) {
            if (venda.codigo == INT_MAX) {
                fclose(arquivo);
                return 0;
            }

            *codigo = venda.codigo + 1;
        }
    }

    int sucesso = !ferror(arquivo);

    if (fclose(arquivo) == EOF)
        sucesso = 0;

    return sucesso;
}

int registrarVendaArquivo(Venda venda, ItemVenda itens[], int totalItens) {
    if (venda.codigo <= 0 || itens == NULL || totalItens <= 0) {
        printf(COR_VERMELHO "Dados da venda inválidos!\n" COR_RESET);
        return 0;
    }

    int quantidade = 0;

    for (int i = 0; i < totalItens; i++) {
        if (itens[i].codigoProduto <= 0 ||
            itens[i].quantidade <= 0 ||
            itens[i].precoUnitario < 0 ||
            quantidade > INT_MAX - itens[i].quantidade) {
            printf(COR_VERMELHO "Item de venda inválido!\n" COR_RESET);
            return 0;
        }

        quantidade += itens[i].quantidade;
    }

    if (quantidade != venda.quantidadeItens) {
        printf(COR_VERMELHO "Quantidade total da venda inválida!\n" COR_RESET);
        return 0;
    }

    FILE *arquivoVendas = fopen("data/vendas.dat", "ab+");

    if (arquivoVendas == NULL) {
        perror("Erro ao abrir arquivo de vendas");
        return 0;
    }

    FILE *arquivoItens = fopen("data/itens_venda.dat", "ab+");

    if (arquivoItens == NULL) {
        perror("Erro ao abrir arquivo de itens");
        fclose(arquivoVendas);
        return 0;
    }

    if (fseek(arquivoVendas, 0, SEEK_END) != 0 ||
        fseek(arquivoItens, 0, SEEK_END) != 0) {
        fclose(arquivoVendas);
        fclose(arquivoItens);
        return 0;
    }

    long tamanhoVendas = ftell(arquivoVendas);
    long tamanhoItens = ftell(arquivoItens);

    if (tamanhoVendas < 0 || tamanhoItens < 0 ||
        (size_t)tamanhoVendas % sizeof(Venda) != 0 ||
        (size_t)tamanhoItens % sizeof(ItemVendaRegistrado) != 0) {
        printf(COR_VERMELHO "Histórico de vendas inválido!\n" COR_RESET);
        fclose(arquivoVendas);
        fclose(arquivoItens);
        return 0;
    }

    int sucesso = 1;

    for (int i = 0; i < totalItens; i++) {
        ItemVendaRegistrado registro;

        registro.codigoVenda = venda.codigo;
        registro.item = itens[i];

        if (fwrite(&registro, sizeof(ItemVendaRegistrado), 1, arquivoItens) != 1) {
            sucesso = 0;
            break;
        }
    }

    if (sucesso && fwrite(&venda, sizeof(Venda), 1, arquivoVendas) != 1)
        sucesso = 0;

    if (fflush(arquivoItens) != 0)
        sucesso = 0;

    if (fflush(arquivoVendas) != 0)
        sucesso = 0;

    if (fclose(arquivoItens) == EOF)
        sucesso = 0;

    if (fclose(arquivoVendas) == EOF)
        sucesso = 0;

    if (!sucesso) {
        int recuperouItens = truncate("data/itens_venda.dat", tamanhoItens) == 0;
        int recuperouVendas = truncate("data/vendas.dat", tamanhoVendas) == 0;

        printf(COR_VERMELHO "Erro ao registrar venda!\n" COR_RESET);

        if (!recuperouItens || !recuperouVendas)
            printf(COR_VERMELHO "Falha na recuperação! Verifique os arquivos de vendas.\n" COR_RESET);

        return 0;
    }

    return 1;
}

static const char *arquivosCompra[3] = {
    "data/produtos.dat",
    "data/vendas.dat",
    "data/itens_venda.dat"
};

static const char *copiasCompra[3] = {
    "data/produtos.compra.bak",
    "data/vendas.compra.bak",
    "data/itens_venda.compra.bak"
};

static int copiarArquivo(const char *origem, const char *destino) {
    FILE *entrada = fopen(origem, "rb");
    if (entrada == NULL)
        return 0;

    FILE *saida = fopen(destino, "wb");
    if (saida == NULL) {
        fclose(entrada);
        return 0;
    }

    char bloco[4096];
    size_t lidos;
    int sucesso = 1;

    while ((lidos = fread(bloco, 1, sizeof(bloco), entrada)) > 0) {
        if (fwrite(bloco, 1, lidos, saida) != lidos) {
            sucesso = 0;
            break;
        }
    }

    if (ferror(entrada))
        sucesso = 0;

    if (fflush(saida) != 0 || fsync(fileno(saida)) != 0)
        sucesso = 0;

    if (fclose(entrada) == EOF)
        sucesso = 0;

    if (fclose(saida) == EOF)
        sucesso = 0;

    if (!sucesso)
        remove(destino);

    return sucesso;
}

int recuperarCompraPendente(void) {
    FILE *controle = fopen("data/compra.pendente", "rb");

    if (controle == NULL) {
        if (errno == ENOENT)
            return 1;
        perror("Erro ao verificar compra pendente");
        return 0;
    }

    int existia[3];
    int valido = fscanf(controle, "%d %d %d", &existia[0], &existia[1], &existia[2]) == 3;

    if (fclose(controle) == EOF)
        valido = 0;

    for (int i = 0; i < 3 && valido; i++) {
        if (existia[i] != 0 && existia[i] != 1)
            valido = 0;
    }

    if (!valido) {
        printf(COR_VERMELHO "Controle de recuperação inválido!\n" COR_RESET);
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        if (existia[i]) {
            char temporario[128];
            snprintf(temporario, sizeof(temporario), "%s.restaurar.tmp", arquivosCompra[i]);

            if (!copiarArquivo(copiasCompra[i], temporario) ||
                rename(temporario, arquivosCompra[i]) != 0) {
                printf(COR_VERMELHO "Falha ao recuperar %s!\n" COR_RESET, arquivosCompra[i]);
                return 0;
            }
        } else if (remove(arquivosCompra[i]) != 0 && errno != ENOENT) {
            perror("Erro ao recuperar arquivo inexistente");
            return 0;
        }
    }

    if (remove("data/compra.pendente") != 0) {
        perror("Erro ao concluir recuperação");
        return 0;
    }

    for (int i = 0; i < 3; i++)
        remove(copiasCompra[i]);

    printf(COR_AMARELO "Compra interrompida recuperada. Nenhuma venda parcial foi mantida.\n" COR_RESET);
    return 1;
}

int registrarCompraCompleta(
    Venda venda,
    ItemVenda itens[],
    int totalItens,
    Produto produtos[],
    int totalProdutos
) {
    if (!recuperarCompraPendente())
        return 0;

    if (venda.codigo <= 0 || venda.codigoCliente <= 0 ||
        venda.codigoVendedor <= 0 || itens == NULL || totalItens <= 0 ||
        produtos == NULL || totalProdutos < 0 ||
        !isfinite(venda.valorTotal) || venda.valorTotal <= 0 ||
        !isfinite(venda.comissao) || venda.comissao < 0) {
        printf(COR_VERMELHO "Dados da compra inválidos!\n" COR_RESET);
        return 0;
    }

    int proximo;
    if (!proximoCodigoVenda(&proximo) || proximo != venda.codigo) {
        printf(COR_VERMELHO "Código da venda inválido ou desatualizado!\n" COR_RESET);
        return 0;
    }

    Produto *atualizados = malloc((size_t)(totalProdutos > 0 ? totalProdutos : 1) * sizeof(Produto));
    if (atualizados == NULL)
        return 0;

    memcpy(atualizados, produtos, (size_t)totalProdutos * sizeof(Produto));

    int unidades = 0;
    double valorCalculado = 0;

    for (int i = 0; i < totalItens; i++) {
        ItemVenda item = itens[i];
        int indice = localizarProdutoPorCodigo(atualizados, totalProdutos, item.codigoProduto);

        if (indice < 0 || item.quantidade <= 0 ||
            !isfinite(item.precoUnitario) || item.precoUnitario < 0 ||
            unidades > INT_MAX - item.quantidade ||
            item.quantidade > atualizados[indice].quantidade) {
            printf(COR_VERMELHO "Produto, quantidade ou preço inválido na compra!\n" COR_RESET);
            free(atualizados);
            return 0;
        }

        unidades += item.quantidade;
        valorCalculado += item.quantidade * item.precoUnitario;
        atualizados[indice].quantidade -= item.quantidade;
    }

    if (!isfinite(valorCalculado) || unidades != venda.quantidadeItens ||
        fabs(valorCalculado - venda.valorTotal) > 0.005 ||
        fabs(calcularComissao(unidades, valorCalculado) - venda.comissao) > 0.005) {
        printf(COR_VERMELHO "Totais da venda inconsistentes!\n" COR_RESET);
        free(atualizados);
        return 0;
    }

    int existia[3] = {0};

    for (int i = 0; i < 3; i++) {
        FILE *teste = fopen(arquivosCompra[i], "rb");

        if (teste != NULL) {
            existia[i] = 1;
            if (fclose(teste) == EOF || !copiarArquivo(arquivosCompra[i], copiasCompra[i])) {
                printf(COR_VERMELHO "Erro ao preparar cópia de segurança!\n" COR_RESET);
                free(atualizados);
                return 0;
            }
        } else if (errno != ENOENT) {
            perror("Erro ao consultar arquivo da compra");
            free(atualizados);
            return 0;
        }
    }

    FILE *controle = fopen("data/compra.pendente.tmp", "wb");
    if (controle == NULL) {
        free(atualizados);
        return 0;
    }

    int pronto = fprintf(controle, "%d %d %d\n", existia[0], existia[1], existia[2]) > 0;
    if (fflush(controle) != 0 || fsync(fileno(controle)) != 0)
        pronto = 0;
    if (fclose(controle) == EOF)
        pronto = 0;

    if (!pronto || rename("data/compra.pendente.tmp", "data/compra.pendente") != 0) {
        remove("data/compra.pendente.tmp");
        free(atualizados);
        return 0;
    }

    int sucesso = registrarVendaArquivo(venda, itens, totalItens);
    if (sucesso)
        sucesso = salvarProdutos(atualizados, totalProdutos);

    if (!sucesso) {
        printf(COR_VERMELHO "Compra não concluída. Tentando recuperar os arquivos...\n" COR_RESET);
        if (!recuperarCompraPendente())
            printf(COR_VERMELHO "Recuperação pendente. Não realize novas operações!\n" COR_RESET);
        free(atualizados);
        return 0;
    }

    if (remove("data/compra.pendente") != 0) {
        printf(COR_VERMELHO "Erro ao concluir a compra. Tentando recuperar...\n" COR_RESET);
        recuperarCompraPendente();
        free(atualizados);
        return 0;
    }

    memcpy(produtos, atualizados, (size_t)totalProdutos * sizeof(Produto));
    free(atualizados);

    for (int i = 0; i < 3; i++)
        remove(copiasCompra[i]);

    return 1;
}