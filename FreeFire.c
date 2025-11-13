#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes Globais ---
#define MAX_ITENS 10 // Capacidade máxima da mochila
#define TAM_NOME 30
#define TAM_TIPO 20

// 1. Definição da Struct
// Estrutura composta 'Item' para representar um objeto no inventário.
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;

// --- Variáveis Globais de Gerenciamento ---
// O vetor de structs representa a mochila (estrutura sequencial).
Item mochila[MAX_ITENS];
// O índice atual rastreia o número de itens presentes na mochila.
int totalItens = 0;

// --- Protótipos das Funções ---
void exibirMenu();
void limparBufferEntrada();
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();

// --- Função Principal (main) ---
int main() {
    int escolha;
    
    printf("--- Sistema de Inventário (Mochila) ---\n");
    printf("Capacidade máxima: %d itens.\n", MAX_ITENS);

    do {
        exibirMenu();
        printf("Escolha sua ação (0-4): ");
        // Leitura de dados: Usando scanf para inteiros
        if (scanf("%d", &escolha) != 1) {
            escolha = -1; // Sinaliza entrada inválida
            limparBufferEntrada();
        }
        limparBufferEntrada(); // Limpa o buffer após scanf

        switch (escolha) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: buscarItem(); break;
            case 0: printf("\nFechando a mochila. Até breve!\n"); break;
            default: printf("Ação inválida. Tente novamente.\n"); break;
        }
        
    } while (escolha != 0);

    return 0;
}

// --- Implementação das Funções Utilitárias e de Interface ---

void limparBufferEntrada() {
    int c;
    // Função utilitária para limpar o buffer, evita erros de leitura de strings após scanf()
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void exibirMenu() {
    // Usabilidade: Interface clara com mensagens orientativas.
    printf("\n--- MOCHILA VIRTUAL ---\n");
    printf("Itens atuais: %d/%d\n", totalItens, MAX_ITENS);
    printf(" [1] - Coletar/Cadastrar Item\n");
    printf(" [2] - Descartar/Remover Item\n");
    printf(" [3] - Listar Todos os Itens\n");
    printf(" [4] - Buscar Item pelo Nome\n");
    printf(" [0] - Sair do Sistema\n");
    printf("-------------------------\n");
}

// --- Implementação das Funções Obrigatórias ---

void inserirItem() {
    // Cadastro de itens: Verifica se há espaço na mochila.
    if (totalItens >= MAX_ITENS) {
        printf("\nERRO: A mochila está cheia! Descarte algo antes.\n");
        return;
    }

    printf("\n--- CADASTRO DE NOVO ITEM ---\n");

    // Preenche a próxima posição livre no vetor
    Item *novoItem = &mochila[totalItens];

    // Leitura do Nome (usando fgets() para melhor controle de buffer)
    printf("Digite o NOME do item (máx %d): ", TAM_NOME - 1);
    fgets(novoItem->nome, TAM_NOME, stdin);
    novoItem->nome[strcspn(novoItem->nome, "\n")] = 0; // Remove o '\n' lido pelo fgets

    // Leitura do Tipo
    printf("Digite o TIPO (arma, municao, cura): ");
    fgets(novoItem->tipo, TAM_TIPO, stdin);
    novoItem->tipo[strcspn(novoItem->tipo, "\n")] = 0; // Remove o '\n'

    // Leitura da Quantidade
    printf("Digite a QUANTIDADE: ");
    if (scanf("%d", &novoItem->quantidade) != 1 || novoItem->quantidade <= 0) {
        printf("ERRO: Quantidade inválida. Item não cadastrado.\n");
        // Reverte a alocação e corrige o totalItens
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();
    
    // Atualiza o contador de itens
    totalItens++;
    printf("\n✅ Item '%s' cadastrado com sucesso! (%d itens na mochila)\n", novoItem->nome, totalItens);
    listarItens(); // Listagem: Exibe os dados após a operação
}

void listarItens() {
    // Listagem dos itens registrados: Percorre o vetor e exibe os dados.
    if (totalItens == 0) {
        printf("\n🚫 A mochila está vazia.\n");
        return;
    }

    printf("\n--- ITENS NA MOCHILA (%d) ---\n", totalItens);
    printf("| %-4s | %-28s | %-18s | %-12s |\n", "POS", "NOME", "TIPO", "QUANTIDADE");
    printf("|------|------------------------------|--------------------|--------------|\n");
    
    for (int i = 0; i < totalItens; i++) {
        // Exibe os dados de cada struct Item
        printf("| %-4d | %-28s | %-18s | %-12d |\n", 
               i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    printf("--------------------------------------------------------------------------\n");
}

void removerItem() {
    // Remoção de itens: Localiza o item e rearranja o vetor.
    if (totalItens == 0) {
        printf("\n🚫 A mochila está vazia. Nada a remover.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o NOME do item a ser removido: ");
    // Leitura do Nome
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; // Remove o '\n'

    int indiceRemover = -1;

    // Busca sequencial para encontrar o índice do item
    for (int i = 0; i < totalItens; i++) {
        // strcasecmp é melhor, mas strcmp é padrão. Usamos strcmp para simplicidade.
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indiceRemover = i;
            break;
        }
    }

    if (indiceRemover != -1) {
        printf("✅ Item '%s' removido da mochila.\n", mochila[indiceRemover].nome);
        
        // Desloca todos os itens seguintes uma posição para trás para preencher o "buraco"
        for (int i = indiceRemover; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1]; // Cópia da struct
        }
        
        totalItens--; // Decrementa o contador
        listarItens(); // Listagem: Exibe os dados após a operação
    } else {
        printf("ERRO: Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

void buscarItem() {
    // Busca sequencial: Localiza o item pelo nome e exibe seus dados.
    if (totalItens == 0) {
        printf("\n🚫 A mochila está vazia. Nada a buscar.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n--- BUSCAR ITEM ---\n");
    printf("Digite o NOME do item a ser buscado: ");
    // Leitura do Nome
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; // Remove o '\n'

    int encontrado = 0;

    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n--- ITEM ENCONTRADO NA POSIÇÃO %d ---\n", i);
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("-------------------------------------\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}