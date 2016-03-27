#include <iostream>
#include <stdlib.h>
#include <list>
#include "matriz.cpp"

using namespace std;

void completeColleyMatrix(matriz &mat, vector<int> &b, list<int*> allTheGames, int equipos);

int main(){

	int equipos;
	int partidos;

	cin >> equipos;
	cin >> partidos;

	list<int*> allTheGames;

	for(int i = 0; i < partidos; i++){
		int *partido = new int [5];
		for(int j = 0; j < 5; j++){
			cin >> partido[j];
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

	matriz matrix = matriz(equipos, equipos);
	vector<int> b (equipos);

	completeColleyMatrix(matrix, b, allTheGames, equipos);
	
	matrix.print();

	matrix.choleskyDecomposition();

	matrix.print();

}

void completeColleyMatrix(matriz &mat, vector<int> &b, list<int*> allTheGames, int equipos){

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
