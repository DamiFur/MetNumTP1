#!/usr/bin/python
# Script hecho para correr tests y generarte graficos lindos
import os
import time
import math
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

def divl(lista, divs, n):
    sol = []
    for i in range(0, len(lista)):
        d = 1
        for j in range(0,n):
            d *= divs[i]
        sol.append(lista[i]/d)
    return sol

def divllog(lista, divs):
    sol = []
    for i in range(0,len(lista)):
       sol.append(lista[i]/(divs[i]*math.log((divs[i]), 2)))
    return sol

archivo_ciclos_cholesky = 'chol.txt'
archivo_ciclos_gauss = 'gauss.txt'
archivo_ciclos_wp = 'wp.txt'

#def leer_ciclos(archivo):
    
iteraciones = 80

# Compilo por las dudas
os.system('g++ ../TP1.cpp -O2 -std=c++11 -march=native ../sqrt.o -o ../TP1')
os.system('g++ ../benchgen.cpp -O2 -std=c++11 -march=native -o ../benchgen')
#os.system('rm -r testCases/')
os.system('rm *.txt')
#x = ciclos_gaussian = ciclos_cholesky = ciclos_wp = []
x=[]
ciclos_gaussian=[]
ciclos_cholesky=[]
ciclos_wp=[]

for i in range(15, iteraciones+1):
    os.system('../benchgen {0} > testCases/test{0}'.format(str(i)))
    # Corro el test con los 3 algoritmos
    
    archivo_test = 'testCases/test{0}'.format(str(i))

    os.system('../TP1 ' + archivo_test + ' salida.out 0 >> ' + archivo_ciclos_gauss)
    os.system('../TP1 ' + archivo_test + ' salida.out 1 >> ' + archivo_ciclos_cholesky)    
    os.system('../TP1 ' + archivo_test + ' salida.out 2 >> ' + archivo_ciclos_wp)

f_gaus = open(archivo_ciclos_gauss, 'r')
f_chol = open(archivo_ciclos_cholesky, 'r')
f_wp = open(archivo_ciclos_wp, 'r')
    
for i in range(15,iteraciones+1):
    x.append(i)
    ciclos_gaussian.append(int(f_gaus.readline()))
    ciclos_wp.append(int(f_wp.readline()))
    ciclos_cholesky.append(int(f_chol.readline()))

# Genero el grafico
plt.plot(x, ciclos_gaussian, 'bo',  x, ciclos_cholesky, 'go', x, ciclos_wp, 'ro')
blue_patch = mpatches.Patch(color='blue', label='Gaussian')
green_patch = mpatches.Patch(color='green', label='Cholesky')
red_patch = mpatches.Patch(color='red', label='Winning Percentage')
plt.legend(handles=[blue_patch, green_patch, red_patch])
plt.xlabel('Cantidad de equipos')
plt.ylabel('Ciclos')
plt.savefig('test_ciclos.png')
#plt.show()


plt.close()

plt.plot(x, divl(ciclos_gaussian, x, 2), 'bo', x, divl(ciclos_gaussian, x, 3), 'go', x, divl(ciclos_gaussian, x, 4), 'ro')
blue_patch = mpatches.Patch(color='blue', label='x=n^2')
green_patch = mpatches.Patch(color='green', label='x=n^3')
red_patch = mpatches.Patch(color='red', label='x=n^4')
plt.legend(handles=[blue_patch, green_patch, red_patch])
plt.xlabel('Ciclos/x')
plt.ylabel('Ciclos')
plt.savefig('complejidad_gaussian.png')


plt.close()

plt.plot(x, divl(ciclos_cholesky, x, 2), 'bo', x, divl(ciclos_cholesky, x, 3), 'go', x, divl(ciclos_cholesky, x, 4), 'ro')
blue_patch = mpatches.Patch(color='blue', label='x=n^2')
green_patch = mpatches.Patch(color='green', label='x=n^3')
red_patch = mpatches.Patch(color='red', label='x=n^4')
plt.legend(handles=[blue_patch, green_patch, red_patch])
plt.xlabel('Ciclos/x')
plt.ylabel('Ciclos')
plt.savefig('complejidad_cholesky.png')

plt.close()

plt.plot(x, divl(ciclos_wp, x, 1), 'bo', label='n')
plt.plot(x, divllog(ciclos_wp, x), 'go', label='n*logn')
plt.plot(x, divl(ciclos_wp, x, 2), 'ro', label='n^2')
blue_patch = mpatches.Patch(color='blue', label='x=n')
green_patch = mpatches.Patch(color='green', label='x=n*log(n)')
red_patch = mpatches.Patch(color='red', label='x=n^2')
plt.legend(handles=[blue_patch, green_patch, red_patch])
plt.xlabel('Ciclos/x')
plt.ylabel('Ciclos')
plt.savefig('complejidad_wp.png')