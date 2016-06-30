#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

const string TEMP_ARCH = "heuristica-temp.out";

vector<pair<double, int> > leerRankings(const string& arch, const string& calendario, int equipo) {
    ifstream in; in.open(arch);
    vector<pair<double, int> > ret;
    double ranking;
    int i = 1;
    while(in >> ranking) {
        // Tiene que poder jugar contra ese equipo o simplemente soy "yo"
        if (calendario[i] > '0' || i == equipo) {
            ret.push_back(make_pair(ranking, i));
        }
        i++;
    }

    in.close();
    sort(ret.rbegin(), ret.rend());
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

string filtrarPartidosEquipo(vector<vector<int>>& partidos, int equipo, int cantEquipos) {
    // Elimina todos los partidos del equipo y devuelve el "calendario"
    string calendario(cantEquipos+1, '0');
    int ganados = 0, cantPartidos = 0;
    for (int i = 1; i<partidos.size(); ++i) {
        if (partidos[i][1] == equipo) {
            if (partidos[i][2] > partidos[i][4])
                ganados++;
            cantPartidos++;
            // sumo al calendario
            calendario[partidos[i][3]]++;
            // borro el resultado
            partidos.erase(partidos.begin()+i);
            // resto uno a la cantidad de partidos
            partidos[0][1]--;
            // muevo i porque el elemento no existe mas
            --i;
        } else if (partidos[i][3] == equipo) {
            if (partidos[i][2] < partidos[i][4])
                ganados++;
            cantPartidos++;
            calendario[partidos[i][1]]++;
            partidos.erase(partidos.begin()+i);
            partidos[0][1]--;
            --i;
        }
    }

    cout << "Calendario: " << calendario << endl;
    cout << "Ganados: " << ganados << "/" << cantPartidos << endl;

    return calendario;
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
    int equipo, cantEquipos;
    if (argc < 4){ 
        cout << "Input: ";
        cin >> inputPath;
        cout << "Equipo: ";
        cin >> equipo;
        cout << "Cantidad de equipos: ";
        cin >> cantEquipos;
    } else {
        inputPath = argv[1];
        equipo = atoi(argv[2]);
        cantEquipos = atoi(argv[3]);
    }

    // Limpio el input de los partidos del equipo
    vector<vector<int>> inputmem;
    inputmem = levantarInput(inputPath);
    string calendario = filtrarPartidosEquipo(inputmem, equipo, cantEquipos);
    pisarInput(inputPath, inputmem);

    // veo cuantos partidos juego yo
    int partidos = 0;
    for (char c : calendario) {
        partidos += c - '0';
    }
    int ganados = 0;
    for (int p = 0; p<partidos; ++p) {
        string correr = "./TP1 " + inputPath + " " + TEMP_ARCH + " 1";
        system(correr.c_str());
        vector<pair<double, int> > ranking = leerRankings(TEMP_ARCH, calendario, equipo);
        // levanto input
        inputmem = levantarInput(inputPath);
        // Sumo 1 a K: cantidad de partidos
        inputmem[0][1]++;
        // si soy el primer equipo, pierdo contra el ultimo
        if (ranking[0].second == equipo) {
            int oponente = ranking[ranking.size()-1].second;
            // Juego contra el "peor" y pierdo
            inputmem.push_back({0, equipo, 0, oponente, 1});
            calendario[oponente]--;
        } else {
            // Juego contra el mejor y le gano
            ganados++;
            inputmem.push_back({0, equipo, 1, ranking[0].second, 0});
            calendario[ranking[0].second]--;
        }
        // Piso el input
        pisarInput(inputPath, inputmem);
    }

    cout << "Ganados: " << ganados << "/" << partidos << endl;
    return 0;
}