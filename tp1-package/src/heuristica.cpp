#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

const string TEMP_ARCH = "heuristica-temp.out";

vector<pair<double, int> > leerRankings(const string& arch, const string& calendario) {
    ifstream in; in.open(arch);
    vector<pair<double, int> > ret;
    double ranking;
    int i = 1;
    while(in >> ranking) {
        // Tiene que poder jugar contra ese equipo o simplemente soy "yo"
        if (calendario[i] > '0' || i+1 == calendario.size()) {
            ret.push_back(make_pair(ranking, i));
        }
        i++;
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
        in >> a >> b >> c >> d >> e;
        ret.push_back({a, b, c, d, e});
    }

    in.close();
    return ret;
}

void pisarInput(const string& arch, const vector<vector<int>>& mem) {
    ofstream out; out.open(arch);

    for(const vector<int>& linea : mem) {
        for(int i = 0; i<linea.size(); ++i) {
            if (i) out << " ";
            out << linea[i];
        }
        out << endl;
    }
    out.close();
}

int main(int argc, char * argv[]){

    string inputPath, outputPath; 
    string calendario; 
    if (argc < 3){ 
        cout << "Input: ";
        cin >> inputPath;
        cout << "Calendario: ";
        cin >> calendario;
    } else {
        inputPath = argv[1];
        calendario = argv[2];
    }

    // nuestro equipo es el N siempre que no debe tener ningun otro partido
    int equipo = leerCantEquipos(inputPath);
    int partidos = 0;
    for (char c : calendario) {
        partidos += c - '0';
    }

    for (int p = 0; p<partidos; ++p) {
        string correr = "./TP1 " + inputPath + " " + TEMP_ARCH + " 1";
        system(correr.c_str());
        vector<pair<double, int> > ranking = leerRankings(TEMP_ARCH, calendario);
        // levanto input
        vector<vector<int>> inputmem = levantarInput(inputPath);
        // Sumo 1 a K: cantidad de partidos
        inputmem[0][1]++;
        // si soy el primer equipo, pierdo contra el ultimo
        if (ranking[0].second == equipo) {
            int oponente = ranking[ranking.size()-1].second;
            // Juego contra el "peor" y pierdo
            inputmem.push_back({0, equipo, 0, oponente, 1})
            calendario[oponente]--;
        } else {
            // Juego contra el mejor y le gano
            inputmem.push_back({0, equipo, 1, ranking[0].second, 0});
            calendario[ranking[0].second]--;
        }
        // Piso el input
        pisarInput(inputPath, inputmem);
    }

    return 0;
}