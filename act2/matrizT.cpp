#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <ctime>

using namespace std;

class OperacionesMatriciales
{
private:
    int m1[120] = {0}, m2[120] = {0}, res[120] = {0}; //Se igualan a 0

public:

    //algoritmo de barajado
    void barajarMatriz(int m[]) {
    for (int i = 120 - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(m[i], m[j]);
    }
}
    void crearMatrizAleatoriaOpenMP()
    {
        #pragma omp parallel for schedule(static, 10)
        for(int cont = 0; cont < 120; cont ++)
        {

            m1[cont] = cont;

        }

        #pragma omp parallel for schedule(static, 10)
        for(int cont = 0; cont < 120; cont ++)
        {
            m2[cont] = cont;

        }
        barajarMatriz(m1);
        barajarMatriz(m2);

        for(int i = 0 ; i <120; i++)
            cout<<"[ "<<m1[i]<<"  "<<m2[i]<<" ]"<<endl;
    }

    void sumarMatricesOpenMP()
    {

        #pragma omp parallel for schedule(static, 10)
        for(int i = 0; i < 120; i ++)
        {

            res[i] = m1[i] + m2[i];

        }

        for(int i = 0; i < 120; i ++)
            printf("%i + %i = %i\n",m1[i],m2[i],res[i]);

    }

    void restarMatricesOpenMP()
    {
        #pragma omp parallel for schedule(static, 10)
        for(int i = 0; i < 120; i ++)
        {

            res[i] = m1[i] - m2[i];

        }

        for(int i = 0; i < 120; i ++)
            printf("%i - %i = %i\n",m1[i],m2[i],res[i]);

    }

    void multiplicarMatricesOpenMP()
    {
        #pragma omp parallel for schedule(static, 10)
        for(int i = 0; i < 120; i ++)
        {

            res[i] = m1[i] * m2[i];

        }

        for(int i = 0; i < 120; i ++)
            printf("%i * %i = %i\n",m1[i],m2[i],res[i]);
    }

    void transponerMatrizOpenMP()
    {
        #pragma omp parallel for schedule(static, 10)
        for(int i = 0; i < 120; i ++)
        {

            res[120 - i - 1] = m1[i]; // Inversión de índices

        }
        printf("matriz | matrizT \n");
        for(int i = 0; i < 120; i ++)
            printf("  [%i]  |   [%i] \n",m1[i],res[i]);
    }
};

int main()
{
    OperacionesMatriciales m;

    int op;

    do
    {
        cout<<"[1] Crear matriz distribuida aleatoria "<<endl;
        cout<<"[2] Sumar matrices aleatorias "<<endl;
        cout<<"[3] Restar matrices aleatorias "<<endl;
        cout<<"[4] Multiplicar matrices aleatorias"<<endl;
        cout<<"[5] Transponer matriz"<<endl;
        cout<<"[0] Salir "<<endl;
        cin>>op;

        if(op == 1)
        {
            double inicial = omp_get_wtime();
            m.crearMatrizAleatoriaOpenMP();
            double fin = omp_get_wtime();
            double dif = fin - inicial;
            printf("\nMatriz distribuida craeada en %.4f segundos\n\n",dif);
        }
        else if(op == 2)
        {
            double inicial = omp_get_wtime();
            m.sumarMatricesOpenMP();
            double fin = omp_get_wtime();
            double dif = fin - inicial;
            printf("\nMatrices distribuidas sumadas en %.4f segundos\n\n",dif);
        }
        else if(op == 3)
        {
            double inicial = omp_get_wtime();
            m.restarMatricesOpenMP();
            double fin = omp_get_wtime();
            double dif = fin - inicial;
            printf("\nMatrices distribuidas restadas en %.4f segundos\n\n",dif);
        }
        else if(op == 4)
        {
            double inicial = omp_get_wtime();
            m.multiplicarMatricesOpenMP();
            double fin = omp_get_wtime();
            double dif = fin - inicial;
            printf("\nMatrices distribuidas multiplicadas en %.4f segundos\n\n",dif);
        }
        else if(op == 5)
        {
            double inicial = omp_get_wtime();
            m.transponerMatrizOpenMP();
            double fin = omp_get_wtime();
            double dif = fin - inicial;
            printf("\nMatriz distribuida trasnpuesta en %.4f segundos\n\n",dif);
        }

    }
    while(op != 0);

    return 0;
}
