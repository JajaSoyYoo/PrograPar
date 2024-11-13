/*Hernandez Cristobal Alan
  Covarrubias Olivares Jose Refugio*/

#include <iostream>
#include <omp.h>
#include <stdio.h>
using namespace std;

int main()
{
    printf("Hernandez Cristobal Alan\nCovarrubias Olivares Jose Refugio\n");
    #pragma omp parallel
    {
        int hilo = omp_get_thread_num();
        #pragma omp for
        for (int i = 0 ; i < 12; i++)
        {
            double It = omp_get_wtime();

            for(int j = 0; j < 20; j++)
            {
                int arreglo[20];
                arreglo[j] = rand() % 101;
                int per = (j + 1) * 100.0 / 20;
                printf("Soy el hilo %i, puse %i y llevo %i% \n",hilo,arreglo[j],per);
            }


            double Ft = omp_get_wtime();
            double Dt = Ft - It;
            printf("Tiempo de ejecucion de hilo %i = %.4f segundos\n",hilo,Dt);
        }
    }
    printf("Hernandez Cristobal Alan\nCovarrubias Olivares Jose Refugio\n");
    return 0;
}
