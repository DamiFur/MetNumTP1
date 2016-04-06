#!/usr/bin/python
# Script hecho para correr tests y generarte graficos lindos
import os
import time
import math
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

ARCHIVOS_TEST = ['saraza.txt']
EQUIPOS_TEST = [10]
archivo_cholesky = 'chol.txt'
archivo_wp = 'wp.txt'

iteraciones = 80

# Compilo por las dudas
os.system('g++ ../TP1.cpp -O2 -std=c++11 -march=native ../sqrt.o -o ../TP1')
os.system('g++ ../benchgen.cpp -O2 -std=c++11 -march=native -o ../benchgen')
os.system('rm *.txt')

numero_test = 0
for archivo_test in ARCHIVOS_TEST:
    os.system('../TP1 {0} {1} 1 >> '.format(archivo_test, archivo_cholesky))
    os.system('../TP1 {0} {1} 2 >> '.format(archivo_test, archivo_wp))

    f_chol = open(archivo_cholesky, 'r')
    f_wp = open(archivo_wp, 'r')
    x = []
    r_chol = []
    r_wp = []

    for i in range(EQUIPOS_TEST[numero_test]):
        x.append(i)
        r_chol.append(int(f_chol.readline()))
        r_wp.append(int(f_wp.readline()))

    numero_test += 1

    # Genero el grafico
    # TODO: Utilizar plt.subplot y .bar
    # Ver: https://plot.ly/matplotlib/bar-charts/#plot-multiple-bars
    plt.plot(x, r_chol, 'bo',  x, r_wp, 'go')
    blue_patch = mpatches.Patch(color='blue', label='Cholesky')
    green_patch = mpatches.Patch(color='green', label='Winning Percentage')
    plt.legend(handles=[blue_patch, green_patch])
    plt.xlabel('Numero de Equipo')
    plt.ylabel('Ranking')
    plt.savefig('test_ranking_{}.png'.format(archivo_test))
    plt.close()
