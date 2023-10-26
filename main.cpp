#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <thread>
using namespace std;

void ejercicio_1() {
    int n = 10000;
    vector<int> vd(n);
    // llenar el vector con valores desde 1 hasta n
    iota(begin(vd), end(vd), 1);
    // calcular usando accumulate
    auto start = chrono::high_resolution_clock::now();
    auto total = accumulate(begin(vd), end(vd), static_cast<long long int>(0));
    auto finish = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(finish - start).count() << endl;
    cout << total << endl;
}

void acumular(long long int& result, vector<int>::iterator first, vector<int>::iterator last) {
    result = accumulate(first, last, static_cast<long long int>(0));
}

void ejercicio_2() {
    int n = 10000;
    vector<int> vd(n);
    // llenar el vector con valores desde 1 hasta n
    iota(begin(vd), end(vd), 1);

    auto start = chrono::high_resolution_clock::now();
    // Cantidad de hilos
    int nt = thread::hardware_concurrency();
    if (n < 10000000) {
        nt = 1;
    }
    cout << "Hilos de nucleo: " << nt << endl;

    // Crear hilos
    vector<jthread> vt(nt);

    // Crear un vector de resultado
    vector<long long> vr(nt);

    // Distribuyendo las tareas
    auto range = n / nt;

    auto first = begin(vd);
    auto last = next(first, range);
    for (int i = 0; i < nt - 1; ++i) {
        vt[i] = jthread(acumular, ref(vr[i]), first, last);
        first = last;
        last = next(first, range);
    }
    vt.back() = jthread(acumular, ref(vr.back()), first, end(vd));

    // Union (join) de los hilos
    for (auto& t: vt) t.join();
    auto finish = chrono::high_resolution_clock::now();

    // Calcular el total de los resultados
    auto total = accumulate(begin(vr), end(vr), static_cast<long long int>(0));
    cout << chrono::duration_cast<chrono::microseconds>(finish - start).count() << endl;
    cout << total << endl;

}

int main() {
    ejercicio_1();
    ejercicio_2();
    return 0;
}
