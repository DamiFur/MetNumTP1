#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc, char * argv[]){

    string inputPath, outputPath; 
    int metodo, equipo; 
    if (argc < 4){ 
        cout << "Input: ";
        cin >> inputPath;
        cout << "Equipo: ";
        cin >> equipo;
        cout << "Metodo (0|1): ";
        cin >> metodo;
    } else {
        inputPath = argv[1];
        equipo = atoi(argv[2]);
        metodo = atoi(argv[3]);
        if (!(metodo == 0 || metodo == 1))
            return 1;
    }

    system('./TP1 ' + inputPath + ' salida.txt 1')

    ifstream input;
    ofstream output;
    input.open(inputPath);
    output.open(inputPath + "_h" + itoa(metodo));

    int equipos;
    int partidos;

    input >> equipos;
    input >> partidos;

    for(int i = 0; i < partidos; i++){
        int *partido = new int [5];
        for(int j = 0; j < 5; j++){
            input >> partido[j];
        }
    }

    return 0;
}