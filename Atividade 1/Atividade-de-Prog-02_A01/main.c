#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int Critical_Section = 10;

int main (int argc, char *argv[]) {

 int request = 0, respond = 0;


    omp_set_num_threads(4);
    int myid;



#pragma omp parallel private(myid) shared(request, respond, Critical_Section)
    {

                myid = omp_get_thread_num();
				if ( myid == 0 ){// então esta é a thread servidor

                    while (1){
						printf("\n##############################################################################\n");
                        printf("Servidor aguardando cliente...\n");
						printf("\n##############################################################################\n");
                        while (request == 0){}
						printf("Servidor respondendo cliente...\n");
						printf("\n##############################################################################\n");
                        respond = request;
                        while (respond != 0){}
                        request = 0;
                        if (Critical_Section == 0) exit(1);
                    }
				}

                else {
                        while (1){
						printf("\n##############################################################################\n");
                        printf("Pré protocolo:  Solicitando acesso a secao critica\n");
						printf ("			* Thread ID: %d\n", myid);
						printf ("			* Regiao Critica (valor): %d\n", Critical_Section);
						printf("\n##############################################################################\n");
                        sleep(1.5);
                        while (respond != myid) request = myid;
                        //entrou na seção critica
                        printf("Acesso na Secao Critica:  Realizando alteracao na secao critica\n");
						printf ("			* Thread ID: %d\n", myid);
						printf ("			* Regiao Critica (valor antigo): %d\n", Critical_Section);
                        sleep(1.5);
                        Critical_Section--;
						printf("\n##############################################################################\n");
						printf ("			* Thread ID: %d\n", myid);
						printf ("			* Regiao Critica (valor novo): %d\n", Critical_Section);
                        printf("			* Abandonando secao critica\n");
						printf("\n##############################################################################\n");
                        sleep(1.5);
                        //saindo da seção critica
                        respond = 0;
                        //saiu da seção critica

                        }
                    }
    }


    return 0;
 }

