//Covarrubias Olivares Jose Refugio
//Gomez Jauregui Johana Guadalupe
//Gomez Ramirez Ikaro Adrian
//Hernandez Cristobal Alan

#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;


class Banco {
private:
    int saldo;
    omp_lock_t lock;

public:
    Banco(int saldoInicial) : saldo(saldoInicial) {
        omp_init_lock(&lock);
    }

    ~Banco() {
        omp_destroy_lock(&lock);
    }


    void depositar(int cantidad, int idHilo) {
        omp_set_lock(&lock);
        saldo += cantidad;
        cout << "Hilo " << idHilo << " realizo un deposito de " << cantidad << ". Saldo actual: " << saldo << endl;
        omp_unset_lock(&lock);
    }


    void retirar(int cantidad, int idHilo) {
        omp_set_lock(&lock);
        if (saldo >= cantidad) {
            saldo -= cantidad;
            cout << "Hilo " << idHilo << " retiro " << cantidad << ". Saldo actual: " << saldo << endl;
        } else {
            cout << "Hilo " << idHilo << ": Saldo insuficiente para retirar " << cantidad << ". Saldo actual: " << saldo << endl;
        }
        omp_unset_lock(&lock);
    }


    void consultarSaldo(int idHilo) {
        omp_set_lock(&lock);
        cout << "Hilo " << idHilo << " consulto el saldo. Saldo actual: " << saldo << endl;
        omp_unset_lock(&lock);
    }


    int obtenerSaldoFinal() {
        return saldo;
    }
};

int main() {
    printf("//Covarrubias Olivares Jose Refugio\n//Gomez Jauregui Johana Guadalupe\n//Gomez Ramirez Ikaro Adrian\n//Hernandez Cristobal Alan\n\n");
    srand(time(0));

    Banco banco(1000);

    int numHilos = 4;
    omp_set_num_threads(numHilos);

    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        int idHilo = omp_get_thread_num();
        for (int i = 0; i < 4; ++i) {
            int operacion = rand() % 3;
            int cantidad = rand() % 500 + 1;

            if (operacion == 0) {
                banco.depositar(cantidad, idHilo);
            } else if (operacion == 1) {
                banco.retirar(cantidad, idHilo);
            } else {
                banco.consultarSaldo(idHilo);
            }
        }
    }


    double fin = omp_get_wtime();


    cout << "Saldo final en la cuenta: " << banco.obtenerSaldoFinal() << endl;

    cout << "Tiempo total de ejecucion: " << fin - inicio << " segundos." << endl;

    printf("\n\n//Covarrubias Olivares Jose Refugio\n//Gomez Jauregui Johana Guadalupe\n//Gomez Ramirez Ikaro Adrian\n//Hernandez Cristobal Alan\n\n");

    return 0;
}
