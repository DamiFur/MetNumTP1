#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

const string TEMP_ARCH = "heuristica-temp.out";

vector<pair<double, int> > leerRankings(const string& arch) {
    ifstream in; in.open(arch);
    vector<pair<double, int> > ret;
    double ranking;
    int i = 1;
    while(in >> ranking) {
        ret.push_back(make_pair(ranking, i++));
    }

    in.close();
    sort(ret.rbegin(), ret.rend());
    return ret;
}

int leerCantEquipos(const string& arch) {
    ifstream in; in.open(arch);
    int ret;
    in >> ret;
    in.close();
    return ret;
}

vector<vector<int>> levantarInput(const string& arch) {
    vector<vector<int>> ret;

    ifstream in; in.open(arch);

    int n, k;
    in >> n >> k;
    ret.push_back({n, k});

    for(int i = 0; i<k; ++i) {
        int a, b, c, d, e;
        cin >> a >> b >> c >> d >> e;
        ret.push_back({a, b, c, d, e});
    }

    in.close();
    return ret;
}

void pisarInput(const string& arch, const vector<vector<int>>& mem) {
    ofstream out; out.open(arch);

    for(const vector<int>& linea : mem) {
        for(int i = 0; i<linea.size(); ++i) {
            if (i) cout << " ";
            cout << linea[i];
        }
        cout << endl;
    }
    out.close();
}

int main(int argc, char * argv[]){

    string inputPath, outputPath; 
    int partidos; 
    if (argc < 3){ 
        cout << "Input: ";
        cin >> inputPath;
        cout << "Partidos: ";
        cin >> partidos;
    } else {
        inputPath = argv[1];
        partidos = atoi(argv[2]);
    }

    // nuestro equipo es el N siempre
    int equipo = leerCantEquipos(inputPath);

    for (int p = 0; p<partidos; ++p) {
        string correr = "./TP1 " + inputPath + " " + TEMP_ARCH + " 1";
        system(correr.c_str());
        vector<pair<double, int> > ranking = leerRankings(inputPath);

        vector<vector<int>> inputmem = levantarInput(inputPath);
        // Sumo 1 a K: cantidad de partidos
        inputmem[0][1]++;
        // Juego contra el mejor y le gano
        inputmem.push_back({0, equipo, 1, ranking[0].second, 0});
        // Piso el input
        pisarInput(inputPath, inputmem);
    }

    return 0;
}