/*
    Projeto Final da ACIEPE - Programacao Paralela: Arquiteturas e Estrategias de Acaleracao
    Caroline Elisa Duarte de Souza - 795565
    Hugo da Silva e Souza - 761211
    
    Este programa resolve a equacao de Laplace em uma grade 2D regular utilizando o metodo
        iterativo de algebra linear de Jacobi, para solucionar a equacao de transferencia 
        de calor de Laplace.  
    
    O calculo do stencil eh finalizado quando iter > ITER_MAX 

    Este programa eh a versao paralela
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define ITER_MAX 3000   //  Numero maximo de iteracoes
#define CONV_THRESHOLD 1.0e-5f  //  Limiar de convergencia

//  Matriz a ser resolvida
double **grid;

//  Matriz auxiliar
double **new_grid;

//  Tamanho de cada lado da grade
int size;

//  Retorna o valor maximo
double max(double a, double b){
    if(a > b){
        return a;
    }
    return b;
}

//  Retorna o valor absoluto de um numero
double absolute(double num){
    if(num < 0){
        return -1.0 * num;
    }
    return num;
}

//  Aloca memoria para a grade
void allocate_memory(){
    grid = (double **) malloc(size * sizeof(double *));
    new_grid = (double **) malloc(size * sizeof(double *));

    for(int i = 0; i < size; i++){
        grid[i] = (double *) malloc(size * sizeof(double));
        new_grid[i] = (double *) malloc(size * sizeof(double));
    }
}

//  Inicializa a grade
void initialize_grid(){
    //  Semente para gerador aleatorio
    srand(10);

    int linf = size / 2;
    int lsup = linf + size / 10;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            //  inicializa regiao de calor no centro do grid
            if(i>=linf && i < lsup && j>=linf && j<lsup){
                grid[i][j] = 100;
            }
            else{
                grid[i][j] = 0;
            }
            new_grid[i][j] = 0.0;
        }
    }
}

//  Salva a grade em um arquivo txt
void save_grid(){

    char file_name[30];
    sprintf(file_name, "grid_omp.txt");

    //  Salva o resultado
    FILE *file;
    file = fopen(file_name, "w");

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            fprintf(file, "%lf ", grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Use: ./laplace_omp N\n");
        printf("N: O tamanho de cada lado do dominio (grid)\n");
        exit(-1);
    }

    //  Variaveis para medir o tempo de execucao
    struct timeval time_start;
    struct timeval time_end;

    size = atoi(argv[1]);

    //  Alocar memoria para a grade (matriz)
    allocate_memory();

    //  Definir condicoes iniciais da grade
    initialize_grid();

    double err = 1.0;
    int iter = 0;

    printf("Calculo do metodo de Jacobi - grade: %d x %d \n", size, size);

    //  Obter a hora de inicio
    gettimeofday(&time_start, NULL);

    //  Metodo iterativo de Jacobi
    //  Esse loop terminara se numero fixo de iteracoes maximas concluidas
    while(iter <= ITER_MAX){
        //  Kernel 1
        //  Calcula a equacao de Laplace para determinar o proximo valor de cada celula da grade
        #pragma omp parallel for shared(grid, new_grid) schedule(dynamic)
        for(int i = 1; i < size-1; i++){
            for(int j = 1; j < size-1; j++){
                new_grid[i][j] = 0.25 * (grid[i][j+1] + grid[i][j-1] +
                                         grid[i-1][j] + grid[i+1][j]);
            }
        }

        
        //  Kernel 2
        //  Copia os proximos valores na matriz de trabalho para a proxima iteracao
        #pragma omp parallel for shared(grid, new_grid) schedule(dynamic)
        for(int i = 1; i < size-1; i++){
            for(int j = 1; j < size-1; j++){
                grid[i][j] = new_grid[i][j];
            }
        }
            
        iter++;
    }

    //  Obter a hora final
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    //  Salva a grade final em arquivo
    save_grid();

    printf("Kernel executado em %lf segundos com %d iteracoes \n", exec_time, iter);

    return 0;
}
