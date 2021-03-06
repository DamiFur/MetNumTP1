#include <iostream>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <chrono>
#include <ctime>
#include <map>
#include "matriz.h"

using namespace std;

void completeColleyMatrix(matriz &mat, vector<int*> allTheGames, int equipos);
unsigned long long int wp(vector<int*> allTheGames, int equipos, ofstream &out);
void printMatriz(matriz &mat, ofstream &out);
unordered_map<int, int> traductorEquipoIndice, traductorIndiceEquipo;

int main(int argc, char * argv[]){

	string inputPath, outputPath; 
	int metodo; 
	if (argc < 4){ 
		cout << "Input: ";
		cin >> inputPath;
		cout << "Output: ";
		cin >> outputPath; 
		cout << "Metodo (0|1|2): ";
		cin >> metodo;
	} else {
		inputPath = argv[1];
		outputPath = argv[2];
		metodo = atoi(argv[3]);
		if (!(metodo == 0 || metodo == 1 || metodo == 2))
			return 1;
	}
		
	ifstream input;
	ofstream output;
	input.open(inputPath);
	output.open(outputPath);

	int equipos;
	int partidos;

	input >> equipos;
	input >> partidos;

	vector<int*> allTheGames;
	allTheGames.reserve(partidos);

	for(int i = 0; i < partidos; i++){
		int *partido = new int [5];
		for(int j = 0; j < 5; j++){
			input >> partido[j];
		}
		allTheGames.push_back(partido);
	}

	// Descomentar para testear el input

	// for(auto it = allTheGames.begin(); it != allTheGames.end(); it++){
	// 	for(int i = 0; i < 5; i++){
	// 		cout << (*it)[i] << " ";
	// 	}
	// 	cout << "\n";
	// }
	unsigned long long int ciclos = 0;
	if(metodo<2){
		matriz matrix = matriz(equipos, equipos + 1);
		completeColleyMatrix(matrix, allTheGames, equipos);
		// matrix.print(cout);
		std::chrono::time_point<std::chrono::system_clock> start, end;
		map<int, double> resultado_equipos;
		if(metodo==0){
			ciclos = matrix.sparseGaussianElimination();
			// Resuelvo el sistema de ecuaciones
			vector<double> b(matrix.filas());
			for (int i = 0; i<b.size(); ++i) {
				b[i] = matrix[i][matrix.columnas()-1];
			}
			vector<double> res = matrix.resolver_sistema_superior(b);
			for (int i = 0; i<res.size(); ++i) {
				resultado_equipos[traductorIndiceEquipo[i]] = res[i];
			}
		}
		else{
			ciclos = matrix.choleskyDecomposition();
			// Resuelvo el sistema de ecuaciones
			// L * Lt * x = b
			// (2) Lt * x = y
			// (1) L * y = b
			// Siedo L triangular inferior de la matriz de cholesky y Lt superior
			// Primero resuelvo (1) para hallar y y luego remplazo en (2) y resuelvo
			vector<double> b(matrix.filas());
			for (int i = 0; i<b.size(); ++i) {
				b[i] = matrix[i][matrix.columnas()-1];
			}
			// Resuelvo el sistema (1)
			vector<double> y = matrix.resolver_sistema_inferior(b);
			// Resuelvo el sistema (2)
			vector<double> x = matrix.resolver_sistema_superior(y);
			for (int i = 0; i<x.size(); ++i) {
				resultado_equipos[traductorIndiceEquipo[i]] = x[i];
			}
		}
		for (auto res_e : resultado_equipos) {
			output << res_e.second << endl;
		}
		//matrix.print(cout);	
	}else{
		ciclos = wp(allTheGames, equipos, output);
	}
	//cout << ciclos << endl;
	input.close();
	output.close();
	for (int i = 0; i < allTheGames.size(); ++i)
		delete allTheGames[i];

}

void completeColleyMatrix(matriz &mat, vector<int*> allTheGames, int equipos){

	int indice = 0;
	traductorIndiceEquipo.clear();
	for(auto it = allTheGames.begin(); it != allTheGames.end(); it++){

		if(traductorEquipoIndice.count((*it)[1]) == 0){
			traductorIndiceEquipo[indice] = (*it)[1];
			traductorEquipoIndice[(*it)[1]] = indice++;
			mat[traductorEquipoIndice[(*it)[1]]][equipos] = 0;
		}

		if(traductorEquipoIndice.count((*it)[3]) == 0){
			traductorIndiceEquipo[indice] = (*it)[3];
			traductorEquipoIndice[(*it)[3]] = indice++;
			mat[traductorEquipoIndice[(*it)[3]]][equipos] = 0;
		}

		bool winner = ((*it)[2] > (*it)[4]);
		if(winner){
			mat[traductorEquipoIndice[(*it)[1]]][equipos]++;
			mat[traductorEquipoIndice[(*it)[3]]][equipos]--;
		} else {
			mat[traductorEquipoIndice[(*it)[3]]][equipos]++;
			mat[traductorEquipoIndice[(*it)[1]]][equipos]--;
		}

		mat[traductorEquipoIndice[(*it)[1]]][traductorEquipoIndice[(*it)[1]]]++;
		mat[traductorEquipoIndice[(*it)[3]]][traductorEquipoIndice[(*it)[3]]]++;
		mat[traductorEquipoIndice[(*it)[3]]][traductorEquipoIndice[(*it)[1]]]--;
		mat[traductorEquipoIndice[(*it)[1]]][traductorEquipoIndice[(*it)[3]]]--;
	}


	for(int i = 0; i < equipos; i++){
		mat[i][equipos] /= 2;
		mat[i][equipos]++;
	}

	for(int l = 0; l < equipos; l++){
		mat[l][l] += 2;
	}
}


unsigned long long int wp(vector<int*> allTheGames, int cantEquipos, ofstream &out){
	unsigned long long int t1 = rdtsc();
	vector<pair<double, double> > v;
	v.reserve(cantEquipos+1);
	traductorIndiceEquipo.clear();
	traductorEquipoIndice.clear();

	for (int i = 0; i < cantEquipos; ++i)
		v.push_back(make_pair(0, 0));
	int indice = 0;
	for (int i = 0; i < allTheGames.size(); ++i)
	{
		int i1 = allTheGames[i][1], i3 = allTheGames[i][3];
		if (!traductorEquipoIndice.count(i1)) {
			traductorEquipoIndice[i1] = indice;
			traductorIndiceEquipo[indice++] = i1;
		}
		if (!traductorEquipoIndice.count(i3)) {
			traductorEquipoIndice[i3] = indice;
			traductorIndiceEquipo[indice++] = i3;
		}
		i1 = traductorEquipoIndice[i1];
		i3 = traductorEquipoIndice[i3];
		v[i1].second++;
		v[i3].second++;
		if(allTheGames[i][2]>allTheGames[i][4])
			v[i1].first++;
		else
			v[i3].first++;
	}
	vector<pair<int, double> > ranking;
	for (int i = 0; i < cantEquipos; ++i) {
		ranking.push_back(make_pair(traductorIndiceEquipo[i], v[i].first / v[i].second));
	}

	sort(ranking.begin(), ranking.end());
	for (int i = 0; i < cantEquipos; ++i) {
		out << ranking[i].second << endl;
	}

	return rdtsc()-t1;
}
