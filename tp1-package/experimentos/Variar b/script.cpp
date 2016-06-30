#include <iostream>
#include <map>
#include <string>

using namespace std;

int main(){
	string id;
	map<string, int> teamId;
	map<string, int> fechaid;
	cout << 11 << " " << 30 << endl;
	while(cin >> id){
		string fecha, equipoA, equipoB, resultado, set1, set2, set3, set4, set5;
		cin >> fecha >> equipoA >> equipoB >> resultado >> set1 >> set2 >> set3 >> set4 >> set5;
		if(teamId.count(equipoA)==0)
			teamId[equipoA] = teamId.size()+1;
		if(teamId.count(equipoB)==0)
			teamId[equipoB] = teamId.size()+1;
		if(fechaid.count(fecha)==0)
			fechaid[fecha] = fechaid.size()+1;

		cout << fechaid[fecha] << " " << teamId[equipoA] << " " << resultado[0] << " " << teamId[equipoB] << " " << resultado[2] << endl;

	}

}