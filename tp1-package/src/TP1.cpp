#include <iostream>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <chrono>
#include <ctime>
#include "matriz.cpp"

using namespace std;

void completeColleyMatrix(matriz &mat, vector<int> &b, vector<int*> allTheGames, int equipos);
void wp(vector<int*> allTheGames, int equipos, ofstream &out);
void printMatriz(matriz &mat, ofstream &out);

int main(){

	string inputPath, outputPath;
	cin >> inputPath;
	cin >> outputPath;
	int metodo;
	cin >> metodo;

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

	if(metodo<2){
		matriz matrix = matriz(equipos, equipos);
		vector<int> b (equipos);
		completeColleyMatrix(matrix, b, allTheGames, equipos);
		// matrix.print(cout);
		std::chrono::time_point<std::chrono::system_clock> start, end;
		if(metodo==0){
			cout << "entring gaussianElimination" << endl;
			start = std::chrono::system_clock::now();
			matrix.gaussianElimination();
			end = std::chrono::system_clock::now();
			cout << "exiting gaussianElimination" << endl;
		}
		else{
			cout << "entring choleskyDecomposition" << endl;
			start = std::chrono::system_clock::now();
			matrix.choleskyDecomposition();
			end = std::chrono::system_clock::now();
			cout << "exiting choleskyDecomposition" << endl;
		}
		matrix.print(output);
		cout << "Amount of operations: " << matrix.getCantOp() << endl;
		cout << "Time elapsed: " << chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << endl;
	}else{
		wp(allTheGames, equipos, output);
	}

	input.close();
	output.close();
	for (int i = 0; i < allTheGames.size(); ++i)
		delete allTheGames[i];

}

void completeColleyMatrix(matriz &mat, vector<int> &b, vector<int*> allTheGames, int equipos){

	unordered_map<int, int> traductorEquipoIndice;
	int indice = 0;

	for(auto it = allTheGames.begin(); it != allTheGames.end(); it++){

		if(traductorEquipoIndice.count((*it)[1]) == 0){
			traductorEquipoIndice[(*it)[1]] = indice++;
			b[traductorEquipoIndice[(*it)[1]]] = 0;
		}

		if(traductorEquipoIndice.count((*it)[3]) == 0){
			traductorEquipoIndice[(*it)[3]] = indice++;
			b[traductorEquipoIndice[(*it)[3]]] = 0;
		}

		bool winner = ((*it)[2] > (*it)[4]);
		if(winner){
			b[traductorEquipoIndice[(*it)[1]]]++;
			b[traductorEquipoIndice[(*it)[3]]]--;
		} else {
			b[traductorEquipoIndice[(*it)[3]]]++;
			b[traductorEquipoIndice[(*it)[1]]]--;
		}

		mat[traductorEquipoIndice[(*it)[1]]][traductorEquipoIndice[(*it)[1]]]++;
		mat[traductorEquipoIndice[(*it)[3]]][traductorEquipoIndice[(*it)[3]]]++;
		mat[traductorEquipoIndice[(*it)[3]]][traductorEquipoIndice[(*it)[1]]]--;
		mat[traductorEquipoIndice[(*it)[1]]][traductorEquipoIndice[(*it)[3]]]--;
	}


	for(auto it = b.begin(); it != b.end(); it++){
		*it /= 2;
		(*it)++;
	}

	for(int l = 0; l < equipos; l++){
		mat[l][l] += 2;
	}
}


void wp(vector<int*> allTheGames, int cantEquipos, ofstream &out){
	vector<pair<double, double> > v;
	v.reserve(cantEquipos+1);
	for (int i = 0; i <= cantEquipos; ++i)
		v.push_back(make_pair(0, 0));
	for (int i = 0; i < allTheGames.size(); ++i)
	{
		v[allTheGames[i][1]].second++;
		v[allTheGames[i][3]].second++;
		if(allTheGames[i][2]>allTheGames[i][4])
			v[allTheGames[i][1]].first++;
		else
			v[allTheGames[i][3]].first++;
	}
	for (int i = 1; i <= cantEquipos; ++i)
	{
		v[i].first /= v[i].second;
		v[i].second = i;
	}
	sort(v.begin(), v.end());
	for (int i = cantEquipos; i; --i)
	{
		out << cantEquipos-i+1 << ".  " << v[i].second << "  "  << v[i].first << endl; 
	}
}
