# Punteros en C++

## Autor
Juan Sebastian Tovar Estrada
Yeiser Kaleth Mena Martinez

# Matrices, Punteros y Lambdas en C++

Programa de consola en **C++** que demuestra el uso de matrices dinámicas, punteros a función, lambdas y composición funcional mediante un menú interactivo.

---

## Descripción general

El programa presenta 4 módulos prácticos accesibles desde un menú:

| Opción | Tema |
|--------|------|
| 1 | Matriz dinámica: suma de filas, mayor elemento, transpuesta |
| 2 | Punteros a función: transformar arreglos |
| 3 | Lambdas y composición: suma transformada |
| 4 | Pipeline de transformaciones encadenadas |

---

## Cabeceras e imports

```cpp
#include <iostream>
```
Habilita la entrada/salida estándar (`cin`, `cout`).

```cpp
#include <vector>
```
Permite usar `std::vector`, el arreglo dinámico de la STL.

```cpp
#include <functional>
```
Incluye `std::function`, necesario para almacenar funciones, lambdas y closures como objetos de primera clase.

```cpp
using namespace std;
```
Evita escribir `std::` antes de cada elemento de la biblioteca estándar.

---

## Función `calcularSumasFilas`

```cpp
void calcularSumasFilas(int** matriz, int n, int m)
```
Recibe un **puntero a puntero** (`int**`) que representa una matriz dinámica de `n` filas y `m` columnas.

```cpp
for (int i = 0; i < n; i++) {
    int suma = 0;
    for (int j = 0; j < m; j++) {
        suma += *(*(matriz + i) + j);
    }
```
Recorre cada fila `i` y cada columna `j`. La expresión `*(*(matriz + i) + j)` es aritmética de punteros pura, equivalente a `matriz[i][j]`:
- `matriz + i` → apunta a la fila `i`.
- `*(matriz + i)` → desreferencia y obtiene el puntero al primer elemento de esa fila.
- `+ j` → avanza `j` posiciones dentro de esa fila.
- `*(...)` → desreferencia para obtener el valor.

```cpp
cout << " Fila " << i << ": " << suma << endl;
```
Imprime la suma acumulada de cada fila.

---

## Función `encontrarMayor`

```cpp
int encontrarMayor(int** matriz, int n, int m)
```
Busca y retorna el **valor máximo** de toda la matriz.

```cpp
int mayor = **matriz;
```
Inicializa `mayor` con el primer elemento. `**matriz` equivale a `matriz[0][0]`:
- El primer `*` desreferencia el puntero de filas → obtiene el puntero a la primera fila.
- El segundo `*` desreferencia ese puntero → obtiene el primer entero.

```cpp
if (*(*(matriz + i) + j) > mayor)
    mayor = *(*(matriz + i) + j);
```
Recorre toda la matriz con la misma aritmética de punteros y actualiza `mayor` si encuentra un elemento más grande.

---

## Función `transponer`

```cpp
int** transponer(int** matriz, int n, int m)
```
Genera y retorna una **nueva matriz transpuesta**: si la original es de `n×m`, la transpuesta será de `m×n`.

```cpp
int** t = new int*[m];
```
Reserva dinámicamente en el heap un arreglo de `m` punteros (las nuevas filas).

```cpp
for (int i = 0; i < m; i++) {
    t[i] = new int[n];
```
Para cada fila `i` de la transpuesta, reserva un arreglo de `n` enteros.

```cpp
    for (int j = 0; j < n; j++) {
        t[i][j] = matriz[j][i];
    }
```
Asigna `t[i][j] = matriz[j][i]`: intercambia filas por columnas, que es la definición de transponer.

```cpp
return t;
```
Retorna el puntero a la nueva matriz. **Importante:** quien llame a esta función es responsable de liberar la memoria con `delete[]`.

---

## Punteros a función

```cpp
typedef double (*Operacion)(double);
```
Define `Operacion` como un **alias de tipo** para punteros a funciones que reciben un `double` y retornan un `double`. Permite pasar funciones como parámetros.

```cpp
double cuadrado(double x) { return x * x; }
double doble(double x)    { return x * 2; }
double inverso(double x)  { return (x != 0) ? 1.0 / x : 0; }
```
Tres funciones compatibles con la firma `double(double)`:
- `cuadrado`: eleva al cuadrado.
- `doble`: multiplica por 2.
- `inverso`: retorna `1/x`; si `x == 0` retorna `0` para evitar división por cero.

```cpp
void procesarArreglo(double* arr, int tam, Operacion op)
```
Recibe un arreglo y aplica la operación `op` a cada elemento in-place (modifica el arreglo original).

```cpp
for (int i = 0; i < tam; i++) arr[i] = op(arr[i]);
```
Llama a la función apuntada por `op` con cada elemento y reemplaza su valor.

---

## `std::function` y composición

```cpp
typedef function<double(double)> Transform;
```
Define `Transform` como un tipo que puede almacenar **cualquier callable** (función, lambda, functor) con firma `double(double)`. A diferencia del puntero `Operacion`, `Transform` también acepta lambdas con capturas.

```cpp
double ejecutarComposicion(double* arr, int tam, Transform trans,
                           function<double(double, double)> combinar)
```
Función de **orden superior**: recibe una transformación y una función de combinación, y aplica ambas sobre el arreglo.

```cpp
double resultado = trans(arr[0]);
for (int i = 1; i < tam; i++) {
    resultado = combinar(resultado, trans(arr[i]));
}
return resultado;
```
Transforma el primer elemento como valor inicial, luego itera combinando el acumulado con cada elemento transformado. Es una implementación manual de `reduce/fold`.

---

## Pipeline de transformaciones

```cpp
void aplicarPipeline(double* arr, int tam, const vector<Transform>& pipe)
```
Recibe un arreglo y un **vector de transformaciones**. Aplica cada transformación en orden sobre cada elemento.

```cpp
for (int i = 0; i < tam; i++) {
    for (const auto& f : pipe) arr[i] = f(arr[i]);
}
```
Para cada elemento `i`, recorre todas las funciones del pipeline en orden y aplica cada una al resultado de la anterior. Es una composición funcional encadenada.

---

## `main()` — Menú principal

```cpp
int opcion;
do { ... } while (opcion != 0);
```
Usa un bucle `do-while` para mostrar el menú **al menos una vez** y repetirlo hasta que el usuario ingrese `0`.

```cpp
cin >> opcion;
switch (opcion) { ... }
```
Lee la opción y la despacha al `case` correspondiente.

---

### Caso 1 — Matriz dinámica

```cpp
int** mat = new int*[n];
for (int i = 0; i < n; i++) {
    mat[i] = new int[m];
    ...
}
```
Aloca la matriz fila por fila en el heap. Primero un arreglo de `n` punteros, luego para cada fila un arreglo de `m` enteros.

```cpp
for (int i = 0; i < n; i++) delete[] mat[i];
delete[] mat;
for (int i = 0; i < m; i++) delete[] t[i];
delete[] t;
```
**Libera la memoria** manualmente: primero cada fila, luego el arreglo de punteros. Se hace para ambas matrices (original y transpuesta) para evitar fugas de memoria (`memory leaks`).

---

### Caso 2 — Punteros a función

```cpp
Operacion ops[] = {cuadrado, doble, inverso};
if(sel >= 1 && sel <= 3) {
    procesarArreglo(arr, tam, ops[sel-1]);
```
Almacena las tres funciones en un arreglo de punteros. El usuario selecciona con 1, 2 o 3, y se usa `sel-1` para convertir al índice base 0. Permite **despachar funciones dinámicamente** sin `if/else`.

---

### Caso 3 — Lambdas y composición

```cpp
auto t = [](double x) { return x * 10; };
auto c = [](double acc, double v) { return acc + v; };
```
Define dos **lambdas sin captura** (`[]`):
- `t`: transforma multiplicando por 10.
- `c`: combina sumando el acumulado con el siguiente valor transformado.

```cpp
ejecutarComposicion(arr, tam, t, c)
```
Calcula la suma de todos los elementos multiplicados por 10: `(1×10) + (2×10) + (3×10) = 60`.

---

### Caso 4 — Pipeline

```cpp
vector<Transform> pipe = {
    [](double x) { return x * 2; },
    [](double x) { return x + 3; },
    [](double x) { return x * x; }
};
```
Define un pipeline de 3 lambdas. Cada elemento pasa por los 3 pasos en orden:
- `1 → ×2=2 → +3=5 → ^2=25`
- `2 → ×2=4 → +3=7 → ^2=49`

```cpp
aplicarPipeline(datos, 2, pipe);
```
Aplica el pipeline completo sobre cada elemento del arreglo.

---

## 🛠️ Requisitos

- Compilador C++ con soporte **C++11 o superior** (g++, clang++, MSVC).

## Cómo compilar y ejecutar

```bash
# Compilar
g++ -std=c++17 -o programa main.cpp

# Ejecutar
./programa
```

---

## Conceptos clave cubiertos

| Concepto | Dónde se usa |
|----------|--------------|
| Aritmética de punteros | `calcularSumasFilas`, `encontrarMayor` |
| Matrices dinámicas (`new`/`delete`) | Caso 1 |
| Punteros a función (`typedef`) | `Operacion`, Caso 2 |
| `std::function` y lambdas | `Transform`, Casos 3 y 4 |
| Funciones de orden superior | `ejecutarComposicion`, `aplicarPipeline` |
| Pipeline funcional | Caso 4 |
| Gestión manual de memoria | Todos los `delete[]` |


