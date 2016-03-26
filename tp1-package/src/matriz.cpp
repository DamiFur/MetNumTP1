#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
 
using namespace std;
 
struct pointer
{
    unordered_map<int, double> * col;
    double &operator[](int i){
        return (*col)[i];
    }
}__attribute__((packed));
 
class matriz
{
private:
    int fils;
    int columns;
    vector<pointer> fila;
public:
    void print();
    ~matriz(){
        for (int i = 0; i < fila.size(); ++i)
            {
                delete fila[i].col;
            }
    };
    matriz(int i, int j){
        fila.reserve(i);
        fils = i;
        columns = j;
        for (int x = 0; x < i; ++x)
        {
            pointer tmp;
            tmp.col = new unordered_map<int, double>;
            tmp.col->reserve(j);
            fila.push_back(tmp);
        }
        return;
    };
    matriz(vector<int> &v, int j){
        fila.reserve(v.size());
        fils = v.size();
        columns = j;
        for (int x = 0; x < v.size(); ++x)
        {
            pointer tmp;
            tmp.col = new unordered_map<int, double>;
            tmp.col->reserve(v[x]);
            fila.push_back(tmp);
        }
        return;
    }
    pointer &operator[](int i){
        return fila[i];
    };
    bool check(int i, int j){
        return fila[i].col->count(j);
    }
    void erase(int i, int j){
        fila[i].col->erase(j);
    }
    unordered_map<int, double>::iterator begin(int i){
        return fila[i].col->begin();
    }
    unordered_map<int, double>::iterator end(int i){
        return fila[i].col->end();
    }
 
    void swapFilas(int a, int b){
        pointer c = fila[a];
        fila[a] = fila[b];
        fila[b] = c;
    }
    int filas(){return fils;}
    int columnas(){return columns;}
};
 
void gaussianElimination(matriz &mat){
    int iteraciones = min(mat.columnas(), mat.filas());
    for (int i = 0; i < iteraciones; ++i)
    {
        int h = i;
        while(h<mat.filas() and !mat.check(h, i))
            h++;
        if(h==mat.filas())
            continue;
        mat.swapFilas(h, i);
        for (int j = i+1; j < mat.filas(); ++j)
        {
            if(!mat.check(j, i))
                continue;
            double pivot = mat[j][i]/mat[i][i];
            mat.erase(j, i);
            for (int k = i+1; k < mat.columnas(); ++k)
            {
                if(!mat.check(i,k))
                    continue;
                mat[j][k]-= mat[i][k]*pivot;
                if(mat[j][k]<0.00001 and mat[j][k]>-0.00001)
                    mat.erase(j,k);
            }
        }
    }
}
 
void sparseGaussianElimination(matriz &mat){
    int iteraciones = min(mat.columnas(), mat.filas());
    for (int i = 0; i < iteraciones; ++i)
    {
        int h = i;
        while(h<mat.filas() and !mat.check(h, i))
            h++;
        if(h==mat.filas())
            continue;
        mat.swapFilas(h, i);
        for (int j = i+1; j < mat.filas(); ++j)
        {
            if(!mat.check(j, i))
                continue;
            double pivot = mat[j][i]/mat[i][i];
            for (auto k = mat.begin(i); k != mat.end(i); ++k)
            {   
                mat[j][k->first]-= k->second*pivot;
                if(mat[j][k->first]<0.00001 and mat[j][k->first]>-0.00001)
                    mat.erase(j,k->first);
            }
            mat.erase(j, i);
        }
    }
}

void matriz::print(){
    for(int i = 0; i < this->filas(); i++){
        unordered_map<int, double>::iterator b = this->begin(i);
        for(auto it = this->begin(i); it != this->end(i); ++it){
            cout << it->first << ": " << it->second << " ";
        }
        cout << "\n";
    }
}

void choleskyDecomposition(matriz &mat){
    for (int i = 0; i < mat.filas(); ++i)
    {
        double diag = sqrt(mat[i][i]);
        mat[i][i] = diag;
        for (int j = i+1; j < mat.filas(); ++j)
            mat[j][i] /= diag;
        for (int j = i+1; j < mat.filas(); ++j)
        {
        for (int k = i+1; k <= j; ++k)
            mat[j][k] -= mat[j][i]*mat[k][i];
        }
    }
}
