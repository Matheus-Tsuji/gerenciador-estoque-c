#include <stdio.h>
#include <string.h>

#define ARQ_TXT "produtos.txt"

#define VERMELHO "\033[31m"
#define VERDE    "\033[32m"
#define RESET    "\033[0m"
#define CIANO    "\033[36m"
#define AMARELO  "\033[33m"
#define BOLD    "\033[1m"

struct Produto
{
    int codigo;
    char nome[50];
    char categoria[25];
    int quantidade;
    float preco;
};

void cadastrarProduto()
{
    FILE *arquivo = fopen(ARQ_TXT, "a");
    struct Produto p;

    if (arquivo == NULL)
    {
        printf("\n" VERMELHO "[ERRO]" RESET " Nao foi possivel abrir ou criar o arquivo.\n");
        return;
    }

    // Cadastro de informações
    printf(BOLD"\n----- CADASTRO DE PRODUTO -----\n"RESET);

    printf("Codigo: ");
    scanf("%d", &p.codigo);
    while (getchar() != '\n');

    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("Categoria: ");
    fgets(p.categoria, sizeof(p.categoria), stdin);
    p.categoria[strcspn(p.categoria, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);

    printf("Preco: ");
    scanf("%f", &p.preco);

    getchar();

    // Registro no arquivo
    fprintf(arquivo, "%d;%s;%s;%d;%f\n", p.codigo, p.nome, p.categoria, p.quantidade, p.preco);

    fclose(arquivo);
    printf("\n" VERDE "[SUCESSO]" RESET " Produto cadastrado com sucesso!\n");
}

void listarProdutos()
{
    FILE *arquivo = fopen(ARQ_TXT, "r");
    struct Produto p;

    if (arquivo == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Nenhum produto cadastrado ainda.\n");
        return;
    }

    printf(BOLD"\n----- LISTA DE PRODUTOS -----\n"RESET);

    while (fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco) == 5)
    {
        printf("\nCodigo: %d", p.codigo);
        printf("\nNome: %s", p.nome);
        printf("\nCategoria: %s", p.categoria);
        printf("\nQuantidade: %d", p.quantidade);
        printf("\nPreco unitario: %.2f\n", p.preco);
        printf("\n-----------------------------\n");
    }

    fclose(arquivo);
}

void buscarProduto()
{
    FILE *arquivo = fopen(ARQ_TXT, "r");
    struct Produto p;
    int codigoPesquisa;
    int encontrado = 0;

    if (arquivo == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Nenhum produto cadastrado ainda.\n");
        return;
    }

    printf("\nDigite o codigo do produto que deseja pesquisar: ");
    scanf("%d", &codigoPesquisa);

    printf(BOLD"\n----- PRODUTO -----\n"RESET);
    while (fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco) == 5)
    {
        if (p.codigo == codigoPesquisa)
        {
            printf("\nCodigo: %d", p.codigo);
            printf("\nNome: %s", p.nome);
            printf("\nCategoria: %s", p.categoria);
            printf("\nQuantidade: %d", p.quantidade);
            printf("\nPreco unitario: %.2f\n", p.preco);
            encontrado = 1;
            break;
        }
    }

    if (encontrado == 0)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Produto nao encontrado.\n");
    }

    fclose(arquivo);
}

void valorTotal()
{
    float total = 0.0f;
    FILE *arquivo = fopen(ARQ_TXT, "r");
    struct Produto p;

    if (arquivo == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Nenhum produto cadastrado ainda.\n");
        return;
    }

    printf(BOLD"\n----- VALOR TOTAL DE ESTOQUE -----\n"RESET);
    while (fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco) == 5)
    {
        total += (p.quantidade * p.preco);
    }

    fclose(arquivo);
    printf("\nValor total do estoque: " VERDE "R$ %.2f" RESET "\n", total);
}

void atualizarEstoque()
{
    FILE *arquivo = fopen(ARQ_TXT, "r");
    FILE *temporario = fopen("temporario.txt", "w");
    struct Produto p;
    int codigoPesquisa, ajusteQtd, opcao;
    int encontrado = 0;

    if (arquivo == NULL || temporario == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Erro ao abrir arquivo ou nenhum produto cadastrado.\n");
        return;
    }

    printf("\nDigite o codigo do produto: ");
    scanf("%d", &codigoPesquisa);

    printf("\nDeseja " CIANO "[1]" RESET BOLD " adicionar" RESET " ou " CIANO "[2]" RESET BOLD " subtrair? " RESET);
    printf("\nDigite a opcao que deseja: ");
    scanf("%d", &opcao);

    while (fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco) == 5)
    {
        if (p.codigo == codigoPesquisa)
        {
            encontrado = 1;
            printf(BOLD"\n----- ATUALIZANDO ESTOQUE -----\n"RESET);
            printf("\nQuantidade atual: %d\n", p.quantidade);
            if (opcao == 1)
            {
                printf("\nQuantidade a adicionar: ");
                scanf("%d", &ajusteQtd);
                p.quantidade += ajusteQtd;
            }
            else
            {
                printf("\nQuantidade a subtrair: ");
                scanf("%d", &ajusteQtd);
                p.quantidade -= ajusteQtd;
            }
        }

        fprintf(temporario, "%d;%s;%s;%d;%f\n", p.codigo, p.nome, p.categoria, p.quantidade, p.preco);
    }

    fclose(arquivo);
    fclose(temporario);

    remove(ARQ_TXT);
    rename("temporario.txt", ARQ_TXT);

    if (encontrado)
    {
        printf("\n" VERDE "[SUCESSO]" RESET " Estoque atualizado com sucesso!\n");
    }
    else
    {
        printf("\n" AMARELO "[AVISO]" RESET " Produto nao encontrado.\n");
    }
}

void menu()
{
    int opcao;

    do
    {
        printf(BOLD "\n=*=*=*= GERENCIADOR DE ESTOQUE =*=*=*=\n" RESET);
        printf("\n" CIANO "[1]" RESET " Cadastrar Produto");
        printf("\n" CIANO "[2]" RESET " Listar Produtos");
        printf("\n"  CIANO "[3]" RESET " Buscar Produto");
        printf("\n" CIANO "[4]" RESET " Calcular Valor de Estoque");
        printf("\n" CIANO "[5]" RESET " Atualizar Estoque");
        printf("\n" VERMELHO "[0]" RESET " Sair");

        printf("\n\n>>> Digite a opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                buscarProduto();
                break;
            case 4:
                valorTotal();
                break;
            case 5:
                atualizarEstoque();
                break;
            case 0:
                printf(CIANO "\n\nSaindo do sistema...\n\n" RESET);
                break;
            default:
                printf("\n" VERMELHO "[ERRO]" RESET " Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}

int main(void)
{
    menu();
    return 0;
}
