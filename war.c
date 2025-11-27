// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ===========================================================================
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h> // Para strcpy, strcmp
#include <stdlib.h> // Para malloc, calloc, free, rand, srand
#include <time.h>   // Para time(NULL)
#include <locale.h> // Necessário para setlocale
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define TAM_NOME 30
#define TAM_COR 10
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct  {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;
// --- Protótipos das Funções ---
void cadastrarTerritorios(Territorio* mapa, int qtd);
void exibirTerritorios(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int qtd);
void inicializarTerritorios(Territorio* mapa, int qtd);
void liberarMemoria(Territorio* mapa);
// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int qtd);
void limparBufferEntrada();
// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, int qtd);
void simularAtaque(Territorio* atacante, Territorio* defensor);
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    Territorio* mapa; // Ponteiro para o início do vetor dinâmico
    int qtdTerritorios;
    int opcao;
    
    printf("=== WAR: NIVEL AVENTUREIRO ===\n");
    printf("==========================================\n");
    printf("   SISTEMA DE CONSTRUÇÃO DE TERRITÓRIOS   \n");
    printf("==========================================\n\n");
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");    
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    printf("Quantos territorios tera no jogo? ");
    scanf("%d", &qtdTerritorios);

    mapa = alocarMapa(qtdTerritorios);
    if (mapa == NULL) return 1; // Encerra se falhar memória
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa, qtdTerritorios);
    // - Define a cor do jogador e sorteia sua missão secreta.
    //(Nível Mestre)

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    do {
     // - A cada iteração, exibe o mapa, a missão e o menu de ações.
        exibirMapa(mapa, qtdTerritorios);
        exibirMenuPrincipal();
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
            //   - Opção 1: Inicia a fase de ataque.
            faseDeAtaque(mapa, qtdTerritorios);
            break;
             //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.(Nivel Mestre)
            case 0
             //   - Opção 0: Encerra o jogo.
             printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    } while (opcao != 0);
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
 }
// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(int qtd) {
    Territorio* p = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (p == NULL) {
        printf("[Erro] Memoria insuficiente!\n");
    }
    return p;
}
// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa, int qtd) {
    for(int i = 0; i < qtd; i++) {
        printf("\n--- Cadastrando Territorio %d de %d ---\n", i + 1, qtd);
        
        printf("Nome: ");
        scanf(" %[^\n]s", mapa[i].nome); // Acesso via ponteiro implícito do vetor

        printf("Cor: ");
        scanf(" %[^\n]s", mapa[i].cor);

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}
// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\n[Sistema] Memoria liberada.\n");
    }
}
// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missao (Em breve)\n");
    printf("0. Sair\n");
    printf("Escolha: ");
}
// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa, int qtd) {
    printf("\n----------------------------------------------------------\n");
    printf("| ID | %-15s | %-10s | %-6s |\n", "Nome", "Cor", "Tropas");
    printf("----------------------------------------------------------\n");
    for(int i = 0; i < qtd; i++) {
        printf("| %02d | %-15s | %-10s | %-6d |\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("----------------------------------------------------------\n");
}
// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.(Nível Mestre)

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa, int qtd) {
    int idAtk, idDef;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("ID do Atacante (1-%d): ", qtd);
    scanf("%d", &idAtk);
    printf("ID do Defensor (1-%d): ", qtd);
    scanf("%d", &idDef);

    int idxAt = idAtk - 1;
    int idxDf = idDef - 1;

    if (idxAt >= 0 && idxAt < qtd && idxDf >= 0 && idxDf < qtd) {
        simularAtaque(&mapa[idxAt], &mapa[idxDf]);
    } else {
        printf("IDs invalidos!\n");
    }
}
// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    // Validação
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Voce nao pode atacar aliados!\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("Tropas insuficientes para atacar!\n");
        return;
    }

    // Dados
    int dAtk = (rand() % 6) + 1;
    int dDef = (rand() % 6) + 1;
    printf("\nRolagem: Atq [%d] x Def [%d]\n", dAtk, dDef);

    // Resolução
    if (dAtk > dDef) {
        printf("Vitoria do Atacante!\n");
        strcpy(defensor->cor, atacante->cor); // Troca dono
        int move = atacante->tropas / 2;
        defensor->tropas = move;
        atacante->tropas -= move;
        printf("Territorio conquistado!\n");
    } else {
        printf("Defesa venceu!\n");
        atacante->tropas -= 1;
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}