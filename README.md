# ACIEPE - PROGRAMAÇÃO PARALELA: Projeto Final

## Descrição do problema
Solucionador de Laplace usando iteração simples de Jacobi. Este programa resolve a equação de Laplace em uma grade regular usando iteração simples de Jacobi, sendo correlacionado a função de transferência de calor em uma placa de metal. A equação de Laplace em duas dimensões é:

![laplace-fig-1](https://github.com/Hugo-Souza/aciepe_pp_projfinal/assets/54536160/39653ad6-72f8-43c6-af85-b5a1a9b66977)

Vamos supor a grade A sendo a placa de metal. Podemos discretizar a equação de Laplace conforme mostrado na figura abaixo.
![laplace-fig-2](https://github.com/Hugo-Souza/aciepe_pp_projfinal/assets/54536160/c2d3cb22-b89c-4fef-87b6-095fe8765893)

Assim o ponto A(i,j) é o ponto central da placa onde será aplicado o calor, e com o decorrer do tempo(iterações), essa temperatura se propaga para o restante da superfície seguindo a equação de laplace.
## Estratégia de paralelização

### Versão sequencial de referência (apenas trecho de interesse para paralelização)


### Versão paralela comentada (apenas trecho de interesse de paralelização)

## Análise da escalabilidade: esperada e obtida

## Discussão sobre a eficiência da solução

## Conclusões




