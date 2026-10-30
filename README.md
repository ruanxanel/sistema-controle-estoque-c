# Sistema de Controle de Estoque em C

Sistema de controle de estoque desenvolvido em **linguagem C** como projeto acadêmico. O programa é executado no terminal e permite gerenciar produtos, acompanhar quantidades disponíveis, registrar entradas e saídas e consultar o histórico de movimentações.

## Objetivo

Aplicar conceitos de programação em C, especialmente **funções, estruturas (`struct`), vetores, ponteiros, alocação dinâmica de memória e manipulação de arquivos**.

## Funcionalidades

O sistema possui dois perfis de acesso, selecionados no menu principal:

| Funcionalidade | Administrador | Usuário |
| --- | :---: | :---: |
| Cadastrar produtos | ✅ | ❌ |
| Listar produtos | ✅ | ✅ |
| Buscar produto por código | ✅ | ✅ |
| Editar produtos | ✅ | ❌ |
| Excluir produtos | ✅ | ❌ |
| Registrar entrada no estoque | ✅ | ✅ |
| Registrar saída do estoque | ✅ | ✅ |
| Consultar produtos com estoque baixo | ✅ | ✅ |
| Consultar histórico de movimentações | ✅ | ✅ |

> **Observação:** os perfis são opções de menu, sem autenticação por senha. As operações disponíveis são controladas pelos respectivos menus.

### Informações dos produtos

Cada produto contém os seguintes campos:

- Código identificador
- Nome
- Preço
- Quantidade em estoque
- Estoque mínimo

O cadastro impede códigos duplicados e valida os principais campos de entrada. Nas movimentações, o sistema impede quantidades menores ou iguais a zero, saídas superiores ao estoque disponível e entradas que ultrapassem o limite do tipo `int`.

## Tecnologias e conceitos

- **Linguagem C** e compilador **GCC**
- **Makefile** para automatizar a compilação e a execução
- **`struct`** para representar produtos e movimentações
- **Vetores e ponteiros** para acessar e modificar dados
- **`malloc()`, `realloc()` e `free()`** para gerenciamento dinâmico de memória
- **`fgets()`, `strtol()` e `strtod()`** para leitura e validação de entradas
- **`fopen()`, `fread()`, `fwrite()` e `fclose()`** para persistência em arquivos binários

## Estrutura do projeto

```text
.
├── include/
│   ├── arquivo.h
│   ├── data.h
│   ├── entrada.h
│   ├── estoque.h
│   ├── menu.h
│   └── produto.h
├── src/
│   ├── arquivo.c
│   ├── data.c
│   ├── entrada.c
│   ├── estoque.c
│   ├── main.c
│   ├── menu.c
│   └── produto.c
├── data/
│   └── .gitkeep
├── .gitignore
├── Makefile
└── README.md
```

A pasta `build/` é criada automaticamente na compilação e não é enviada ao repositório.

## Como executar

### Pré-requisitos

- GCC instalado
- `make` instalado
- Terminal em Linux ou ambiente compatível com os comandos do Makefile

### Passo a passo

1. Clone o repositório:

   ```bash
   git clone <URL_DO_REPOSITORIO>
   cd <NOME_DA_PASTA>
   ```

2. Compile e execute:

   ```bash
   make run
   ```

Também é possível usar os comandos separadamente:

```bash
make        # Compila o programa
make run    # Executa o programa
make clean  # Remove os arquivos de compilação
```

> Execute os comandos a partir da **raiz do projeto**, porque o programa utiliza caminhos relativos para acessar a pasta `data/`.

## Armazenamento dos dados

Os dados são armazenados localmente em arquivos binários:

| Arquivo | Finalidade |
| --- | --- |
| `data/produtos.dat` | Armazena os produtos cadastrados e suas informações |
| `data/movimentacao.dat` | Armazena o histórico de entradas e saídas, com data e hora |

O sistema carrega os produtos ao iniciar e salva o cadastro **ao encerrar normalmente**. O histórico é gravado quando uma movimentação é registrada com sucesso.

Para reduzir o risco de sobrescrever um cadastro existente com dados incompletos, o salvamento dos produtos utiliza um arquivo temporário (`produtos.tmp`) antes de substituir `produtos.dat`.

**Limitação conhecida:** se o programa for encerrado à força, como com `Ctrl+C`, alterações recentes dos produtos podem não ser salvas, enquanto o histórico já pode ter sido atualizado. O projeto não implementa transações entre os dois arquivos.

## Exemplo de utilização

```text
SISTEMA DE CONTROLE DE ESTOQUE

1 - Administrador
2 - Usuário
0 - Sair

Escolha uma opção: 1
```

No painel do administrador, é possível cadastrar um produto informando código, nome, preço, quantidade e estoque mínimo. Após o cadastro, o produto pode ser consultado, editado, excluído ou ter seu estoque movimentado.

Quando a quantidade disponível fica **abaixo** do estoque mínimo, o sistema exibe um alerta.

## Organização do código

- **`main.c`** — inicialização, alocação e liberação da memória, carregamento e salvamento dos produtos.
- **`menu.c`** — menus do administrador e do usuário e encaminhamento das opções.
- **`produto.c`** — cadastro, listagem, busca, edição e exclusão de produtos.
- **`estoque.c`** — entradas, saídas e alertas de estoque baixo.
- **`entrada.c`** — leitura e validação de inteiros, textos e números decimais.
- **`arquivo.c`** — gravação e leitura dos produtos e do histórico.
- **`data.c`** — obtenção da data e hora das movimentações.
- **`include/`** — declarações de estruturas e funções utilizadas pelos módulos.

## Autores

- Ruan Henrique
- Rafael Angelo

**Turma:** 1º B

**Tipo:** Trabalho acadêmico — Sistema de Controle de Estoque em C.
