#include <iostream>
#include <stdlib.h>
#include <list>
#include <fstream>
#include "matriz.cpp"

using namespace std;

void completeColleyMatrix(matriz &mat, vector<int> &b, vector<int*> allTheGames, int equipos);
void wp(vector<int*> allTheGames, int equipos, ofstream &out);
void printMatriz(matriz &mat, ofstream &out);

int main(){

	string inputPath, outputPath;
	cin >> inputPath >> outputPath;
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

	//Descomentar para testear el input

	// for(list<int*>::iterator it = allTheGames.begin(); it != allTheGames.end(); it++){
	// 	for(int i = 0; i < 5; i++){
	// 		cout << (*it)[i] << " ";
	// 	}
	// 	cout << "\n";
	// }

	if(metodo<2){
		matriz matrix = matriz(equipos, equipos);
		vector<int> b (equipos);
		completeColleyMatrix(matrix, b, allTheGames, equipos);
		if(metodo==0)
			matrix.gaussianElimination();
		else
			matrix.choleskyDecomposition();
		matrix.print(output);
	}else{
		wp(allTheGames, equipos, output);
	}

	input.close();
	output.close();
	for (int i = 0; i < allTheGames.size(); ++i)
		delete allTheGames[i];

}

void completeColleyMatrix(matriz &mat, vector<int> &b, vector<int*> allTheGames, int equipos){

	for(int i = 0; i < equipos; i++){
		for(int j = 0; j < equipos; j++){
			//Fijarse que con esto perdes las ventajas de la matriz esparsa
			mat[i][j] = 0;
		}
	}


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
