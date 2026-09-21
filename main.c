#include <stdio.h>
#include <string.h>

// Nome do arquivo para persistencia dos dados
#define ARQ_TXT "produtos.txt"

// Cores e formatacao para o terminal (ANSI)
#define VERMELHO "\033[31m"
#define VERDE    "\033[32m"
#define RESET    "\033[0m"
#define CIANO    "\033[36m"
#define AMARELO  "\033[33m"
#define BOLD     "\033[1m"

// Estrutura que representa um produto no estoque
struct Produto
{
    int codigo;
    char nome[50];
    char categoria[25];
    int quantidade;
    float preco;
};

// Cadastra um novo produto e salva no final do arquivo
void cadastrarProduto()
{
    FILE *arquivo = fopen(ARQ_TXT, "a");
    struct Produto p;

    if (arquivo == NULL)
    {
        printf("\n" VERMELHO "[ERRO]" RESET " Nao foi possivel abrir ou criar o arquivo.\n");
        return;
    }

    // Coleta dos dados do produto
    printf(BOLD"\n----- CADASTRO DE PRODUTO -----\n"RESET);

    printf("Codigo: ");
    scanf("%d", &p.codigo);
    while (getchar() != '\n'); // Limpa o buffer do teclado

    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0'; // Remove o \n capturado pelo fgets

    printf("Categoria: ");
    fgets(p.categoria, sizeof(p.categoria), stdin);
    p.categoria[strcspn(p.categoria, "\n")] = '\0'; // Remove o \n capturado pelo fgets

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);

    printf("Preco: ");
    scanf("%f", &p.preco);

    getchar(); // Consome o \n residual do scanf

    // Grava os dados separados por ponto e virgula
    fprintf(arquivo, "%d;%s;%s;%d;%f\n", p.codigo, p.nome, p.categoria, p.quantidade, p.preco);

    fclose(arquivo);
    printf("\n" VERDE "[SUCESSO]" RESET " Produto cadastrado com sucesso!\n");
}

// Lista todos os produtos cadastrados no arquivo
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

    // Le cada linha do arquivo ate o fim
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

// Busca um produto especifico pelo codigo
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

    // Percorre os registros buscando o codigo informado
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

// Calcula e exibe o valor total acumulado do estoque
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

    // Soma a multiplicacao de quantidade por preco de cada item
    while (fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco) == 5)
    {
        total += (p.quantidade * p.preco);
    }

    fclose(arquivo);
    printf("\nValor total do estoque: " VERDE "R$ %.2f" RESET "\n", total);
}

// Atualiza a quantidade em estoque (adicionando ou subtraindo)
void atualizarEstoque()
{
    // Abre o arquivo atual para leitura e um temporario para escrita
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

    // Copia os dados para o temporario, alterando apenas o produto pesquisado
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

        // Salva o registro no arquivo temporario
        fprintf(temporario, "%d;%s;%s;%d;%f\n", p.codigo, p.nome, p.categoria, p.quantidade, p.preco);
    }

    fclose(arquivo);
    fclose(temporario);

    // Substitui o arquivo original pelo temporario atualizado
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

// Menu principal com o loop de operacoes
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

// Funcao principal
int main(void)
{
    menu();
    return 0;
}
