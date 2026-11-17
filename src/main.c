#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char randChar(){

    char* chrLst = " abcdefghijklmnopqrstuvwxyz"; 

    int lwrBnd = 0;
    int uprBnd = 26;

    int value = rand() % (uprBnd - lwrBnd + 1)+ lwrBnd;

    return chrLst[value];
}

int getRandomValue(int min, int max){
    return rand() % (max-min+1)+min;
}

int* calcFitness(char* subject, char** population, int populationCount, int subLgnt){
    int* fitness = malloc(sizeof(int)*populationCount);
    for(int i=0; i<populationCount; ++i){
        int score = 0;
        for(int j=0; j<subLgnt; ++j){
            if(population[i][j] == subject[j])
                score++;
        }
        fitness[i] = score;
    }
    return fitness;
}

int main(){
    srand ( time(NULL) );
    
    char* subject = "the entire works of shakespeare loll";
    int subLgnt = strlen(subject);
    size_t populationCount = 200;
    char** population = malloc(sizeof(char*) * populationCount);
    for (size_t i = 0; i < populationCount; ++i) {
        population[i] = malloc((subLgnt + 1) * sizeof(char));
        for (int j = 0; j < subLgnt; ++j)
            population[i][j] = randChar();
        population[i][subLgnt] = '\0';
    }

    int gen = 0;

    while(1){
    
        int* fitness = malloc(sizeof(int)*populationCount);
        fitness = calcFitness(subject, population, populationCount, subLgnt);

        size_t bestIdx = 0;
        for(size_t i = 1; i < populationCount; ++i){
            if(fitness[i] > fitness[bestIdx])
                bestIdx = i;
        }
        char* bestCand = population[bestIdx];
        printf("Best Candidate of Generation %i: %s\n", gen, bestCand);
        
        if(fitness[bestIdx]==subLgnt) 
            break;
        gen++;

        //CONTESTS
        int winners[populationCount];

        for(int i=0; i<populationCount; ++i){
            int c1 = getRandomValue(0, populationCount - 1);;
            while(fitness[c1]==0)
                c1 = getRandomValue(0, populationCount-1);
            int c2 = getRandomValue(0, populationCount - 1);;
            while(fitness[c2]==0)
                c2 = getRandomValue(0, populationCount-1);
            if(fitness[c1]>=fitness[c2]) winners[i] = c1;
            else winners[i] = c2;
        }


        //REPRODUCTION
        char** nextGen = malloc(sizeof(char*) * populationCount);
        nextGen[0] = malloc((subLgnt + 1) * sizeof(char));
        strcpy(nextGen[0], bestCand);

        for(size_t i = 1; i < populationCount; ++i){
            char* offspring = malloc((subLgnt + 1) * sizeof(char));
            int offints[subLgnt];

            int p1 = getRandomValue(0, populationCount - 1);
            int p2 = getRandomValue(0, populationCount - 1);

            int zC = 0;
            int oC = 0;
            for(int j = 0; j < subLgnt; ++j){
                offints[j] = getRandomValue(0, 1);
                if(offints[j] == 0) zC++;
                else oC++;
            }
            if(zC >= oC){
                for(int k = 0; k < subLgnt; ++k){
                    if(p1 >= 2){
                        if(offints[k] == 0)
                            offspring[k] = population[winners[p1]][k];
                        else
                            offspring[k] = population[winners[p2]][k];
                    }
                    else{
                        if(offints[k] == 0)
                            offspring[k] = population[winners[p2]][k];
                        else
                            offspring[k] = population[winners[p1]][k];
                    }

                    if(getRandomValue(0, 100) <= 1)
                        offspring[k] = randChar();
                }
            } else {
                for(int k = 0; k < subLgnt; ++k){
                    if(p1 >= 2){
                        if(offints[k] == 1)
                            offspring[k] = population[winners[p1]][k];
                        else
                            offspring[k] = population[winners[p2]][k];
                    }
                    else{
                        if(offints[k] == 1)
                            offspring[k] = population[winners[p2]][k];
                        else
                            offspring[k] = population[winners[p1]][k];
                    }

                    if(getRandomValue(0, 100) <= 1)
                        offspring[k] = randChar();
                }
            }

            offspring[subLgnt] = '\0';
            nextGen[i] = offspring;
        } 
        for (size_t i = 0; i < populationCount; ++i) {
            free(population[i]);
            population[i] = nextGen[i];
        }
        free(nextGen);
        free(fitness);
    }

    return 0;
}
