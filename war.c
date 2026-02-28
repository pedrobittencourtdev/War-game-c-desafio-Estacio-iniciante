//---Construir a base de dados dos territórios---//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <locale.h>

// Definições de cores de texto (Foreground)
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//DEfinição de cores de fundo (Background)
#define ANSI_BG_RED     "\x1b[41m"
#define ANSI_BG_GREEN   "\x1b[42m"
#define ANSI_BG_YELLOW  "\x1b[43m"
#define ANSI_BG_BLUE    "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN    "\x1b[46m"
#define ANSI_BG_RESET   "\x1b[0m"


//---Definição da estrutura Território---//

typedef struct {
    char nome[30]; // Campo para armazenar o nome do território, que será usado para exibir o nome do território no mapa e nas mensagens do jogo
    char cor_Exercito[10]; // Campo para armazenar a cor do exército presente no território, que será usada para formatação de cores no mapa e nas mensagens do jogo
    int num_Tropas; // Campo para armazenar o número de tropas presentes no território
    char* missao; // Campo para armazenar a missão do território (opcional, pode ser usado para futuras expansões do jogo)
    int conquistas; // Campo para indicar se o território foi conquistado (0 para não conquistado, 1 para conquistado)
} territorios;

// Struct para facilitar a busca de cores
typedef struct {
    char *nome;
    char *codigo;
} CorMapa;
// Tabela de referência para as cores
 CorMapa mapaCores[] = {
        {"vermelho", ANSI_COLOR_RED},
        {"verde",    ANSI_COLOR_GREEN},
        {"azul",     ANSI_COLOR_BLUE},
        {"amarelo",  ANSI_COLOR_YELLOW},
        {"magenta",  ANSI_COLOR_MAGENTA},
        {"ciano",    ANSI_COLOR_CYAN}
    };// Tabela de referência para as cores


//---Declaração das funções---//

void obterCodigoCor(char* corExército, CorMapa* mapaCores, char* saida);
void limparBuffer();
void quantidadeTerritorios(int* totalTerritorios);
void cadastrarTerritorios(territorios* lista_Territorios, int totalTerritorios);
void exibirMapa(territorios* lista_Territorios, int totalTerritorios);
void menuBatalha( territorios* lista_Territorios, int totalTerritorios , int* opcao);
void lerEscolhas(int* escolha1, int* escolha2, int totalTerritorios);
void atacar(territorios* atacante, territorios* defensor);
void eliminarTerritorio(territorios* lista_Territorios, int* totalTerritorios, int indice);
int vencedor (territorios* lista_Territorios, int *totalTerritorios);
void liberarMemoria(territorios* mapa, int totalTerritorios, char* vetorMissoes[], int totalMissoes);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(territorios* territorio, char* vetorMissoes[], int totalTerritorios);

// Vetor de missões para os territórios
char* vetorMissoes[2] = {
    ANSI_COLOR_CYAN"Seja o último território restante no jogo." ANSI_COLOR_RESET,
    ANSI_COLOR_CYAN"Conquiste um território." ANSI_COLOR_RESET
};




//---Função principal---//

int main() {

    setlocale(LC_ALL, "Portuguese"); // Configura a localidade para Português
    SetConsoleOutputCP(CP_UTF8); // Configura a saída do console para UTF-8 para suportar caracteres acentuados
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    int escolha1, escolha2, opcao; // Variáveis para armazenar as escolhas dos jogadores e a opção do menu

    // Variáveis para os territórios
    territorios *t= NULL; // Ponteiro para a lista de territórios
    territorios *lista_Territorios;
    int totalTerritorios; // Variável para armazenar a quantidade total de territórios

    quantidadeTerritorios(&totalTerritorios);

    // Aloca memória para a lista de territórios
    lista_Territorios = (territorios*)calloc(totalTerritorios, sizeof(territorios));
    // Verifica se a alocação foi bem-sucedida
    if (lista_Territorios == NULL) {
        printf(ANSI_COLOR_RED"Erro ao alocar memória para os territórios.\n" ANSI_COLOR_RESET);
        return 1;
    }
 
    // Loop para cadastrar os territórios
    cadastrarTerritorios(lista_Territorios, totalTerritorios);


    // Loop para permitir múltiplas batalhas até que o jogador escolha sair
    do {
        printf("\033[H\033[J"); // Limpa a tela do console
        exibirMapa(lista_Territorios, totalTerritorios);
        menuBatalha(lista_Territorios, totalTerritorios, &opcao);
        switch (opcao){
            case 1:
                printf(ANSI_COLOR_GREEN"\nIniciando o confronto...\n" ANSI_COLOR_RESET);
                Sleep(1000); // Pausa de 1 segundo para criar uma sensação de processamento
                break;
            case 2:
                printf(ANSI_COLOR_YELLOW"\nVerificando missões...\n" ANSI_COLOR_RESET);
                // Loop para imprimir a missão de cada jogador vivo no mapa
            for (int i = 0; i < totalTerritorios; i++) {
                printf("Exército %s: %s\n", lista_Territorios[i].cor_Exercito, lista_Territorios[i].missao);
            }
            
            // Pausa para o jogador conseguir ler antes do loop recomeçar e limpar a tela
            printf(ANSI_COLOR_YELLOW "\nPressione ENTER para voltar ao menu..." ANSI_COLOR_RESET);
            limparBuffer();
            getchar();
                break;
            case 3:
                printf(ANSI_COLOR_CYAN"\nAJUDA:\n" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_CYAN"SOBRE O JOGO: O jogo consiste em conquistar territórios e cumprir missões para vencer o jogo.\n" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_CYAN"1. Realizar ataque: Permite escolher um território atacante e um defensor para iniciar uma batalha.\n" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_CYAN"2. Verificar missões: Exibe a missão de cada território vivo no mapa para que os jogadores possam acompanhar seus objetivos.\n" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_CYAN"0. Sair: Encerra o jogo e libera a memória alocada.\n" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_YELLOW "\nPressione ENTER para voltar ao menu..." ANSI_COLOR_RESET);
                limparBuffer();
                getchar();
                break;
            case 0:
                printf(ANSI_COLOR_YELLOW"\nEncerrando o jogo. Obrigado por jogar!\n" ANSI_COLOR_RESET);
                break;
            default:
                printf(ANSI_COLOR_RED"\nOpção inválida. Por favor, escolha uma opção válida.\n" ANSI_COLOR_RESET);
                break;
        }
        // Verifica se o jogador escolheu iniciar um confronto ou sair, e executa a ação correspondente
        if(opcao == 1){
            int ataqueValido = 0;
            // Loop para garantir que o jogador escolha um território atacante válido (com tropas)
            do {
            // Loop que insiste até o jogador escolher exércitos diferentes
                lerEscolhas(&escolha1, &escolha2, totalTerritorios);
                
                // Verifica se as cores são iguais ANTES de ir para a batalha
                if (strcmp(lista_Territorios[escolha1 - 1].nome, lista_Territorios[escolha2 - 1].nome) == 0) {
                    printf(ANSI_COLOR_RED "\nErro: Não é possível atacar um território do seu próprio exército!\n" ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW "Por favor, escolha novamente.\n" ANSI_COLOR_RESET);
                } else {
                    ataqueValido = 1; // Exércitos diferentes! Pode sair do loop.
                }
            } while (ataqueValido == 0);

            atacar(&lista_Territorios[ escolha1 - 1], &lista_Territorios[escolha2 - 1]);
            
            // --- Verifica quem morreu após o ataque e elimina ---
            if (lista_Territorios[escolha2 - 1].num_Tropas <= 0) {
                eliminarTerritorio(lista_Territorios, &totalTerritorios, escolha2 - 1);
            } 
            else if (lista_Territorios[escolha1 - 1].num_Tropas <= 0) {
                eliminarTerritorio(lista_Territorios, &totalTerritorios, escolha1 - 1);
            }

            // --- VERIFICAÇÕES DE VITÓRIA --- //
            int missaoCumprida = 0;
            
            // Varre TODOS os territórios vivos para ver se alguém completou a missão
            for (int i = 0; i < totalTerritorios; i++) {
                if (verificarMissao(&lista_Territorios[i], vetorMissoes, totalTerritorios) == 1) {
                    missaoCumprida = 1;
                    break; 
                }
            }
            
            if (missaoCumprida == 1) {
                break; // Sai do loop principal do jogo para liberar a memória
            }

            // Verifica se ganhou por ELIMINAÇÃO (sendo o último a sobrar)
            if (vencedor(lista_Territorios, &totalTerritorios) == 1) {
                break; // Sai do loop principal do jogo
            }
            printf(ANSI_COLOR_YELLOW "\nPressione ENTER para continuar para a proxima rodada..." ANSI_COLOR_RESET);
            getchar(); // Espera o jogador apertar Enter
        }
    } while (opcao != 0); // Loop infinito para permitir múltiplas batalhas

liberarMemoria(lista_Territorios, totalTerritorios, vetorMissoes, 2); // Libera a memória antes de encerrar o programa

return 0;
}


//---FUNÇÕES DO JOGO---//

// Função para limpar o buffer do teclado.
void limparBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para solicitar a quantidade de territórios a serem cadastrados.
void quantidadeTerritorios(int* totalTerritorios) {

    do {
        printf("\nQuantos territórios deseja cadastrar? (Minimo de 2): ");
        scanf("%d", totalTerritorios);
        limparBuffer(); // Limpa o buffer após ler o número
            if (*totalTerritorios < 2) {
                printf(ANSI_COLOR_RED "\nPor favor, cadastre pelo menos 2 territórios para iniciar o jogo.\n" ANSI_COLOR_RESET);
                printf("\n");
            }
    } while (*totalTerritorios < 2);
}

// Função para cadastrar os territórios, suas cores e número de tropas.
void cadastrarTerritorios(territorios* lista_Territorios, int totalTerritorios){

    char* continentes[6] = {"América do Norte", "América do Sul", "Europa", "África", "Ásia", "Oceania"}; // Lista de continentes disponíveis para os territórios.

    char* paises[6][5] = {
        {"Estados Unidos", "Canadá", "México", "Cuba", "Panamá"},
        {"Brasil", "Argentina", "Colômbia", "Peru", "Chile"},
        {"França", "Alemanha", "Itália", "Espanha", "Reino Unido"},
        {"Nigéria", "Egito", "África do Sul", "Quênia", "Marrocos"},
        {"China", "Índia", "Japão", "Rússia", "Arábia Saudita"},
        {"Austrália", "Nova Zelândia", "Fiji", "Samoa", "Tonga"}
    };// Lista de países disponíveis para os territórios, organizada por continente.

    char* nomesCores[6] = {"vermelho", "verde", "azul", "amarelo", "magenta", "ciano"}; // Lista de cores disponíveis para os exércitos.
    char* codigoCores[6] = {ANSI_COLOR_RED, ANSI_COLOR_GREEN, ANSI_COLOR_BLUE, ANSI_COLOR_YELLOW, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN}; // Lista de códigos de formatação de cores correspondentes às cores disponíveis para os exércitos.


    int contador = 0;

    while (contador < totalTerritorios){
        territorios* t = &lista_Territorios[contador]; 

        // Exibe as opções de continentes, países e cores para o jogador escolher
        int opcoesContinente, opcoesPais, opcoesCor;
        
        do {
            printf("\nEscolha o continente:\n");
            for (int i = 0; i < 6; i++) {
                printf("%d - %s\n", i + 1, continentes[i]);
            }
            printf("Opção: ");
            scanf("%d", &opcoesContinente);
            limparBuffer(); // Limpa o buffer após ler a opção
            if (opcoesContinente < 1 || opcoesContinente > 6) {
                printf(ANSI_COLOR_RED"\nOpção inválida. Por favor, escolha um continente válido.\n" ANSI_COLOR_RESET);
            }
        }while (opcoesContinente < 1 || opcoesContinente > 6);

        do {
            printf("\nEscolha o país:\n");
            for (int i = 0; i < 5; i++){
                printf("%d - %s\n", i + 1, paises[opcoesContinente - 1][i]);
            }
            printf("Opção: ");
            scanf("%d", &opcoesPais);
            limparBuffer(); // Limpa o buffer após ler a opção
            if (opcoesPais < 1 || opcoesPais > 5) {
                printf(ANSI_COLOR_RED"\nOpção inválida. Por favor, escolha um país válido.\n" ANSI_COLOR_RESET);
            }
        }while (opcoesPais < 1 || opcoesPais > 5);

        strcpy (t->nome, paises[opcoesContinente - 1][opcoesPais - 1]); // Atribui o nome do país escolhido ao território

        do {
            printf("\nEscolha a cor do exército:\n");
            for (int i = 0; i < 6; i++) {
                printf("%d - %s%s%s\n", i + 1, codigoCores[i], nomesCores[i], ANSI_COLOR_RESET);
            }
            printf("Opção: ");
            scanf("%d", &opcoesCor);
            limparBuffer(); // Limpa o buffer após ler a opção
            if (opcoesCor < 1 || opcoesCor > 6) {
                printf(ANSI_COLOR_RED"\nOpção inválida. Por favor, escolha uma cor válida.\n" ANSI_COLOR_RESET);
            }
        }while (opcoesCor < 1 || opcoesCor > 6);
        strcpy(t->cor_Exercito, nomesCores[opcoesCor - 1]); // Atribui a cor do exército escolhida ao território
        t -> num_Tropas = 2 + (rand() % 2); // Atribui um número aleatório de tropas (entre 2 e 3) ao território


        t-> conquistas = 0;
        t->missao = (char*)malloc(100 * sizeof(char)); 
        atribuirMissao(t->missao, vetorMissoes, 2);
        printf("\n");
        printf(ANSI_COLOR_YELLOW"Aguarde um momento enquanto processamos o cadastro...\n" ANSI_COLOR_RESET);
        Sleep(700); // Pausa de 0.7 segundos para criar uma sensação de processamento
        printf(ANSI_COLOR_GREEN"\nTerritório cadastrado com sucesso!\n\n" ANSI_COLOR_RESET);
        contador++;
    }

}

// Função para exibir o mapa do mundo atual com os territórios cadastrados e suas informações após a batalha, usando formatação de cores para os exércitos.
void exibirMapa(territorios* lista_Territorios, int totalTerritorios){
    printf("\n");
        printf(ANSI_COLOR_YELLOW"Processando o mapa do mundo atual...\n" ANSI_COLOR_RESET);
            Sleep(1000); // Pausa de 1 segundo para criar uma sensação de processamento
        // Exibe o mapa do mundo atual
        printf(ANSI_COLOR_BLUE"========================================\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"          MAPA DO MUNDO ATUAL     \n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"========================================\n" ANSI_COLOR_RESET);

        // Exibe os territórios cadastrados
        for (int i = 0; i < totalTerritorios; i++) {
        territorios* t = &lista_Territorios[i];

        char cor_formatacao[100]; // Buffer para armazenar o código de formatação da cor
        obterCodigoCor(t->cor_Exercito, mapaCores, cor_formatacao); // Obtém o código de formatação para a cor do exército

            // Exibe o território com a formatação de cor e a MISSÃO
        printf("%d - %s (Exército %s%s%s, Tropas: %d) \n", i + 1,  
                t->nome, cor_formatacao, t->cor_Exercito, ANSI_COLOR_RESET, t->num_Tropas);
        }
}

// Função para exibir o menu de batalha e solicitar a escolha do jogador para iniciar um confronto entre dois territórios.
void menuBatalha (territorios* lista_Territorios, int totalTerritorios, int* opcao) {
    // Exibe as opções de batalha
        printf("\n");
        printf(ANSI_BG_BLUE"INICIAR CONFRONTO" ANSI_BG_RESET);
        printf("\n");
        printf(ANSI_COLOR_BLUE"\n1 - Realizar ataque\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"2 - Verificar missões\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"3 - AJUDA\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_RED"0 - Sair\n"ANSI_COLOR_RESET);
        printf("Escolha uma opção: ");
        *opcao = -1; // Inicializa a opção com um valor inválido para garantir que o loop de validação seja executado
        while (*opcao != 1 && *opcao != 2 &&  *opcao != 3 && *opcao != 0) {
            scanf("%d", opcao);
            limparBuffer(); // Limpa o buffer após ler a opção
            if (*opcao != 1 && *opcao != 2 && *opcao != 3 && *opcao != 0) {
                printf(ANSI_COLOR_RED"\nOpção inválida. Por favor, escolha uma opção válida.\n" ANSI_COLOR_RESET);
                printf("Escolha uma opção: ");
            }
        }
}

// Função para ler as escolhas dos jogadores para o confronto entre os territórios, com validação para garantir que as escolhas sejam válidas.
void lerEscolhas(int* escolha1, int* escolha2, int totalTerritorios) {
    printf("\nEscolha os territórios para o confronto:\n");
    do{
        printf("Escolha o território do ATACANTE (1 a %d): ", totalTerritorios);
        scanf("%d", escolha1);
        printf("Escolha o território do DEFENSOR (1 a %d): ", totalTerritorios);
        scanf("%d", escolha2);
        limparBuffer();

        // Validação das escolhas
        if (*escolha1 < 1 || *escolha1 > totalTerritorios || *escolha2 < 1 || *escolha2 > totalTerritorios) {
        printf(ANSI_COLOR_RED"Escolha inválida. Encerrando o jogo, por favor digite novamente.\n" ANSI_COLOR_RESET);
         }
    } while (*escolha1 < 1 || *escolha1 > totalTerritorios || *escolha2 < 1 || *escolha2 > totalTerritorios);
}

// Função para simular o ataque entre os territórios escolhidos.
void atacar(territorios* atacante, territorios* defensor){
    
    //simulação de ataque usando dados
    int dadosAtacante = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6
    int dadosDefensor = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6

    printf("\nRESULTADO DA BATALHA:\n");
// Loop para criar a animação rápida
for (int i = 0; i < 15; i++) {
    int simulaAtaque = (rand() % 6) + 1;
    int simulaDefesa = (rand() % 6) + 1;
    
    // O \r no começo faz o texto sobrescrever a si mesmo na mesma linha
    printf("\rGirando... Atacante: [%d] | Defensor: [%d]", simulaAtaque, simulaDefesa);
    
    // Força o terminal a mostrar o texto imediatamente
    fflush(stdout); 
    
    // Pausa bem rápida de 100 milissegundos (0.2 segundo)
    Sleep(200); 
}

// Quando o loop acaba, limpamos a linha da animação com espaços e imprimimos o resultado real
printf("\r                                                  \r");

    printf("Atacante (%s) rolou: %d\n", atacante->nome, dadosAtacante);
    printf("Defensor (%s) rolou: %d\n", defensor->nome, dadosDefensor);

    if(dadosAtacante > dadosDefensor){
        printf(ANSI_COLOR_GREEN"\nO atacante venceu a batalha!\n" ANSI_COLOR_RESET);
        defensor->num_Tropas -= 1; // O defensor perde uma tropa
        atacante->num_Tropas += 1; // O atacante ganha uma tropa
        if (defensor->num_Tropas <= 0) {
            printf(ANSI_COLOR_GREEN"\nO território %s (%s) foi conquistado por %s (%s)!\n" ANSI_COLOR_RESET, defensor->nome, defensor->cor_Exercito, atacante->cor_Exercito, atacante->nome);
            atacante->conquistas = 1; // Marca o território como conquistado
        }
    } else if(dadosAtacante < dadosDefensor){
        printf(ANSI_COLOR_GREEN"\nO defensor venceu a batalha!\n" ANSI_COLOR_RESET);
        atacante->num_Tropas -= 1; // O atacante perde uma tropa
        defensor->num_Tropas += 1; // O defensor ganha uma tropa

        // Verifica se o atacante perdeu todas as tropas
        if (atacante->num_Tropas <= 0) {
            printf(ANSI_COLOR_GREEN"\nO território %s (%s) foi conquistado pelo exército %s (%s)!\n" ANSI_COLOR_RESET, atacante->nome, atacante->cor_Exercito, defensor->cor_Exercito, defensor->nome);
            defensor->conquistas = 1; // Marca o território como conquistado
        }

    } else {
        printf(ANSI_COLOR_YELLOW"\nEmpate! Nenhuma tropa é perdida.\n" ANSI_COLOR_RESET);
    }
}

// Função para eliminar um território do jogo quando ele perde todas as tropas, e exibir uma mensagem informando a eliminação.
void eliminarTerritorio(territorios* lista_Territorios, int* totalTerritorios, int indice) {
    printf(ANSI_COLOR_RED"\nO território %s (%s) perdeu todas tropas e foi removido do jogo!\n" ANSI_COLOR_RESET, lista_Territorios[indice].nome, lista_Territorios[indice].cor_Exercito);
    // Remove o território do jogo
    for (int i = indice; i < (*totalTerritorios) - 1; i++) {
        lista_Territorios[i] = lista_Territorios[i + 1];
    }
    (*totalTerritorios)--; // Reduz o número total de territórios
}

// Função para verificar se há um vencedor após cada batalha, ou seja, quando resta apenas um território no jogo, e exibir uma mensagem declarando o vencedor.
int vencedor(territorios* lista_Territorios, int *totalTerritorios) {
    if (*totalTerritorios == 1) {
        printf(ANSI_COLOR_GREEN"O território %s (%s) é o único restante e foi declarado vencedor!\n" ANSI_COLOR_RESET, lista_Territorios[0].nome, lista_Territorios[0].cor_Exercito);
        printf("\n");
        printf("=======================================");
        printf("\n");
        printf(ANSI_BG_GREEN"     Exército %s (%s) WINNER!    " ANSI_BG_RESET, lista_Territorios[0].cor_Exercito, lista_Territorios[0].nome);
        printf("\n");
        printf("=======================================");
        liberarMemoria (lista_Territorios, *totalTerritorios, vetorMissoes, 2); // Libera a memória antes de encerrar o programa
        return 1; // Retorna 1 indicando que há um vencedor
    }
    return 0; // Retorna 0 indicando que ainda não há um vencedor
}

// Função para liberar a memória alocada para os territórios e exibir uma mensagem de despedida.
void liberarMemoria(territorios* mapa, int totalTerritorios, char* vetorMissoes[], int totalMissoes) {
    for (int i = 0; i < totalTerritorios; i++) {
        free(mapa[i].missao); // Libera a memória alocada para a missão de cada território
    }
    free(mapa);
    printf(ANSI_COLOR_GREEN"\nMemoria liberada. Ate a proxima!\n" ANSI_COLOR_RESET);
}

//Função para obter o código de formatação de cor correspondente ao nome da cor do exército, usando a tabela de referência mapaCores, e armazenar o código na variável de saída para ser usado na formatação do texto.
void obterCodigoCor(char* corExército, CorMapa* mapaCores, char* saida) {
    char corTemp[20];
    strcpy(corTemp, corExército);

    // Converte para minúsculo
    for (int j = 0; corTemp[j]; j++) {
        corTemp[j] = tolower(corTemp[j]);
    }

    // Busca o código na sua struct mapaCores
    for (int k = 0; k < 6; k++) {
        if (strcmp(corTemp, mapaCores[k].nome) == 0) {
            strcpy(saida, mapaCores[k].codigo); // Copia o código de formatação para a variável de saída
            return; // Sai da função após encontrar a cor
        }
    }

    strcpy(saida, ANSI_COLOR_RESET); // Cor padrão caso não encontre
}

// Função para atribuir uma missão aleatória a cada território, selecionando uma missão da lista de missões disponíveis e copiando-a para o campo de missão do território.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Função para verificar se um território cumpriu sua missão.
int verificarMissao(territorios* territorio, char* vetorMissoes[], int totalTerritorios) {
    // Missão 0: "Seja o último território restante no jogo."
    if (strcmp(territorio->missao, vetorMissoes[0]) == 0) {
        if (totalTerritorios == 1) { 
            printf(ANSI_COLOR_GREEN"\n=======================================\n" ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"Parabéns! O território %s cumpriu a missão:\n" ANSI_COLOR_RESET, territorio->nome);
            printf("%s\n", territorio->missao);
            printf(ANSI_COLOR_GREEN"=======================================\n" ANSI_COLOR_RESET);
            return 1; // Retorna 1 indicando vitória
        }
    }
    
    // Missão 1: "Conquiste um território específico." (Simplificado para conquistar 1 qualquer)
    else if (strcmp(territorio->missao, vetorMissoes[1]) == 0) {
        if (territorio->conquistas >= 1) {
            printf(ANSI_COLOR_GREEN"\n=======================================\n" ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"Parabéns! O exército %s cumpriu a missão:\n" ANSI_COLOR_RESET, territorio->nome);
            printf("%s\n", territorio->missao);
            printf(ANSI_COLOR_GREEN"=======================================\n" ANSI_COLOR_RESET);
            return 1; // Retorna 1 indicando vitória                           
        }
    }

    return 0; // Retorna 0 indicando que a missão ainda não foi cumprida
}
