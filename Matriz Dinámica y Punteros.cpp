#include <iostream>
#include <vector>
#include <functional>

using namespace std;

void calcularSumasFilas(int** matriz, int n, int m) {
    cout << "\n>>> Sumas por fila:" << endl;
    for (int i = 0; i < n; i++) {
        int suma = 0;
        for (int j = 0; j < m; j++) {
            suma += *(*(matriz + i) + j); 
        }
        cout << " Fila " << i << ": " << suma << endl;
    }
}

int encontrarMayor(int** matriz, int n, int m) {
    int mayor = **matriz;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (*(*(matriz + i) + j) > mayor) 
                mayor = *(*(matriz + i) + j);
        }
    }
    return mayor;
}

int** transponer(int** matriz, int n, int m) {
    int** t = new int*[m];
    for (int i = 0; i < m; i++) {
        t[i] = new int[n];
        for (int j = 0; j < n; j++) {
            t[i][j] = matriz[j][i];
        }
    }
    return t;
}

typedef double (*Operacion)(double);
double cuadrado(double x) { return x * x; }
double doble(double x) { return x * 2; }
double inverso(double x) { return (x != 0) ? 1.0 / x : 0; }

void procesarArreglo(double* arr, int tam, Operacion op) {
    for (int i = 0; i < tam; i++) arr[i] = op(arr[i]);
}

typedef function<double(double)> Transform;

double ejecutarComposicion(double* arr, int tam, Transform trans, function<double(double, double)> combinar) {
    double resultado = trans(arr[0]);
    for (int i = 1; i < tam; i++) {
        resultado = combinar(resultado, trans(arr[i]));
    }
    return resultado;
}

void aplicarPipeline(double* arr, int tam, const vector<Transform>& pipe) {
    for (int i = 0; i < tam; i++) {
        for (const auto& f : pipe) arr[i] = f(arr[i]);
    }
}

int main() {
    int opcion;

    do {
        cout << "\n========================================";
        cout << "\n        MENU PRINCIPAL DE C++";
        cout << "\n========================================";
        cout << "\n1. Matriz Dinamica (Suma, Mayor, Transpuesta)";
        cout << "\n2. Punteros a Funcion (Transformar arreglo)";
        cout << "\n3. Lambdas y Composicion (Suma transformada)";
        cout << "\n4. BONUS: Pipeline de Transformaciones";
        cout << "\n0. Salir del programa";
        cout << "\n----------------------------------------";
        cout << "\nSeleccione una opcion: "; 
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int n, m;
                cout << "Ingrese filas y columnas (n m): "; cin >> n >> m;
                int** mat = new int*[n];
                for (int i = 0; i < n; i++) {
                    mat[i] = new int[m];
                    for (int j = 0; j < m; j++) {
                        cout << "  Elemento [" << i << "][" << j << "]: "; cin >> mat[i][j];
                    }
                }
                
                calcularSumasFilas(mat, n, m);
                cout << ">>> Mayor elemento: " << encontrarMayor(mat, n, m) << endl;
                
                int** t = transponer(mat, n, m);
                cout << ">>> Primera fila de la transpuesta: ";
                for(int j=0; j<n; j++) cout << t[0][j] << " ";
                cout << endl;

                // Liberar matriz original
                for (int i = 0; i < n; i++) delete[] mat[i];
                delete[] mat;
                // Liberar transpuesta
                for (int i = 0; i < m; i++) delete[] t[i];
                delete[] t;
                break;
            }
            case 2: {
                int tam; cout << "Tamano del arreglo: "; cin >> tam;
                double* arr = new double[tam];
                cout << "Ingrese " << tam << " valores: ";
                for(int i=0; i<tam; i++) cin >> arr[i];
                
                cout << "Operacion (1: Cuadrado, 2: Doble, 3: Inverso): ";
                int sel; cin >> sel;
                
                Operacion ops[] = {cuadrado, doble, inverso};
                if(sel >= 1 && sel <= 3) {
                    procesarArreglo(arr, tam, ops[sel-1]);
                    cout << ">>> Resultado: ";
                    for(int i=0; i<tam; i++) cout << arr[i] << " ";
                    cout << endl;
                } else {
                    cout << "Opcion no valida." << endl;
                }
                
                delete[] arr;
                break;
            }
            case 3: {
                int tam = 3;
                double* arr = new double[3]{1, 2, 3};
                cout << "Usando arreglo base: [1, 2, 3]" << endl;
                
                auto t = [](double x) { return x * 10; };
                auto c = [](double acc, double v) { return acc + v; };
                
                cout << ">>> Suma de cada elemento multiplicado por 10: " 
                     << ejecutarComposicion(arr, tam, t, c) << endl;
                
                delete[] arr;
                break;
            }
            case 4: {
                double* datos = new double[2]{1, 2};
                cout << "Pipeline sobre: [1, 2]" << endl;
                cout << "Pasos: (x*2) -> (x+3) -> (x^2)" << endl;

                vector<Transform> pipe = {
                    [](double x) { return x * 2; },
                    [](double x) { return x + 3; },
                    [](double x) { return x * x; }
                };

                aplicarPipeline(datos, 2, pipe);
                cout << ">>> Resultado final: [" << datos[0] << ", " << datos[1] << "]" << endl;
                
                delete[] datos;
                break;
            }
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida, intente de nuevo." << endl;
        }

    } while (opcion != 0);

    return 0;
}
