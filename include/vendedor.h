#ifdef VENDEDOR_H
#define VENDEDOR_H

typedef struct {
    int codigo;
    char nome[100];
    double percentualComissao;
} Vendedor;

#endif