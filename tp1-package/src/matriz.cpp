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
    void gaussianElimination();
    void sparseGaussianElimination();
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
 
void matriz::gaussianElimination(){
    int iteraciones = min(this->columnas(), this->filas());
    for (int i = 0; i < iteraciones; ++i)
    {
        int h = i;
        while(h<this->filas() and !this->check(h, i))
            h++;
        if(h==this->filas())
            continue;
        this->swapFilas(h, i);
        for (int j = i+1; j < this->filas(); ++j)
        {
            if(!this->check(j, i))
                continue;
            double pivot = this->fila[j][i]/this->fila[i][i];
            this->erase(j, i);
            for (int k = i+1; k < this->columnas(); ++k)
            {
                if(!this->check(i,k))
                    continue;
                this->fila[j][k]-= this->fila[i][k]*pivot;
                if(this->fila[j][k]<0.00001 and this->fila[j][k]>-0.00001)
                    this->erase(j,k);
            }
        }
    }
}
 
void matriz::sparseGaussianElimination(){
    int iteraciones = min(this->columnas(), this->filas());
    for (int i = 0; i < iteraciones; ++i)
    {
        int h = i;
        while(h<this->filas() and !this->check(h, i))
            h++;
        if(h==this->filas())
            continue;
        this->swapFilas(h, i);
        for (int j = i+1; j < this->filas(); ++j)
        {
            if(!this->check(j, i))
                continue;
            double pivot = this->fila[j][i]/this->fila[i][i];
            for (auto k = this->begin(i); k != this->end(i); ++k)
            {   
                this->fila[j][k->first]-= k->second*pivot;
                if(this->fila[j][k->first]<0.00001 and this->fila[j][k->first]>-0.00001)
                    this->erase(j,k->first);
            }
            this->erase(j, i);
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
