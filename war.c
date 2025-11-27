// ============================================================================
//          PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
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
#include <stdlib.h> // malloc, calloc, free, rand, srand
#include <string.h> // strcpy, strcmp
#include <time.h>   // time
#include <locale.h> // setlocale

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define TAM_NOME 30
#define TAM_COR 10
#define QTD_TERRITORIOS 5

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int qtd);
void inicializarTerritorios(Territorio* mapa, int qtd);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int qtd);
void exibirMissao();
void limparBufferEntrada();

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, int qtd);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao();
int verificarVitoria();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    Territorio* mapa;
    int qtdTerritorios = QTD_TERRITORIOS; // Fixo em 5
    int opcao;

    printf("==========================================\n");
    printf("   WAR ESTRUTURADO - NIVEL AVENTUREIRO    \n");
    printf("==========================================\n");
    printf("   SISTEMA DE CONSTRUÇÃO DE TERRITÓRIOS   \n");
    printf("==========================================\n\n");

    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");
    
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    mapa = alocarMapa(qtdTerritorios);
    if (mapa == NULL) return 1;

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa, qtdTerritorios);

    // - Define a cor do jogador e sorteia sua missão secreta.
    sortearMissao(); // (Nível Mestre)

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
                // - Opção 1: Inicia a fase de ataque.
                faseDeAtaque(mapa, qtdTerritorios);
                break;
            case 2:
                // - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
                printf("\n(Funcionalidade de Nivel Mestre)\n");
                getchar();
                break;
            case 0:
                // - Opção 0: Encerra o jogo.
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
    int i;
    for(i = 0; i < qtd; i++) {
        printf("\n--- Cadastrando Territorio %d de %d ---\n", i + 1, qtd);
        
        // Leitura do Nome
        printf("Digite o nome do territorio (ex: America): ");
        scanf(" %[^\n]s", mapa[i].nome); 

        // Leitura da Cor
        printf("Digite a cor do exercito (ex: Azul): ");
        scanf(" %[^\n]s", mapa[i].cor);

        // Leitura das Tropas
        printf("Digite a quantidade de tropas: ");
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
    printf("2. Verificar Missao (Bloqueado)\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa, int qtd) {
    int i;
    printf("\n==========================================\n");
    printf("      RELATÓRIO ATUAL DE TERRITÓRIOS      \n");
    printf("==========================================\n");
    printf("| ID | %-15s | %-10s | %-6s |\n", "Nome", "Cor", "Tropas");
    printf("------------------------------------------\n");

    for(i = 0; i < qtd; i++) {
        printf("| %02d | %-15s | %-10s | %-6d |\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("==========================================\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao() {
    // (Nível Mestre)
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa, int qtd) {
    int idAtk, idDef;

    printf("\n--- PREPARAR ATAQUE ---\n");
    printf("Digite o ID do territorio ATACANTE (1 a %d): ", qtd);
    scanf("%d", &idAtk);

    printf("Digite o ID do territorio DEFENSOR (1 a %d): ", qtd);
    scanf("%d", &idDef);

    int idxAt = idAtk - 1;
    int idxDf = idDef - 1;

    if (idxAt >= 0 && idxAt < qtd && idxDf >= 0 && idxDf < qtd) {
        simularAtaque(&mapa[idxAt], &mapa[idxDf]);
    } else {
        printf("[Erro] IDs invalidos!\n");
    }
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    // Validação: Fogo amigo
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[Bloqueado] Voce nao pode atacar sua propria cor (%s)!\n", atacante->cor);
        return;
    }

    // Validação: Tropas insuficientes
    if (atacante->tropas <= 1) {
        printf("\n[Bloqueado] Tropas insuficientes no territorio %s para atacar.\n", atacante->nome);
        return;
    }

    printf("\n>>> BATALHA: %s vs %s <<<\n", atacante->nome, defensor->nome);
    
    // Simulação dos dados
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;
    printf("Dados -> Atacante: %d | Defensor: %d\n", dadoAtk, dadoDef);

    if (dadoAtk > dadoDef) {
        printf("RESULTADO: Vitoria do Atacante!\n");
        // Conquista: Troca a cor
        strcpy(defensor->cor, atacante->cor);
        
        // Move metade das tropas
        int move = atacante->tropas / 2;
        defensor->tropas = move;
        atacante->tropas -= move;
        printf("O territorio %s agora pertence ao exercito %s.\n", defensor->nome, defensor->cor);
    } else {
        printf("RESULTADO: Vitoria da Defesa!\n");
        // Penalidade
        atacante->tropas -= 1;
        printf("O atacante perdeu 1 tropa.\n");
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    // (Nível Mestre)
    return 0;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria() {
    // (Nível Mestre)
    return 0;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}