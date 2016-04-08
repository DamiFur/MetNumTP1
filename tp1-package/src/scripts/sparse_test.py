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

def divconst(lista, divs, n, k):
    sol = []
    for i in range(0, len(lista)):
        d = 1
        for j in range(0,n):
            d *= divs[i]
        sol.append(lista[i]/(d/k))
    return sol   

archivo_ciclos_chol_sparse = 'sparse_chol.txt'
archivo_ciclos_gauss_sparse = 'sparse_gauss.txt'

#def leer_ciclos(archivo):
    
#iteraciones = 1000

# Compilo por las dudas
'''os.system('g++ ../TP1.cpp -O2 -std=c++11 -march=native ../sqrt.o -o ../TP1')
os.system('g++ ../sparseBenchgen.cpp -O2 -std=c++11 -march=native -o ../sparseBenchgen')
os.system('rm ' + archivo_ciclos_chol_sparse)
os.system('rm ' + archivo_ciclos_gauss_sparse)'''
#os.system('rm *.txt')
#x = ciclos_gaussian = ciclos_cholesky = ciclos_wp = []
x=[]
ciclos_gaussian=[]
ciclos_chol=[]

i = 100
delta= 1
'''while(i>=0):
    os.system('../sparseBenchgen 700 {0} > sparseTestCases/sparse_test{0}'.format(str(i)))
    # Corro el test con los 3 algoritmos
    
    archivo_test = 'sparseTestCases/sparse_test{0}'.format(str(i))

    os.system('../TP1 ' + archivo_test + ' salida.out 0 >> ' + archivo_ciclos_gauss_sparse)
    os.system('../TP1 ' + archivo_test + ' salida.out 1 >> ' + archivo_ciclos_chol_sparse)
    i-=delta'''

f_gaus = open(archivo_ciclos_gauss_sparse, 'r')
f_chol = open(archivo_ciclos_chol_sparse, 'r')


i = 100
while(i>=0):
    x.append(i)
    ciclos_gaussian.append(int(f_gaus.readline()))
    ciclos_chol.append(int(f_chol.readline()))
    i-=delta

# Genero el grafico
plt.plot(x, ciclos_gaussian, 'bo',  x, ciclos_chol, 'go')
blue_patch = mpatches.Patch(color='blue', label='Sparse Gaussian')
green_patch = mpatches.Patch(color='green', label='Sparse Cholesky')
plt.legend(handles=[blue_patch, green_patch])
plt.xlabel('Porcentaje de zeros')
plt.ylabel('Ciclos')
plt.savefig('test_sparse.png')
#plt.show()

plt.close()

plt.plot(x, divl(ciclos_gaussian, ciclos_chol, 1), 'bo')
blue_patch = mpatches.Patch(color='blue', label='Gaussian/Cholesky')
plt.legend(handles=[blue_patch])
plt.xlabel('Porcentaje de zeros')
plt.ylabel('Ciclos')
plt.savefig('sparse_gaussian_vs_cholesky.png')

plt.close()