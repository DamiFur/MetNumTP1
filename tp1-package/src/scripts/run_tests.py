#!/usr/bin/python
# Script hecho para correr tests y generarte graficos lindos
import os
import time
import matplotlib.pyplot as plt

ARCHIVO_TEST = {'SARAZA1', 'SARAZA2'}
ARCHIVO_CICLOS = 'tiempo.txt'

def leer_ciclos():
    f = open(ARCHIVO_TIEMPO, 'r')
    linea = f.readline()
    return int(linea)


# Compilo por las dudas
os.system('g++ ../TP1.cpp -O2 -std=c++11 -o ../TP1')

# Corro el test con los 3 algoritmos
nro_test = 1
for test in ARCHIVO_TEST:
    x.append(nro_test)
    nro_test += 1

    os.system('../TP1 ' + ARCHIVO_TEST + ' salida.out 0 > ' + ARCHIVO_CICLOS)
    ciclos_gaussian.append(leer_ciclos())

    os.system('../TP1 ' + ARCHIVO_TEST + ' salida.out 1 > ' + ARCHIVO_CICLOS)
    ciclos_cholesky.append(leer_ciclos())

    os.system('../TP1 ' + ARCHIVO_TEST + ' salida.out 2 > ' + ARCHIVO_CICLOS)
    ciclos_wp.append(leer_ciclos())

# Genero el grafico
plt.xlabel