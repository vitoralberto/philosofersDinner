#include<stdio.h>
#define n 5

int compltedPhilo = 0,i;

struct fork{
    int taken;
}ForkAvil[5];

struct philosp{
    int left;
    int right;
}Philostatus[5];

void goForDinner(int philID){ //threads
    if(Philostatus[philID].left==1 && Philostatus[philID].right==10)
        printf("Filosofo %d terminou de jantar\n",philID+1);
    //se o baby terminou de jantar
    else if(Philostatus[philID].left==1 && Philostatus[philID].right==1){
            //se pegou apenas dois garfos
            printf("Filosofo %d terminou de jantar\n",philID+1);

            Philostatus[philID].left = Philostatus[philID].right = 10; //contador que lembra quem ja comeu
            int otherFork = philID-1;

            if(otherFork== -1)
                otherFork=(n-1);

                //funcao que libera os garfos na mesa
            ForkAvil[philID].taken = ForkAvil[otherFork].taken = 0; //libera os garfos
            printf("Filosofo %d deixou o garfo %d e o garfo %d\n",philID+1,philID+1,otherFork+1);
            compltedPhilo++;
        }

        else if(Philostatus[philID].left==1 && Philostatus[philID].right==0){ //tenta pegar o garfo direito
                if(philID==(n-1)){
                    if(ForkAvil[philID].taken==0){ //o ultimo filosofo pode tentar acessar o garfo pela ordem reversa
                        ForkAvil[philID].taken = Philostatus[philID].right = 1;
                        printf("Garfo %d pego pelo filosofo %d\n",philID+1,philID+1);
                    }
                    else{
                        printf("Filosofo %d espera pelo garfo %d\n",philID+1,philID+1);
                    }
                }
                else{ //ultimo filosofo
                    int dupphilID = philID;
                    philID-=1;

                    if(philID== -1)
                        philID=(n-1);

                    if(ForkAvil[philID].taken == 0){
                        ForkAvil[philID].taken = Philostatus[dupphilID].right = 1;
                        printf("Garfo %d pego pelo filosofo %d\n",philID+1,dupphilID+1);
                    }
                    else{
                        printf("Filosofo %d espera pelo garfo %d\n",dupphilID+1,philID+1);
                    }
                }
            }
            else if(Philostatus[philID].left==0){ //vai verificar se tem o garfo esquerdo
                    if(philID==(n-1)){
                        if(ForkAvil[philID-1].taken==0){
                            ForkAvil[philID-1].taken = Philostatus[philID].left = 1;
                            printf("Garfo %d pego pelo filosofo %d\n",philID,philID+1);
                        }
                        else{
                            printf("Filosofo %d espera pelo garfo %d\n",philID+1,philID);
                        }
                    }
                    else{ //except last philosopher case
                        if(ForkAvil[philID].taken == 0){
                            ForkAvil[philID].taken = Philostatus[philID].left = 1;
                            printf("Garfo %d pego pelo filosofo %d\n",philID+1,philID+1);
                        }
                        else{
                            printf("Filosofo %d espera pelo garfo %d\n",philID+1,philID+1);
                        }
                    }
        }
}

int main(){
    for(i=0;i<n;i++)
        ForkAvil[i].taken=Philostatus[i].left=Philostatus[i].right=0;

    while(compltedPhilo<n){
        /* Sera executado o laço até que todos tenham completado o jantar
        */
        for(i=0;i<n;i++)
            goForDinner(i);
        printf("\nNumero de Filosofos que ja comeram %d\n\n",compltedPhilo);
    }

    return 0;
}
