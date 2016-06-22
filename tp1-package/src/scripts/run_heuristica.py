#!/usr/bin/python
# Script hecho para correr tests y generarte graficos lindos
import os
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

ITERACIONES = 10
ARCHIVO = 'test_heuristica.in'
OUT = 'heuristica-temp.out'

def leerRanking():
    f_rankings = open('./{}'.format(OUT), 'r')
    ranking = []
    equipo = 0
    while True:
        rank = f_rankings.readline()
        if rank == '':
            break
        ranking.append([float(rank), equipo])
        equipo += 1

    ranking.sort(reverse=True)
    pos = 1
    for r in ranking:
        if r[1] == equipo - 1:
            return pos

        pos += 1

    return -1

x = []
mi_ranking = []

for i in range(1, ITERACIONES):
    os.system('./heuristica {} {}'.format(ARCHIVO, 1))
    x.append(i)
    mi_ranking.append(leerRanking())

# Genero el grafico
plt.plot(x, mi_ranking, 'bo')
blue_patch = mpatches.Patch(color='blue', label='Ranking')
plt.legend(handles=[blue_patch])
plt.xlabel('Cantidad de partidos')
plt.ylabel('Ranking')
plt.savefig('test_heuristica.png')

plt.close()
