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
#include <string.h> // strcpy, strcmp, strstr
#include <time.h>   // time
#include <locale.h> // setlocale

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings.
#define TAM_NOME 30
#define TAM_COR 10
#define QTD_TERRITORIOS 5
#define TAM_MISSAO 100

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
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void liberarMemoria(Territorio* mapa, char* missao);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int qtd);
void exibirMissao(const char* missao);
void limparBufferEntrada();

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, int qtd, const char* missao, int* jogoAtivo);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int verificarMissao(const char* missao, Territorio* mapa, int tamanho);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    Territorio* mapa;
    char* missaoJogador;
    int qtdTerritorios = QTD_TERRITORIOS; // Fixo em 5
    int opcao;
    int jogoAtivo = 1; // Controle do loop do jogo

    // Lista de missões possíveis (Nível Mestre)
    char* listaMissoes[] = {
        "Conquistar a maioria dos territorios (3 ou mais)",
        "Eliminar totalmente o exercito Vermelho",
        "Conquistar a maioria dos territorios (3 ou mais)",
        "Eliminar totalmente o exercito Vermelho",
        "Manter tropas em todos os territorios"
    };

    printf("==========================================\n");
    printf("      WAR ESTRUTURADO - NIVEL MESTRE      \n");
    printf("==========================================\n");
    printf("   SISTEMA DE CONSTRUÇÃO DE TERRITÓRIOS   \n");
    printf("==========================================\n\n");

    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // - Aloca a memória para o mapa do mundo
    mapa = alocarMapa(qtdTerritorios);
    if (mapa == NULL) return 1;

    // - Aloca memória para a missão do jogador (Requisito Nível Mestre)
    missaoJogador = (char*) malloc(TAM_MISSAO * sizeof(char));
    if (missaoJogador == NULL) {
        free(mapa);
        return 1;
    }

    // - Preenche os territórios com os dados iniciais.
    inicializarTerritorios(mapa, qtdTerritorios);

    // - Define a cor do jogador e sorteia sua missão secreta.
    atribuirMissao(missaoJogador, listaMissoes, 5);

    // Exibe a missão uma vez no início
    exibirMissao(missaoJogador);
    
    printf("\nPressione ENTER para iniciar o jogo...");
    limparBufferEntrada();
    getchar();

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        exibirMapa(mapa, qtdTerritorios);
        exibirMenuPrincipal();
        
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                // - Opção 1: Inicia a fase de ataque (Verifica missão dentro dela)
                faseDeAtaque(mapa, qtdTerritorios, missaoJogador, &jogoAtivo);
                break;
            case 2:
                // - Opção 2: Consulta a missão atual
                printf("\n--- CONSULTAR MISSAO ---\n");
                printf("Objetivo: %s\n", missaoJogador);
                if (verificarMissao(missaoJogador, mapa, qtdTerritorios)) {
                    printf("Status: COMPLETA (Voce venceu!)\n");
                    jogoAtivo = 0;
                } else {
                    printf("Status: Em andamento...\n");
                }
                printf("Pressione Enter...");
                getchar();
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                jogoAtivo = 0;
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (jogoAtivo);

    // 3. Limpeza:
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
Territorio* alocarMapa(int qtd) {
    Territorio* p = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (p == NULL) {
        printf("[Erro] Memoria insuficiente!\n");
    }
    return p;
}

// inicializarTerritorios():
// Preenche os dados iniciais mantendo o estilo de perguntas solicitado.
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
        printf("Digite a quantidade de tropas(1 a 9): ");
        scanf("%d", &mapa[i].tropas);
    }
}

// atribuirMissao():
// Sorteia uma missão e copia para a variável do jogador.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// liberarMemoria():
// Libera a memória do mapa e da missão.
void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa != NULL) free(mapa);
    if (missao != NULL) free(missao);
    printf("\n[Sistema] Memoria liberada com sucesso.\n");
}

// exibirMenuPrincipal():
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1. Atacar (3 Tentativas)\n");
    printf("2. Consultar Missao\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// exibirMapa():
void exibirMapa(const Territorio* mapa, int qtd) {
    int i;
    printf("\n==========================================\n");
    printf("      RELATÓRIO ATUAL DE TERRITÓRIOS      \n");
    printf("==========================================\n");
    printf("| ID | %-15s | %-10s | %-6s |\n", "Nome", "Cor", "Tropas");
    printf("------------------------------------------\n");

    for(i = 0; i < qtd; i++) {
        printf("| %02d | %-15s | %-10s | %-6d |\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("==========================================\n");
}

// exibirMissao():
void exibirMissao(const char* missao) {
    printf("\n**************************************************\n");
    printf(" SUA MISSAO SECRETA: %s\n", missao);
    printf("**************************************************\n");
}

// faseDeAtaque():
// Gerencia o ataque e verifica vitória.
// ALTERADO: Pausa após cada ataque e pausa final após 3 tentativas.
void faseDeAtaque(Territorio* mapa, int qtd, const char* missao, int* jogoAtivo) {
    int idAtk, idDef;
    int i;

    // Mostra a missão novamente para o jogador lembrar
    printf("\n>>> LEMBRETE DA MISSAO: %s <<<\n", missao);

    // Loop de 3 Tentativas
    for(i = 0; i < 3; i++) {
        if (*jogoAtivo == 0) break; // Se ganhou, para de perguntar

        printf("\n--- TENTATIVA DE ATAQUE %d DE 3 ---\n", i + 1);
        printf("Digite o ID do territorio ATACANTE (1 a %d) ou 0 para parar: ", qtd);
        scanf("%d", &idAtk);
        if (idAtk == 0) break; // Opção de saída antecipada

        printf("Digite o ID do territorio DEFENSOR (1 a %d): ", qtd);
        scanf("%d", &idDef);

        int idxAt = idAtk - 1;
        int idxDf = idDef - 1;

        if (idxAt >= 0 && idxAt < qtd && idxDf >= 0 && idxDf < qtd) {
            simularAtaque(&mapa[idxAt], &mapa[idxDf]);
            
            // Verificação silenciosa de vitória após o ataque
            if (verificarMissao(missao, mapa, qtd)) {
                printf("\n#################################################\n");
                printf(" PARABENS! VOCE CUMPRIU SUA MISSAO: \n %s\n", missao);
                printf("#################################################\n");
                *jogoAtivo = 0; // Encerra o jogo
            }

        } else {
            printf("[Erro] IDs invalidos!\n");
        }

        // --- PAUSA PARA LER O RESULTADO (AQUI DENTRO DO LOOP) ---
        if (*jogoAtivo) {
            printf("\n(Pressione ENTER para continuar para a proxima tentativa...)");
            limparBufferEntrada();
            getchar();
        }
    }

    // --- PAUSA FINAL PARA CONCLUIR A RODADA ---
    if (*jogoAtivo) {
        printf("\n>>> Rodada de ataques finalizada. Pressione ENTER para voltar ao menu...");
        // O buffer pode estar sujo dependendo de como o usuário saiu do loop, 
        // mas o getchar() anterior geralmente resolve.
        // Se saiu pelo "break" do 0, precisa limpar.
        if (i < 3) limparBufferEntrada(); 
        getchar();
    }
}

// simularAtaque():
// ALTERADO: Texto dos dados mais explícito.
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
    
    printf("Ambos jogaram os dados...\n");
    printf("RESULTADO: O Atacante tirou %d e o Defensor tirou %d.\n", dadoAtk, dadoDef);

    if (dadoAtk > dadoDef) {
        printf("VITORIA DO ATACANTE!\n");
        strcpy(defensor->cor, atacante->cor);
        
        int move = atacante->tropas / 2;
        defensor->tropas = move;
        atacante->tropas -= move;
        printf("O territorio %s foi conquistado e agora pertence ao exercito %s.\n", defensor->nome, defensor->cor);
    } else {
        printf("VITORIA DA DEFESA!\n");
        atacante->tropas -= 1;
        printf("O atacante perdeu 1 tropa na batalha.\n");
    }
}

// verificarMissao():
// Verifica se o jogador cumpriu os requisitos.
int verificarMissao(const char* missao, Territorio* mapa, int tamanho) {
    int i;
    int contaAzul = 0;
    int contaVermelho = 0;

    // Coleta estatísticas do mapa
    for(i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, "Azul") == 0) contaAzul++;
        if (strcmp(mapa[i].cor, "Vermelho") == 0) contaVermelho++;
    }

    // Lógica para missão "Maioria" (Assumindo jogador Azul)
    if (strstr(missao, "maioria") != NULL) {
        if (contaAzul >= 3) return 1; 
    }

    // Lógica para missão "Eliminar Vermelho" (Assumindo jogador Azul)
    if (strstr(missao, "Vermelho") != NULL) {
        if (contaVermelho == 0) return 1;
    }
    
    // Lógica para missão "Manter tropas"
    if (strstr(missao, "Manter") != NULL) {
        if (contaAzul > 0) return 0; // Missão contínua
    }

    return 0;
}

// limparBufferEntrada():
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}