#include <stdio.h>
#include <string.h>

// Arquivo de texto utilizado para persistencia dos dados
#define ARQ_TXT "produtos.txt"

// Codigos ANSI para cores e formatacao no terminal
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

// Funcao para realizar o cadastro de um novo produto
void cadastrarProduto()
{
    // Abertura do arquivo em modo append ("a") para adicionar ao final
    FILE *arquivo = fopen(ARQ_TXT, "a");
    struct Produto p;

    // Verificacao de erro ao abrir ou criar o arquivo
    if (arquivo == NULL)
    {
        printf("\n" VERMELHO "[ERRO]" RESET " Nao foi possivel abrir ou criar o arquivo.\n");
        return;
    }

    // Cadastro de informacoes do produto
    printf(BOLD"\n----- CADASTRO DE PRODUTO -----\n"RESET);

    printf("Codigo: ");
    scanf("%d", &p.codigo);
    while (getchar() != '\n'); // Limpa o buffer de entrada apos o numero

    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0'; // Remove o \n final capturado pelo fgets

    printf("Categoria: ");
    fgets(p.categoria, sizeof(p.categoria), stdin);
    p.categoria[strcspn(p.categoria, "\n")] = '\0'; // Remove o \n final capturado pelo fgets

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);

    printf("Preco: ");
    scanf("%f", &p.preco);

    getchar(); // Consome a quebra de linha pendente no buffer

    // Registro dos dados no arquivo (separados por ponto e virgula)
    fprintf(arquivo, "%d;%s;%s;%d;%f\n", p.codigo, p.nome, p.categoria, p.quantidade, p.preco);

    // Fecha o arquivo e informa o sucesso da gravacao
    fclose(arquivo);
    printf("\n" VERDE "[SUCESSO]" RESET " Produto cadastrado com sucesso!\n");
}

// Funcao para listar todos os produtos armazenados
void listarProdutos()
{
    // Abertura do arquivo em modo leitura ("r")
    FILE *arquivo = fopen(ARQ_TXT, "r");
    struct Produto p;

    // Verifica se o arquivo existe e pode ser lido
    if (arquivo == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Nenhum produto cadastrado ainda.\n");
        return;
    }

    printf(BOLD"\n----- LISTA DE PRODUTOS -----\n"RESET);

    // Leitura formatada dos registros linha por linha ate o final do arquivo
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

// Funcao para buscar um produto especifico pelo codigo
void buscarProduto()
{
    FILE *arquivo = fopen(ARQ_TXT, "r");
    struct Produto p;
    int codigoPesquisa;
    int encontrado = 0;

    // Verifica se o arquivo existe
    if (arquivo == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Nenhum produto cadastrado ainda.\n");
        return;
    }

    // Leitura do codigo para pesquisa
    printf("\nDigite o codigo do produto que deseja pesquisar: ");
    scanf("%d", &codigoPesquisa);

    printf(BOLD"\n----- PRODUTO -----\n"RESET);

    // Percorre o arquivo procurando o codigo correspondente
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
            break; // Interrompe a busca apos encontrar o produto
        }
    }

    // Notifica o usuario caso nenhum produto tenha sido localizado
    if (encontrado == 0)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Produto nao encontrado.\n");
    }

    fclose(arquivo);
}

// Funcao para calcular e exibir o valor total acumulado do estoque
void valorTotal()
{
    float total = 0.0f;
    FILE *arquivo = fopen(ARQ_TXT, "r");
    struct Produto p;

    // Verifica se o arquivo existe
    if (arquivo == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Nenhum produto cadastrado ainda.\n");
        return;
    }

    printf(BOLD"\n----- VALOR TOTAL DE ESTOQUE -----\n"RESET);

    // Percorre os produtos somando o valor total (quantidade * preco)
    while (fscanf(arquivo, "%d;%49[^;];%24[^;];%d;%f\n", &p.codigo, p.nome, p.categoria, &p.quantidade, &p.preco) == 5)
    {
        total += (p.quantidade * p.preco);
    }

    fclose(arquivo);
    printf("\nValor total do estoque: " VERDE "R$ %.2f" RESET "\n", total);
}

// Funcao para atualizar a quantidade em estoque (adicionar ou subtrair unidades)
void atualizarEstoque()
{
    // Abertura do arquivo original para leitura e criacao de um temporario para gravacao
    FILE *arquivo = fopen(ARQ_TXT, "r");
    FILE *temporario = fopen("temporario.txt", "w");
    struct Produto p;
    int codigoPesquisa, ajusteQtd, opcao;
    int encontrado = 0;

    // Verifica se os arquivos foram abertos corretamente
    if (arquivo == NULL || temporario == NULL)
    {
        printf("\n" AMARELO "[AVISO]" RESET " Erro ao abrir arquivo ou nenhum produto cadastrado.\n");
        return;
    }

    // Leitura do codigo do produto a ser modificado
    printf("\nDigite o codigo do produto: ");
    scanf("%d", &codigoPesquisa);

    // Selecao da operacao: adicao ou subtracao
    printf("\nDeseja " CIANO "[1]" RESET BOLD " adicionar" RESET " ou " CIANO "[2]" RESET BOLD " subtrair? " RESET);
    printf("\nDigite a opcao que deseja: ");
    scanf("%d", &opcao);

    // Percorre todos os produtos atualizando a quantidade do item desejado
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

        // Grava o registro (atualizado ou original) no arquivo temporario
        fprintf(temporario, "%d;%s;%s;%d;%f\n", p.codigo, p.nome, p.categoria, p.quantidade, p.preco);
    }

    // Fecha os arquivos antes de substituir
    fclose(arquivo);
    fclose(temporario);

    // Substitui o arquivo original pelo temporario atualizado
    remove(ARQ_TXT);
    rename("temporario.txt", ARQ_TXT);

    // Mensagem de retorno sobre o resultado da operacao
    if (encontrado)
    {
        printf("\n" VERDE "[SUCESSO]" RESET " Estoque atualizado com sucesso!\n");
    }
    else
    {
        printf("\n" AMARELO "[AVISO]" RESET " Produto nao encontrado.\n");
    }
}

// Exibicao do menu interativo e controle das opcoes
void menu()
{
    int opcao;

    do
    {
        // Menu de navegacao
        printf(BOLD "\n=*=*=*= GERENCIADOR DE ESTOQUE =*=*=*=\n" RESET);
        printf("\n" CIANO "[1]" RESET " Cadastrar Produto");
        printf("\n" CIANO "[2]" RESET " Listar Produtos");
        printf("\n" CIANO "[3]" RESET " Buscar Produto");
        printf("\n" CIANO "[4]" RESET " Calcular Valor de Estoque");
        printf("\n" CIANO "[5]" RESET " Atualizar Estoque");
        printf("\n" VERMELHO "[0]" RESET " Sair");

        printf("\n\n>>> Digite a opcao: ");
        scanf("%d", &opcao);

        // Tratamento da opcao selecionada
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

// Funcao principal (ponto de entrada da aplicacao)
int main(void)
{
    menu();
    return 0;
}
