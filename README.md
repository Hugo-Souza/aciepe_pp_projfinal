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
Para verificar a escalabilidade foi criado um script shell para realizar a execução das aplicações 5 vezes para obter a média da execução.

A máquina onde foi executado as aplicações tanto a sequencial quanto a paralela com openMP possui as seguintes configurações de CPU:

```
Arquitetura:                x86_64
Modo(s) operacional da CPU: 32-bit, 64-bit
Ordem dos bytes:            Little Endian
CPU(s):                     8
Lista de CPU(s) on-line:    0-7
Thread(s) per núcleo:       2
Núcleo(s) por soquete:      4
Soquete(s):                 1
Nó(s) de NUMA:              1
ID de fornecedor:           GenuineIntel
Família da CPU:             6
Modelo:                     142
Nome do modelo:             Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz
```

Rodando o script shell, foi obtido os seguintes resultados de tempo de execução de cada aplicação, sendo ela serial ou paralela com variação de quantidade de threads. Segue o resultado da execução do script:
```
Execucao das aplicacoes para analise de desempenho
Sera executado 5 vezes cada aplicacao para ser analizado a media
  
  
----------------------------------------------------------
*** SEQUENCIAL ***
Ex1: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 27.776314 segundos com 3001 iteracoes 
  
Ex2: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 28.923202 segundos com 3001 iteracoes 
  
Ex3: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 30.230641 segundos com 3001 iteracoes 
  
Ex4: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 29.176592 segundos com 3001 iteracoes 
  
Ex5: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 31.026530 segundos com 3001 iteracoes 
  
----------------------------------------------------------
*** PARALELO OPENMP***
*** 2 THREADS ***
Ex1: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 17.238071 segundos com 3001 iteracoes 
  
Ex2: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 17.238856 segundos com 3001 iteracoes 
  
Ex3: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 17.330511 segundos com 3001 iteracoes 
  
Ex4: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 18.996199 segundos com 3001 iteracoes 
  
Ex5: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 18.602503 segundos com 3001 iteracoes 
  
----------------------------------------------------------
*** PARALELO OPENMP***
*** 4 THREADS ***
Ex1: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 10.490534 segundos com 3001 iteracoes 
  
Ex2: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 10.735021 segundos com 3001 iteracoes 
  
Ex3: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 10.556781 segundos com 3001 iteracoes 
  
Ex4: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 10.402260 segundos com 3001 iteracoes 
  
Ex5: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 10.329706 segundos com 3001 iteracoes 
  
----------------------------------------------------------
*** PARALELO OPENMP***
*** 8 THREADS ***
Ex1: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 11.265941 segundos com 3001 iteracoes 
  
Ex2: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 12.927978 segundos com 3001 iteracoes 
  
Ex3: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 13.098629 segundos com 3001 iteracoes 
  
Ex4: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 13.707154 segundos com 3001 iteracoes 
  
Ex5: 
Calculo do metodo de Jacobi - grade: 1024 x 1024 
Kernel executado em 12.881422 segundos com 3001 iteracoes 
```

Com os tempos de execução obtidos, foi possível calcular a média do tempo de execução para cada situação da aplicação:
![Média do Tempo de Execução](https://github.com/Hugo-Souza/aciepe_pp_projfinal/assets/54536160/2b6a8daa-7242-4a6b-ae28-ac2e54e4a7e3)


## Discussão sobre a eficiência da solução
Ao analisar a média do tempo de execução, constata-se que a utilização do OpenMP para paralelização resulta em um tempo de execução menor em comparação com a execução sequencial. No entanto, é relevante notar que, em média, o tempo de execução de 12,93 segundos com 8 threads foi superior aos 10,49 segundos obtidos com 4 threads. Isso se deve ao fato de que determinadas partes do código ainda são executadas de forma sequencial, limitando assim o ganho máximo alcançado com a paralelização. Esse fenômeno é conhecido como a Lei de Amdahl. Neste contexto, o desempenho máximo é atingido quando se utilizam 4 threads.

Após toda a análise da eficiência das aplicações, foi relembrado de uma diretica do OpenMP no qual poderia ser utilizado para melhorar a paralelização, pois a mesma é mais utilizada com problemas que apresentam uma quantidade maior de dados a serem computados, no qual é a omp unroll.

## Conclusões
Através da análise do código do solucionador de Laplace, foi possível identificar oportunidades de otimização com o uso do OpenMP. Inicialmente, utilizou-se a versão sequencial e, em seguida, foram implementada as diretivas do OpenMP, como "omp parallel", que cria uma região paralela, "omp for", que divide um loop entre as threads, "omp shared", que especifica quais variáveis serão compartilhadas entre as threads na região, e "omp schedule", que controla a distribuição das iterações do loop entre as threads.

Com a paralelização, notou-se uma redução no tempo de execução da aplicação à medida que aumentaram o número de threads. No entanto, também constatou-se que nem sempre a paralelização é vantajosa. Por exemplo, com 8 threads, observou-se que o tempo de execução médio foi maior do que com 4 threads, o que reforça o princípio da Lei de Amdahl, demonstrando que há limitações na melhoria de desempenho com a paralelização em certos cenários.


## Discentes do Projeto
Segue os alunos que participaram do desenvolvimento do projeto

<table>
    <tr>
        <td align="center">
            <a href="https://github.com/CarolElisa">
                <img src="https://avatars.githubusercontent.com/CarolElisa" width="100px;" alt="Foto do Carol no GitHub"/><br>
                <sub>
                    <b>Caroline Elisa Duarte de Souza - 795565</b>
            </a>
        </td>
        <td align="center">
            <a href="https://github.com/Hugo-Souza">
                <img src="https://avatars.githubusercontent.com/Hugo-Souza" width="100px;" alt="Foto do Hugo no GitHub"/><br>
                <sub>
                    <b>Hugo da Silva e Souza - 761211</b>
            </a>
        </td>
    </tr>
</table>


