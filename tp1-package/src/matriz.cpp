#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
 
using namespace std;

/*extern "C"{
    extern double asmsqrt(double *x);
}*/

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
    unsigned long long int gaussianElimination();
    unsigned long long int sparseGaussianElimination();
    unsigned long long int choleskyDecomposition();
    void print(ofstream &out);
    void print(ostream &out);
    vector<double> resolver_sistema_inferior(vector<double> b);
    vector<double> resolver_sistema_superior(vector<double> b);
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
            tmp.col->reserve(min(j, MAX_RESERVE_MATRIX));
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

unsigned long long int matriz::gaussianElimination(){
    unsigned long long int t1 = rdtsc();
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
    return rdtsc()-t1;
}
 
unsigned long long int matriz::sparseGaussianElimination(){
    unsigned long long int t1 = rdtsc();
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
    return rdtsc()-t1;
}



unsigned long long int matriz::choleskyDecomposition(){
    unsigned long long int t1 = rdtsc();
    for (int i = 0; i < this->filas(); ++i)
    {
//        double diag = asmsqrt(&this->fila[i][i]);
        double diag = sqrt(this->fila[i][i]);
        this->fila[i][i] = diag;
        for (int j = i+1; j < this->filas(); ++j)
            this->fila[j][i] /= diag;
        for (int j = i+1; j < this->filas(); ++j)
        {
            for (int k = i+1; k <= j; ++k)
                this->fila[j][k] -= this->fila[j][i]*this->fila[k][i];
        }
    }

    for(int k = 0; k < this->filas(); ++k){
        for(int l = k - 1; l >= 0; --l){
            (*this)[l][k] = (*this)[k][l];
        }
    }
    return rdtsc()-t1;
}


void matriz::print(ofstream &out){
    for(int i = 0; i < this->filas() ; i++){
        for (int j = 0; j < this->columnas(); ++j)
        {
            if(this->check(i, j))
                out << (*this)[i][j] << "\t\t";
            else
                out << 0 << "\t\t";
        }
        out << endl;
    }
}

void matriz::print(ostream &out){
    for(int i = 0; i < this->filas() ; i++){
        for (int j = 0; j < this->columnas(); ++j)
        {
            if(this->check(i, j))
                out << (*this)[i][j] << "\t\t";
            else
                out << 0 << "\t\t";
        }
        out << endl;
    }
}

vector<double> matriz::resolver_sistema_inferior(vector<double> b){
    // a 0 0 0
    // b c 0 0
    // d e f 0
    // g h i j
    // resuelvo de arriba para abajo un triangular inferior tal que
    // Ax = b utilizando sustitucion
    vector<double> ret(b.size(), 0.0);

    for (int i = 0; i<b.size(); ++i){
        double sum = 0.0;
        for(int j = 0; j<i; ++j){
            if(fila[i].count(j)) {
                sum += ret[j] * fila[i][j];
            }
        }
        if(fila[i].count(i)) {
            ret[i] = (b[i]-sum)/fila[i][i];
        }
    }    

    return ret;
}

vector<double> matriz::resolver_sistema_superior(vector<double> b){
    // a b c d
    // 0 e f g
    // 0 0 h i
    // 0 0 0 j
    // resuelvo de abajo para arriba un triangular superior tal que
    // Ax = b utilizando sustitucion
    vector<double> ret(b.size(), 0.0);

    for (int i = b.size()-1; i>=0; --i){
        double sum = 0.0;
        for(int j = i+1; j<b.size(); ++j) {
            if(fila[i].count(j)) {
                sum += ret[j] * fila[i][j];
            }
        }
        if(fila[i].count(i)) {
            ret[i] = (b[i]-sum)/fila[i][i];
        }
    }    

    return ret;
}

