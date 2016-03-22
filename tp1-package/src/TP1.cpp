#include <iostream>
#include <stdlib.h>
#include <list>
#include "matriz.cpp"

using namespace std;

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

	for(list<int*>::iterator it = allTheGames.begin(); it != allTheGames.end(); it++){
		for(int i = 0; i < 5; i++){
			cout << (*it)[i] << " ";
		}
		cout << "\n";
	}

	matriz matrix = matriz(equipos, equipos);

	cout << matrix[0][0];

}
