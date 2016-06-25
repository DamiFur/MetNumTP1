#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

const int N = 40;
const int K = 2000;
const int MAX_GOLES_A = 40;
const int MAX_GOLES_B = 40;

int main(int argc, char * argv[]){
    freopen("test_heuristica.in", "w", stdout);
    printf("%i %i\n", N, K);

    srand(time(NULL));
    for (int k = 0; k<K; ++k) {
        int equipA, equipB;
        do {
            equipA = rand()%(N-1) + 1;
            equipB = rand()%(N-1) + 1;
        } while (equipA == equipB);

        int golesA = rand()%MAX_GOLES_A + 1, golesB = rand()%MAX_GOLES_B + 1;
        if (golesA == golesB) {
            if (rand()%2 == 0) {
                golesA++;
            } else {
                golesB++;
            }
        }

        printf("0 %i %i %i %i\n", equipA, golesA, equipB, golesB);
    }

    return 0;
}