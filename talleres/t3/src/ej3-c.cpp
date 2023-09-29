#include <thread>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>

// Rows and columns
#define RR 3
#define CC 3

using namespace std;

atomic_int pos(0);

void imprimoVector(vector<int> v, int vectorSize){
    for (int i = 0; i < vectorSize; ++i) {
        if(i==0) { cout << " [ " << v[i] << ", "; }
        if(i==vectorSize-1){ cout << v[i] << " ]" << endl; }
        if(i>0 && i<vectorSize-1){
            cout << v[i] << ", ";
        }
    }
}
void imprimoMatriz(vector<vector<int>> m, int matrixSize){
    for (int i = 0; i < matrixSize; ++i) {
        cout << "Fila " << i << ": ";
        for (int j = 0; j < matrixSize; ++j) {
            if(j==0) { cout << " [ " << m[i][j] << ", "; }
            if(j==matrixSize-1){ cout << m[i][j] << " ]" << endl; }
            if(j>0 && j<matrixSize-1){
                cout << m[i][j] << ", ";
            }
        }
    }
}
vector<int> tomarColumna(vector<vector<int>> matrix, int columna){
    vector<int> resultado;

    for(int i=0; i<RR; i++){
        for(int j=0; j<CC; j++){
            if(j==columna){ resultado.push_back(matrix[i][j]); }
        }
    }
    return resultado;
}



int main() {
    vector<vector<int>> matrix1(RR, vector<int>(CC));
    vector<vector<int>> matrix2(RR, vector<int>(CC));
    vector<vector<int>> matrixRes(RR, vector<int>(CC));

    // Inicializo valores de matrices
    int matrixSize = CC;
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            matrix1[i][j] = i * matrixSize + j + 1;
            matrix2[i][j] = (matrixSize * matrixSize) - (i * matrixSize + j);
        }
    }


    /** TO-DO **/

    /** IMPRIMO MATRICES **/
    imprimoMatriz(matrix1, matrixSize);
    imprimoMatriz(matrix2, matrixSize);
    imprimoMatriz(matrixRes, matrixSize);

    return 0;
}

