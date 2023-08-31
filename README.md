# ACIEPE - PROGRAMAÇÃO PARALELA: Projeto Final

## Descrição do problema
Solucionador de Laplace usando iteração simples de Jacobi. Este programa resolve a equação de Laplace em uma grade regular usando iteração simples de Jacobi, sendo correlacionado a função de transferência de calor em uma placa de metal. A equação de Laplace em duas dimensões é:

![laplace-fig-1](https://github.com/Hugo-Souza/aciepe_pp_projfinal/assets/54536160/39653ad6-72f8-43c6-af85-b5a1a9b66977)

Vamos supor a grade A sendo a placa de metal. Podemos discretizar a equação de Laplace conforme mostrado na figura abaixo.
![laplace-fig-2](https://github.com/Hugo-Souza/aciepe_pp_projfinal/assets/54536160/c2d3cb22-b89c-4fef-87b6-095fe8765893)

Assim o ponto A(i,j) é o ponto central da placa onde será aplicado o calor, e com o decorrer do tempo(iterações), essa temperatura se propaga para o restante da superfície seguindo a equação de laplace.
## Estratégia de paralelização
![OpenMP_logo](https://github.com/Hugo-Souza/aciepe_pp_projfinal/assets/54536160/7db7d67d-0252-40b3-9a55-ad97494af435)

Para realizar a paralelização da aplicação proposta, foi utilizado a interface de programação OpenMP, no qual suporta programação de multiprocessamento em memória compartilhada em plataformas múltiplas, usando C.
O OpenMP funciona permitindo que o desenvolvedor especifíque o paralelismo em seu código por meio de combinações de diretivas do compilador, rotinas da biblioteca em tempo real e variáveis de ambiente. Essas diretivas e funções fornecem sugestões ao compilador sobre onde e como paralelizar porções do seu código.

### Versão sequencial de referência (apenas trecho de interesse para paralelização)
Na versão sequencial foi identificado os kernels onde temos o interesse de paralelizar, para aumentar a eficiência utilizando os artifícios que o OpenMP nos fornece. 
```c
//  Metodo iterativo de Jacobi
//  Esse loop terminara se numero fixo de iteracoes maximas concluidas
while(iter <= ITER_MAX){
    //  Kernel 1
    //  Calcula a equacao de Laplace para determinar o proximo valor de cada celula da grade
    
    for(int i = 1; i < size-1; i++){
        for(int j = 1; j < size-1; j++){
            new_grid[i][j] = 0.25 * (grid[i][j+1] + grid[i][j-1] +
                                     grid[i-1][j] + grid[i+1][j]);
        }
    }

    
    //  Kernel 2
    //  Copia os proximos valores na matriz de trabalho para a proxima iteracao
    for(int i = 1; i < size-1; i++){
        for(int j = 1; j < size-1; j++){
            grid[i][j] = new_grid[i][j];
        }
    }
        
    iter++;
}
```

### Versão paralela comentada (apenas trecho de interesse de paralelização)
Com os kernels identificados foi realizado o estudo de quais diretivas poderiam ser utilizadas, para paralelizar de uma maneira que podesse ter maior efeciência.Então foi escolhido as seguintes diretivas 
Diretiva|Descrição
--------|---------
omp parallel | sta diretiva cria uma região paralela, onde várias threads serão executadas em paralelo. Cada thread dentro dessa região executará o bloco de código contido entre as diretivas omp parallel e omp for. Cada thread é uma "instância" da execução do bloco de código.
omp for | A diretiva omp for é usada dentro da região paralela e indica que um loop subsequente será dividido entre as threads disponíveis para execução paralela. Geralmente, um loop for é escolhido para ser executado em paralelo porque suas iterações são independentes umas das outras.
omp shared | A cláusula shared é usada para especificar quais variáveis devem ser compartilhadas entre as threads dentro da região paralela. Variáveis compartilhadas são aquelas que todas as threads podem acessar e modificar.
omp schedule | A cláusula schedule é usada para controlar como as iterações do loop são distribuídas entre as threads. Ela determina a maneira como as iterações são atribuídas às threads para equilibrar a carga de trabalho.

Segue o código da aplicação modificado com as diretivas utilizadas para a realização da paralelização. 
```c
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
```


## Análise da escalabilidade: esperada e obtida

## Discussão sobre a eficiência da solução

## Conclusões




