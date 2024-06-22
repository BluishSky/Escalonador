/*
Simulador de Algoritmos de Escalonamento
Trabalho Prático
Sistemas Operacionais — 2024.1
Professor: Renato Porfirio Ishii
UFMS (Universidade Federal de Mato Grosso do Sul)
FACOM (Faculdade de Computação)

Aluno: Vinícius Santos Silva
RGA: 2022.1904.0140
*/


#include <stdio.h>
#include <stdlib.h>

// Estrutura para armazenar as informações dos processos
typedef struct {
    int id;                  // ID do processo
    int tempo_chegada;       // Tempo de chegada do processo
    int tempo_execucao;      // Tempo de execução do processo
    int tempo_restante;      // Tempo de execução restante (para SJF preemptivo e Round Robin)
    int tempo_comeco;        // Tempo de início da execução
    int tempo_termino;       // Tempo de término da execução
    int tempo_espera;        // Tempo de espera na fila
    int tempo_retorno;       // Tempo de retorno (tempo de término - tempo de chegada)
    int prioridade;          // Prioridade do processo (para escalonamento por prioridade)
} Processo;

// Função para o escalonamento FCFS (First-Come, First-Served)
void fcfs(Processo *processos, int n) {
    int tempo_atual = 0;  // Variável para rastrear o tempo atual
    for (int i = 0; i < n; i++) {
        Processo *proc = &processos[i];
        if (tempo_atual < proc->tempo_chegada) {
            tempo_atual = proc->tempo_chegada;  // Atualiza o tempo atual para o tempo de chegada do processo se necessário
        }
        proc->tempo_comeco = tempo_atual;  // Define o tempo de início
        proc->tempo_termino = tempo_atual + proc->tempo_execucao;  // Calcula o tempo de término
        proc->tempo_espera = proc->tempo_comeco - proc->tempo_chegada;  // Calcula o tempo de espera
        proc->tempo_retorno = proc->tempo_termino - proc->tempo_chegada;  // Calcula o tempo de retorno
        tempo_atual += proc->tempo_execucao;  // Atualiza o tempo atual

        printf("Processo %d | Inicio: %d | Fim: %d | Espera: %d | Retorno: %d\n", 
                proc->id, proc->tempo_comeco, proc->tempo_termino, 
                proc->tempo_espera, proc->tempo_retorno);
    }
}

// Função para o escalonamento SJF não preemptivo
void sjf_nao_preemptivo(Processo *processos, int n) {
    int tempo_atual = 0;  // Variável para rastrear o tempo atual
    int processos_restantes = n;  // Contador de processos restantes
    int min_index;  // Índice do próximo processo a ser executado
    int completado[n];  // Array para marcar processos completados
    
    for (int i = 0; i < n; i++) {
        completado[i] = 0;  // Inicializa todos os processos como não completados
    }

    while (processos_restantes > 0) {
        int min_tempo_exec = 99999;  // Define um valor alto para encontrar o tempo de execução mínimo
        min_index = -1;

        for (int i = 0; i < n; i++) {
            if (processos[i].tempo_chegada <= tempo_atual && !completado[i] && processos[i].tempo_execucao < min_tempo_exec) {
                min_tempo_exec = processos[i].tempo_execucao;
                min_index = i;  // Encontra o índice do próximo processo a ser executado
            }
        }

        if (min_index == -1) {
            tempo_atual++;  // Incrementa o tempo atual se nenhum processo estiver pronto
            continue;
        }

        Processo *proc = &processos[min_index];
        proc->tempo_comeco = tempo_atual;  // Define o tempo de início
        proc->tempo_termino = tempo_atual + proc->tempo_execucao;  // Calcula o tempo de término
        proc->tempo_espera = proc->tempo_comeco - proc->tempo_chegada;  // Calcula o tempo de espera
        proc->tempo_retorno = proc->tempo_termino - proc->tempo_chegada;  // Calcula o tempo de retorno

        tempo_atual += proc->tempo_execucao;  // Atualiza o tempo atual
        completado[min_index] = 1;  // Marca o processo como completado
        processos_restantes--;  // Decrementa o contador de processos restantes

        printf("Processo %d | Inicio: %d | Fim: %d | Espera: %d | Retorno: %d\n", 
                proc->id, proc->tempo_comeco, proc->tempo_termino, 
                proc->tempo_espera, proc->tempo_retorno);
    }
}

// Função para o escalonamento SJF preemptivo
void sjf_preemptivo(Processo *processos, int n) {
    int tempo_atual = 0, processos_completos = 0;  // Variáveis para rastrear o tempo atual e processos completos
    int min_tempo_restante, min_index;  // Variáveis para encontrar o próximo processo a ser executado
    int completado[n];  // Array para marcar processos completados
    
    for (int i = 0; i < n; i++) {
        completado[i] = 0;  // Inicializa todos os processos como não completados
        processos[i].tempo_restante = processos[i].tempo_execucao;  // Inicializa o tempo restante com o tempo de execução
    }

    while (processos_completos < n) {
        min_tempo_restante = 99999;  // Define um valor alto para encontrar o tempo de execução restante mínimo
        min_index = -1;

        for (int i = 0; i < n; i++) {
            if (processos[i].tempo_chegada <= tempo_atual && !completado[i] && processos[i].tempo_restante < min_tempo_restante) {
                min_tempo_restante = processos[i].tempo_restante;
                min_index = i;  // Encontra o índice do próximo processo a ser executado
            }
        }

        if (min_index == -1) {
            tempo_atual++;  // Incrementa o tempo atual se nenhum processo estiver pronto
            continue;
        }

        Processo *proc = &processos[min_index];

        if (proc->tempo_comeco == -1) {
            proc->tempo_comeco = tempo_atual;  // Define o tempo de início se ainda não foi definido
        }

        proc->tempo_restante--;  // Decrementa o tempo restante
        tempo_atual++;  // Incrementa o tempo atual

        if (proc->tempo_restante == 0) {
            proc->tempo_termino = tempo_atual;  // Define o tempo de término
            proc->tempo_espera = proc->tempo_termino - proc->tempo_chegada - proc->tempo_execucao;  // Calcula o tempo de espera
            proc->tempo_retorno = proc->tempo_termino - proc->tempo_chegada;  // Calcula o tempo de retorno
            completado[min_index] = 1;  // Marca o processo como completado
            processos_completos++;  // Incrementa o contador de processos completos
            printf("Processo %d | Inicio: %d | Fim: %d | Espera: %d | Retorno: %d\n", 
                    proc->id, proc->tempo_comeco, proc->tempo_termino, 
                    proc->tempo_espera, proc->tempo_retorno);
        }
    }
}

// Função para o escalonamento por prioridade
void prioridade(Processo *processos, int n) {
    int tempo_atual = 0;  // Variável para rastrear o tempo atual
    int processos_restantes = n;  // Contador de processos restantes
    int min_index;  // Índice do próximo processo a ser executado
    int completado[n];  // Array para marcar processos completados

    for (int i = 0; i < n; i++) {
        completado[i] = 0;  // Inicializa todos os processos como não completados
    }

    while (processos_restantes > 0) {
        int min_prioridade = 99999;  // Define um valor alto para encontrar a prioridade mínima
        min_index = -1;

        for (int i = 0; i < n; i++) {
            if (processos[i].tempo_chegada <= tempo_atual && !completado[i] && processos[i].prioridade < min_prioridade) {
                min_prioridade = processos[i].prioridade;
                min_index = i;  // Encontra o índice do próximo processo a ser executado
            }
        }

        if (min_index == -1) {
            tempo_atual++;  // Incrementa o tempo atual se nenhum processo estiver pronto
            continue;
        }

        Processo *proc = &processos[min_index];
        proc->tempo_comeco = tempo_atual;  // Define o tempo de início
        proc->tempo_termino = tempo_atual + proc->tempo_execucao;  // Calcula o tempo de término
        proc->tempo_espera = proc->tempo_comeco - proc->tempo_chegada;  // Calcula o tempo de espera
        proc->tempo_retorno = proc->tempo_termino - proc->tempo_chegada;  // Calcula o tempo de retorno

        tempo_atual += proc->tempo_execucao;  // Atualiza o tempo atual
        completado[min_index] = 1;  // Marca o processo como completado
        processos_restantes--;  // Decrementa o contador de processos restantes

        printf("Processo %d | Inicio: %d | Fim: %d | Espera: %d | Retorno: %d\n", 
                proc->id, proc->tempo_comeco, proc->tempo_termino, 
                proc->tempo_espera, proc->tempo_retorno);
    }
}

// Função para o escalonamento Round Robin
void round_robin(Processo *processos, int n, int quantum) {
    int tempo_atual = 0;  // Variável para rastrear o tempo atual
    int processos_completos = 0;  // Contador de processos completos
    int i = 0;  // Índice do processo atual
    while (processos_completos < n) {
        if (processos[i].tempo_chegada <= tempo_atual && processos[i].tempo_restante > 0) {
            if (processos[i].tempo_restante == processos[i].tempo_execucao) {
                processos[i].tempo_comeco = tempo_atual;  // Define o tempo de início se ainda não foi definido
            }

            if (processos[i].tempo_restante > quantum) {
                tempo_atual += quantum;  // Incrementa o tempo atual pelo valor do quantum
                processos[i].tempo_restante -= quantum;  // Decrementa o tempo restante pelo valor do quantum
            } else {
                tempo_atual += processos[i].tempo_restante;  // Incrementa o tempo atual pelo tempo restante do processo
                processos[i].tempo_restante = 0;  // Define o tempo restante como 0
                processos[i].tempo_termino = tempo_atual;  // Define o tempo de término
                processos[i].tempo_espera = processos[i].tempo_termino - processos[i].tempo_chegada - processos[i].tempo_execucao;  // Calcula o tempo de espera
                processos[i].tempo_retorno = processos[i].tempo_termino - processos[i].tempo_chegada;  // Calcula o tempo de retorno
                processos_completos++;  // Incrementa o contador de processos completos
                printf("Processo %d | Inicio: %d | Fim: %d | Espera: %d | Retorno: %d\n", 
                        processos[i].id, processos[i].tempo_comeco, processos[i].tempo_termino, 
                        processos[i].tempo_espera, processos[i].tempo_retorno);
            }
        } else if (processos[i].tempo_chegada > tempo_atual) {
            tempo_atual++;  // Incrementa o tempo atual se nenhum processo estiver pronto
        }
        i = (i + 1) % n;  // Passa para o próximo processo (circular)
    }
}

// Função para exibir o menu de seleção de algoritmo de escalonamento
void menu() {
    printf("Escolha o algoritmo de escalonamento:\n");
    printf("1. FCFS (First-Come, First-Served)\n");
    printf("2. SJF (Shortest Job First) - Nao Preemptivo\n");
    printf("3. SJF (Shortest Job First) - Preemptivo\n");
    printf("4. Prioridade\n");
    printf("5. Round Robin\n");
    printf("6. Sair\n");
}

// Função principal
int main() {
    int escolha;  // Variável para armazenar a escolha do usuário
    
    while (1) {
        menu();  // Exibe o menu de seleção
        scanf("%d", &escolha);  // Lê a escolha do usuário

        if (escolha == 6) {
            printf("Saindo...\n");  // Sai do programa se a escolha for 6
            break;
        }

        int n, quantum;  // Variáveis para o número de processos e o quantum do Round Robin
        printf("Digite o numero de processos: ");
        scanf("%d", &n);

        Processo processos[n];  // Array para armazenar os processos
        for (int i = 0; i < n; i++) {
            printf("Digite o tempo de chegada do processo ( %d ): ", i + 1);
            scanf("%d", &processos[i].tempo_chegada);
            printf("Digite o tempo de execucao do processo ( %d ): ", i + 1);
            scanf("%d", &processos[i].tempo_execucao);
            if (escolha == 4) {
                printf("Digite a prioridade do processo ( %d ): ", i + 1);
                scanf("%d", &processos[i].prioridade);
            }
            processos[i].id = i + 1;  // Define o ID do processo
            processos[i].tempo_restante = processos[i].tempo_execucao;  // Inicializa o tempo restante
            processos[i].tempo_comeco = -1;  // Inicializa o tempo de início
            processos[i].tempo_termino = -1;  // Inicializa o tempo de término
            processos[i].tempo_espera = 0;  // Inicializa o tempo de espera
            processos[i].tempo_retorno = 0;  // Inicializa o tempo de retorno
        }

        switch (escolha) {
            case 1:
                fcfs(processos, n);  // Chama a função FCFS
                break;
            case 2:
                sjf_nao_preemptivo(processos, n);  // Chama a função SJF não preemptivo
                break;
            case 3:
                sjf_preemptivo(processos, n);  // Chama a função SJF preemptivo
                break;
            case 4:
                prioridade(processos, n);  // Chama a função de prioridade
                break;
            case 5:
                printf("Digite o tempo do quantum (em milissegundos): ");
                scanf("%d", &quantum);
                round_robin(processos, n, quantum);  // Chama a função Round Robin
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");  // Mensagem de erro para escolha inválida
        }
    }
    return 0;
}
