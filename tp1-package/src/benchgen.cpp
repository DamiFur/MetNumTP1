#include <iostream>
#include <cstdlib>

using namespace std;



int main(){

	int cantEquipos;
	cin >> cantEquipos;
	cout << cantEquipos << " " << cantEquipos*1000 << endl;
	for (int i = 0; i < 1000; ++i)
	{
		for (int j = 0; j < cantEquipos; ++j)
		{
			int v = rand() % 2;
			int r = rand() % cantEquipos;
			while(r==j)
				r = rand() % cantEquipos;
			v <<= 1;
			cout << "1 " << j << " 1 " << r << " " << v << endl;
		}
	}
	



}