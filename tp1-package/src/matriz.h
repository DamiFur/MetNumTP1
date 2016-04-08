#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
 
using namespace std;

extern "C"{
    extern double asmsqrt(double *x);
}

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

const int MAX_RESERVE_MATRIX = 1024;
 
struct pointer
{
    unordered_map<int, double> * col;
    double &operator[](int i){
        return (*col)[i];
    }
    bool count(int i) {
        return col->count(i);
    }
}__attribute__((packed));
 
class matriz
{
private:
    int fils;
    int columns;
    vector<pointer> fila;
public:
	~matriz();
	matriz(int i, int j);
	matriz(vector<int> &v, int j);
    unsigned long long int gaussianElimination();
    unsigned long long int sparseGaussianElimination();
    unsigned long long int choleskyDecomposition();
    void print(ofstream &out);
    void print(ostream &out);
    vector<double> resolver_sistema_inferior(vector<double> b);
    vector<double> resolver_sistema_superior(vector<double> b);
    pointer &operator[](int i);
    bool check(int i, int j);
    void erase(int i, int j);
    unordered_map<int, double>::iterator begin(int i);
    unordered_map<int, double>::iterator end(int i);

    void swapFilas(int a, int b);
    int filas();
    int columnas();
};

