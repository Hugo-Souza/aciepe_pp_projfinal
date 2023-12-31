#!/bin/bash

echo "Teste"
echo "Execucao das aplicacoes para analise de desempenho"
echo "Sera executado 5 vezes cada aplicacao para ser analizado a media"
echo "  "
echo "  "
echo "----------------------------------------------------------"
echo "*** SEQUENCIAL ***"
echo "Ex1: "
./laplace_seq 1024
echo "  "
echo "Ex2: "
./laplace_seq 1024
echo "  "
echo "Ex3: "
./laplace_seq 1024
echo "  "
echo "Ex4: "
./laplace_seq 1024
echo "  "
echo "Ex5: "
./laplace_seq 1024
echo "  "
echo "----------------------------------------------------------"
echo "*** PARALELO OPENMP***"
echo "*** 2 THREADS ***"
export OMP_NUM_THREADS=2
echo "Ex1: "
./laplace_omp 1024
echo "  "
echo "Ex2: "
./laplace_omp 1024
echo "  "
echo "Ex3: "
./laplace_omp 1024
echo "  "
echo "Ex4: "
./laplace_omp 1024
echo "  "
echo "Ex5: "
./laplace_omp 1024
echo "  "
echo "----------------------------------------------------------"
echo "*** PARALELO OPENMP***"
echo "*** 4 THREADS ***"
export OMP_NUM_THREADS=4
echo "Ex1: "
./laplace_omp 1024
echo "  "
echo "Ex2: "
./laplace_omp 1024
echo "  "
echo "Ex3: "
./laplace_omp 1024
echo "  "
echo "Ex4: "
./laplace_omp 1024
echo "  "
echo "Ex5: "
./laplace_omp 1024
echo "  "
echo "----------------------------------------------------------"
echo "*** PARALELO OPENMP***"
echo "*** 8 THREADS ***"
export OMP_NUM_THREADS=8
echo "Ex1: "
./laplace_omp 1024
echo "  "
echo "Ex2: "
./laplace_omp 1024
echo "  "
echo "Ex3: "
./laplace_omp 1024
echo "  "
echo "Ex4: "
./laplace_omp 1024
echo "  "
echo "Ex5: "
./laplace_omp 1024
echo "  "