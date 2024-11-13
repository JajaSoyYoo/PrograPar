// Covarrubias Olivares Jose Refugio
// Gomez Jauregui Johana Guadalupe
// Gomez Ramirez Ikaro Adrian
// Hernandez Cristobal Alan

#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class OperacionesMatriciales
{
private:
    vector<int> numeros;

public:
    void barajarVector() {
        for (int i = numeros.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(numeros[i], numeros[j]);
        }
    }

    void crearArreglo(int total) {
        numeros.resize(total);
        for (int cont = 0; cont < total; cont++) {
            numeros[cont] = cont + 1; //Se llena del 1 al 1000 y luego se baraja
        }
        barajarVector();
    }

    vector<int>& getNumeros() {
        return numeros;
    }

    void setNumeros(const vector<int>& nuevoNumeros) {
        numeros = nuevoNumeros;  // Copia el nuevo vector
    }

    int sumarVector() {
        int resultado = 0;
        for (int num : numeros) {
            resultado += num;
        }
        return resultado;
    }

    float promedioVector(int total) {
        return static_cast<float>(sumarVector()) / total;
    }

    int maximo() {
        int max_val = numeros[0];
        for (int num : numeros) {
            if (num > max_val) {
                max_val = num;
            }
        }
        return max_val;
    }

    int minimo() {
        int min_val = numeros[0];
        for (int num : numeros) {
            if (num < min_val) {
                min_val = num;
            }
        }
        return min_val;
    }

    void imprimirVector() {
        for (int num : numeros) {
            cout << "[ " << num << " ]";
        }
        cout << endl;
    }
};

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total = 1000;
    OperacionesMatriciales m;

    if (rank == 0) {
        cout << "//Covarrubias Olivares Jose Refugio\n"
             << "//Gomez Jauregui Johana Guadalupe\n"
             << "//Gomez Ramirez Ikaro Adrian\n"
             << "//Hernandez Cristobal Alan\n" << endl;

        m.crearArreglo(total);
    }

    // Distribuir el tamaño del vector
    MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso crea su propio vector
    m.crearArreglo(total);

    // Distribuir el trabajo
    int elements_per_process = total / size;
    vector<int> subArray(elements_per_process);

    // Enviar a cada proceso su parte del arreglo
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(m.getNumeros().data() + i * elements_per_process,
                     elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // El proceso 0 maneja su propia parte
        for (int i = 0; i < elements_per_process; i++) {
            subArray[i] = m.getNumeros()[i];
        }
    } else {
        MPI_Recv(subArray.data(), elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Reemplazar subArray en el objeto OperacionesMatriciales
    m.setNumeros(subArray);

    int op;
    do {
        if (rank == 0) {
            cout << "[1] Crear vector aleatorio " << endl;
            cout << "[2] Sumar contenido de vector " << endl;
            cout << "[3] Promedio de vector " << endl;
            cout << "[4] Maximo de vector" << endl;
            cout << "[5] Minimo de vector" << endl;
            cout << "[0] Salir " << endl;
            cin >> op;

            if (op == 1) {
                double inicial = MPI_Wtime();
                m.crearArreglo(total);
                cout << "Vector creado: ";
                m.imprimirVector();
                double fin = MPI_Wtime();
                double dif = fin - inicial;
                printf("\nMatriz distribuida creada en %.4f segundos\n\n", dif);
                // Enviar el nuevo vector a otros procesos
                for (int i = 1; i < size; i++) {
                    MPI_Send(m.getNumeros().data(), total, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }
        }

        // Sincronizar procesos antes de ejecutar
        MPI_Bcast(&op, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Recibir el vector si es necesario
        if (op == 1 && rank != 0) {
            MPI_Recv(m.getNumeros().data(), total, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Calcular suma, promedio, máximo y mínimo
        if (op == 2) {
            double inicial = MPI_Wtime();
            int local_sum = m.sumarVector();
            int total_sum = 0;
            MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

            if (rank == 0) {
                printf("\n\nSuma de todos los elementos del vector = %i\n", total_sum);
                double fin = MPI_Wtime();
                double dif = fin - inicial;
                printf("Tiempo para sumar: %.4f segundos\n", dif);
            }
        } else if (op == 3) {
            double inicial = MPI_Wtime();
            float local_promedio = m.promedioVector(total);
            float total_promedio;
            MPI_Reduce(&local_promedio, &total_promedio, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

            if (rank == 0) {
                total_promedio /= size; // Promedio total
                printf("\n\nPromedio de todos los elementos del vector = %.2f\n", total_promedio);
                double fin = MPI_Wtime();
                printf("Tiempo para calcular promedio: %.4f segundos\n", fin - inicial);
            }
        } else if (op == 4) {
            double inicial = MPI_Wtime();
            int local_max = m.maximo();
            int global_max;
            MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

            if (rank == 0) {
                printf("\n\nMaximo de todos los elementos del vector = %i\n", global_max);
                double fin = MPI_Wtime();
                printf("Tiempo para calcular maximo: %.4f segundos\n", fin - inicial);
            }
        } else if (op == 5) {
            double inicial = MPI_Wtime();
            int local_min = m.minimo();
            int global_min;
            MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

            if (rank == 0) {
                printf("\n\nMinimo de todos los elementos del vector = %i\n", global_min);
                double fin = MPI_Wtime();
                printf("Tiempo para calcular minimo: %.4f segundos\n", fin - inicial);
            }
        }

    } while (op != 0);

    MPI_Finalize();
    return 0;
}
