#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <iostream>

#define MAX_GENERATION 100  // Number of times the loop will run
#define POP_SIZE 100 // Size of population
#define CROSSOVER_RATE 0.8 // Probability of CrossOver (typically near 1)
#define MUTATION_RATE 0.3 //Probability of mutation (typically <.1)
#define CHROMOSOME_LENGTH  10 // Length of the chromosome, number of city

//10 cities with given data
double xyAxis[CHROMOSOME_LENGTH][2] = { {0.3642,0.7770},{0.7185,0.8312},{0.0986,0.5891},{0.2954,0.9606},{0.5951,0.4647},{0.6697 ,0.7657},{0.4353,0.1709},{0.2131,0.8349},{0.3479,0.6984},{0.4516,0.0488}};

//22 cities with 12 extra data
//double xyAxis[CHROMOSOME_LENGTH][2] = { {0.3642,0.7770},{0.7185,0.8312},{0.0986,0.5891},{0.2954,0.9606},{0.5951,0.4647},{0.6697 ,0.7657},{0.4353,0.1709},{0.2131,0.8349},{0.3479,0.6984},{0.4516,0.0488} ,{0.8811,0.9511},{0.1234,0.1234},{0.8765,0.1234},{0.9123,0.3456},{0.9981,0.5000},{0.1234,0.3456},{0.6891,0.3456},{0.7191,0.0534},{0.0534,0.8876},{0.3456,0.4567},{0.8765,0.6789},{0.6697,0.9388}};

//Extend investigation --- China TSP 34 cities
//double xyAxis[CHROMOSOME_LENGTH][2] ={{4386,570},{4312,790},{4196,1044},{3639,1315},{3712,1399},{3238,1229},{2788,1491},{1332,695},{3326,1556},{3488,1535},{2381,1676},{3715,1678},{2562,1756},{3429,1908},{3077,1970},{3780,2212},{3918,2179},{4177,2244},{2545,2357},{3507,2376},{4061,2370},{2769,2492},{1304,2312},{3394,2643},{3676,2578},{2778,2826},{4029,2838},{2370,2975},{2935,3240},{3439,3201},{3538,3298},{3470,3304},{4263,2931},{3140,3557}};

//For testig, burma14 question
//double xyAxis[CHROMOSOME_LENGTH][2] = {{16.47,96.10},{16.47,94.44},{20.09,92.54},{22.39,93.37},{25.23,97.24},{22.00,96.05},{20.47,97.02},{17.20,96.29},{16.30,97.38},{14.05,98.12},{16.53,97.38},{21.52,95.59},{19.41,97.13},{20.09,94.55}};

int chromosome[POP_SIZE][CHROMOSOME_LENGTH ]; // group
int elitism[POP_SIZE][CHROMOSOME_LENGTH ]; // store the best 10% generation of previous
int bestDistance[CHROMOSOME_LENGTH ]; // best Distance order
double minDistance; // shortest Distance length
int replace = -1;

// function declaration
void Initialize(void); // Initialize
void Selection(int[POP_SIZE][CHROMOSOME_LENGTH ]); // Selection process
void CrossOver(int[POP_SIZE][CHROMOSOME_LENGTH ]); // CrossOver process
void Mutation(int[POP_SIZE][CHROMOSOME_LENGTH ]); // mutation process
void Reverse(int[POP_SIZE][CHROMOSOME_LENGTH ]); // reserve elite indifivudals within generated range, and replace the previous if reversed one is better
double PathLength(int *); // calculate centrain path length
double Distance_Compare(double *, double *); // Distance between two citys
double * minValue(double *); // minimun Distance between xyAxis elemtnes


// Initialisation
void Initialize(void){
    for (int i = 0; i < POP_SIZE; i++){
        for (int j = 0; j < CHROMOSOME_LENGTH ; j++){
            chromosome[i][j] = j + 1; //assign a unique ID to each gene
        }
    }
    
    for(int i=0;i<POP_SIZE;i++){
        for(int j=0;j<CHROMOSOME_LENGTH;j++){
            int pos1 = (int)(CHROMOSOME_LENGTH *(((double)rand()) / (RAND_MAX + 1.0))); // position 1
            int pos2 = (int)(CHROMOSOME_LENGTH *(((double)rand()) / (RAND_MAX + 1.0))); // position 2
            std::swap(chromosome[i][pos1], chromosome[i][pos2]); //swap the gene randomly
        }
    }
}


// Selection
void Selection(int chromosome[POP_SIZE][CHROMOSOME_LENGTH ]){
    double pick, sumOfFit = 0;
    int worestFitness = 1;
    int worestIndex = -1;
    double choice[POP_SIZE][CHROMOSOME_LENGTH ];
    double fittnessProbi[POP_SIZE];
    double fit[POP_SIZE]; // fittness
    
    for (int j = 0; j < POP_SIZE; j++){
        double path = PathLength(chromosome[j]);
        fit[j] = 1 / path;
        if (fit[j]<worestFitness){ //calculate the worest chromosome
            worestIndex = j;
            worestFitness = fit[j];
        }
        sumOfFit += fit[j];
    }
    //-----10 cities elitism Start-------
    if (CHROMOSOME_LENGTH==10 && replace==1){
        for(int j=0;j<CHROMOSOME_LENGTH;j++)
            chromosome[worestIndex][j]=elitism[0][j];
    }
    //-----10 cities elitism End-------
    
    for (int j = 0; j < POP_SIZE; j++){
        fittnessProbi[j] = fit[j] / sumOfFit;
    }
    // random selection start
    for (int i = 0; i < POP_SIZE; i++){
        pick = ((double)rand()) / RAND_MAX; // random from 0 to 1
        for (int j = 0; j < POP_SIZE; j++){
            pick -= fittnessProbi[j];
            if (pick<=0){
                for (int k = 0; k < CHROMOSOME_LENGTH ; k++){
                    choice[i][k] = chromosome[j][k]; // choose one chromosome
                }
                break;
            }
        }
    }
    for (int i = 0; i < POP_SIZE; i++){
        for (int j = 0; j < CHROMOSOME_LENGTH ; j++){
            chromosome[i][j] = choice[i][j]; //replace the chromosome according to the fitness selection
        }
    }
}

//CrossOver
void CrossOver(int chromosome[POP_SIZE][CHROMOSOME_LENGTH ]){
    double pick,pick1, pick2;
    int parent1, parent2,pos1, pos2,temp,counter1, counter2,index1, index2;
    int conflict1[CHROMOSOME_LENGTH ];
    int conflict2[CHROMOSOME_LENGTH ];// confilct postion
    int move = 0; // current moving position
    
    while (move < CHROMOSOME_LENGTH  - 1){
        pick = ((double)rand()) / RAND_MAX; // decide to CrossOver or not
        if (pick > CROSSOVER_RATE){
            move += 2; //compare between 2 partents
            continue; //no CrossOver
        }
        
        parent1 = move;
        parent2 = move + 1;  // choose two partant
        pick1 = ((double)rand()) / (RAND_MAX + 1.0);
        pick2 = ((double)rand()) / (RAND_MAX + 1.0);
        pos1 = (int)(pick1*CHROMOSOME_LENGTH ); //locate the postion of two partant
        pos2 = (int)(pick2*CHROMOSOME_LENGTH );
        
        while (pos1 > CHROMOSOME_LENGTH  - 2 || pos1 < 1){ //prevent the generated position is not valid
            pick1 = ((double)rand()) / (RAND_MAX + 1.0);
            pos1 = (int)(pick1*CHROMOSOME_LENGTH );
        }
        while (pos2 > CHROMOSOME_LENGTH  - 2 || pos2 < 1){
            pick2 = ((double)rand()) / (RAND_MAX + 1.0);
            pos2 = (int)(pick2*CHROMOSOME_LENGTH );
        }
        
        if (pos1 > pos2){ //Since pos1 means start point, so need to swap with pos2 if pos1 is larger than pos2
            temp = pos1;
            pos1 = pos2;
            pos2 = temp; // swap pos1 and pos2
        }
        
        for (int j = pos1; j <= pos2; j++){ //start crossover within position 1 to position 2
            temp = chromosome[parent1][j];
            chromosome[parent1][j] = chromosome[parent2][j];
            chromosome[parent2][j] = temp; // exchange the order
        }
        counter1 = 0;
        counter2 = 0;
        if (pos1 > 0 && pos2 < CHROMOSOME_LENGTH  - 1){ //find each chromsome that have repeated ID (conflict)
            for (int j = 0; j <= pos1 - 1; j++){
                for (int k = pos1; k <= pos2; k++){
                    if (chromosome[parent1][j] == chromosome[parent1][k]){ //find out which position is conflict and store in conflic1,2 array
                        conflict1[counter1] = j;
                        counter1++;
                    }
                    if (chromosome[parent2][j] == chromosome[parent2][k]){
                        conflict2[counter2] = j;
                        counter2++;
                    }
                }
            }
            for (int j = pos2 + 1; j < CHROMOSOME_LENGTH ; j++){ //aslo used to find out the position of conflic , but the start postion changed
                for (int k = pos1; k <= pos2; k++){
                    if (chromosome[parent1][j] == chromosome[parent1][k]){
                        conflict1[counter1] = j;
                        counter1++;
                    }
                    if (chromosome[parent2][j] == chromosome[parent2][k]) {
                        conflict2[counter2] = j;
                        counter2++;
                    }
                }
                
            }
        }
        if ((counter1 == counter2) && counter1 > 0){ //the nubmer of conflict of two parents should be the same
            for (int j = 0; j < counter1; j++){
                index1 = conflict1[j];
                index2 = conflict2[j];
                temp = chromosome[parent1][index1]; // swap the position of conflict between partent 1 and parent 2
                chromosome[parent1][index1] = chromosome[parent2][index2]; //The first conflict of partent 1 swap with the first conflict of parent 2
                chromosome[parent2][index2] = temp;
            }
        }
        move += 2;
    }
}

//random two positions and swap them
void Mutation(int chromosome[POP_SIZE][CHROMOSOME_LENGTH ]){
    int pos1, pos2;
    for (int i = 0; i < POP_SIZE; i++){
        if ((((double)rand()) / RAND_MAX) > MUTATION_RATE) //decide to mutation or not
            continue;

        pos1 = (int)((((double)rand()) / (RAND_MAX + 1.0))*CHROMOSOME_LENGTH ); //locate the position for mutation
        pos2 = (int)((((double)rand()) / (RAND_MAX + 1.0))*CHROMOSOME_LENGTH );
        std::swap(chromosome[i][pos1],chromosome[i][pos2]);
    }
}

void Reverse(int chromosome[POP_SIZE][CHROMOSOME_LENGTH ]){
    double distance, reverseDistance;
    int noOfReverse,flag, pos1, pos2, temp;
    int reverseArray[CHROMOSOME_LENGTH ];
    
    for (int i = 0; i < POP_SIZE; i++){
        flag = 0; // indicate whether this reverse is ok or not
        while (flag == 0){
            pos1 = (int)((((double)rand()) / (RAND_MAX + 1.0))*CHROMOSOME_LENGTH ); // choose the position to reverse
            pos2 = (int)((((double)rand()) / (RAND_MAX + 1.0))*CHROMOSOME_LENGTH );
            if (pos1 > pos2){ //Since pos1 means start point, so need to swap with pos2 if pos1 is larger than pos2
                temp = pos1;
                pos1 = pos2;
                pos2 = temp;
            }
            if (pos1 < pos2){ //make sure position 1 is smaller than postion 2
                for (int j = 0; j < CHROMOSOME_LENGTH ; j++)
                    reverseArray[j] = chromosome[i][j];
                noOfReverse = 0;
                for (int j = pos1; j <= pos2; j++){
                    reverseArray[j] = chromosome[i][pos2 - noOfReverse];
                    noOfReverse++;
                }
                reverseDistance = PathLength(reverseArray); // the Distance after reverse
                distance = PathLength(chromosome[i]); // original Distance
                if (reverseDistance < distance){
                    for (int j = 0; j < CHROMOSOME_LENGTH ; j++)
                        chromosome[i][j] = reverseArray[j]; // update the chromosome
                }
            }
            flag = 1; //this reverse is ok
        }
        
    }
}

// calculate the path length
double PathLength(int * j){
    double pathLength = 0; //store total path length
    for (int i = 0; i < CHROMOSOME_LENGTH  - 1; i++){
        double distance = Distance_Compare(xyAxis[*(j + i) - 1], xyAxis[*(j + i + 1) - 1]);
        //calculate the path length between j+i city and j+i+1 city
        pathLength += distance;
    }
    //calculate the path length between first and last city
    double distance = Distance_Compare(xyAxis[*(j + CHROMOSOME_LENGTH  - 1) - 1], xyAxis[*j - 1]);
    pathLength += distance;
    return pathLength; // total path length
}

double Distance_Compare(double * city1, double * city2){
    return sqrt((*city1 - *city2)*(*city1 - *city2) + (*(city1 + 1) - *(city2 + 1))*(*(city1 + 1) - *(city2 + 1)));
}

double * minValue(double * distanceArray){
    static double bestIndex[2];
    double minDistance = *distanceArray;
    double minIndex = 0;
    for (int i = 1; i < POP_SIZE; i++){
        double distance = *(distanceArray + i);
        if (distance < minDistance){
            minDistance = distance;
            minIndex = i;
        }
    }
    bestIndex[0] = minIndex;
    bestIndex[1] = minDistance;
    return bestIndex;
}

int main(){
    
    printf("-----------------------------TSP by genetic algorithm-----------------------------\n");
    printf("No of City: %d\nPopulation No: %d\nGeneration No: %d\nCrossOver Rate: %.2f\nMutatiopn Rate: %.2f\n", CHROMOSOME_LENGTH, POP_SIZE, MAX_GENERATION,CROSSOVER_RATE,MUTATION_RATE);

    time_t startTime, finishTime;
    startTime = clock(); // start counting the time
    srand((unsigned)time(NULL));
    Initialize(); //
    
    int highFitnessIndex = 0;
    double distanceArray[POP_SIZE];
    
    for (int j = 0; j < POP_SIZE; j++){
        distanceArray[j] = PathLength(chromosome[j]);
    }
    
    double * bestIndex = minValue(distanceArray); // calculate the shortest Distance and index
    minDistance = *(bestIndex + 1);
    int index = (int)(*bestIndex);
    for (int j = 0; j < CHROMOSOME_LENGTH ; j++)
        bestDistance[j] = chromosome[index][j];
    
    double counter = 0;
    double * newIndex;
    for (int i = 0; i < MAX_GENERATION; i++){
        Selection(chromosome);
        CrossOver(chromosome);
        Mutation(chromosome);
        Reverse(chromosome);
        for (int j = 0; j < POP_SIZE; j++)
            distanceArray[j] = PathLength(chromosome[j]);
        
        newIndex = minValue(distanceArray);
        if (*(newIndex + 1) < minDistance){
            minDistance = *(newIndex + 1);
            for (int j = 0; j < CHROMOSOME_LENGTH ; j++){
                bestDistance[j] = chromosome[(int)(*newIndex)][j];
                //-----10 cities elitism Start-------
                if (CHROMOSOME_LENGTH==10){
                    elitism[0][j] = chromosome[(int)(*newIndex)][j];
                    replace = 1;
                }
                //-----10 cities elitism End-------
            }
            highFitnessIndex = i + 1;
        }
        
        //-----22 cities elitism Start-------
        if (CHROMOSOME_LENGTH==22){
            //find out the best two
            int elitism_value[2] = {10};
            int elitism_index[2] = {10};
            for(int k=0;k<POP_SIZE;k++){
                if(PathLength(chromosome[k])<elitism_value[0]){
                    elitism_index[1] = elitism_index[0];
                    elitism_value[1] = elitism_value[0];
                    elitism_index[0] = k;
                    elitism_value[0] =PathLength(chromosome[k]);
                }
            }
            //find out the worest two
            int worest_value[2] = {0};
            int worest_index[2] = {0};
            for(int k=0;k<POP_SIZE;k++){
                if(PathLength(chromosome[k])>worest_value[0]){
                    worest_index[1] = worest_index[0];
                    worest_value[1] = worest_value[0];
                    worest_index[0] = k;
                    worest_value[0] =PathLength(chromosome[k]);
                }
            }
        
            for(int k=0;k<CHROMOSOME_LENGTH;k++){
                chromosome[worest_index[0]][k] = chromosome[elitism_index[0]][k];
                chromosome[worest_index[1]][k]  = chromosome[elitism_index[1]][k];
            }
        }
        //-----22 cities elitism End-------
        
        //--------Testing----------
        //printf("The shortest distance of %d generation is: %f\n",i+1,minDistance);
        //printf("%f\n",minDistance);
        //--------Testing----------
        counter+=minDistance;
    }
    
    finishTime = clock(); // end time
    double duration = ((double)(finishTime - startTime)) / CLOCKS_PER_SEC;
    
    printf("---------------------------------Analysis result ---------------------------------\n");
    printf("Shortest Distance: \n");
    
    for (int i = 0; i < CHROMOSOME_LENGTH ; i++)
            printf("%d --> ", bestDistance[i]);
    printf("%d ", bestDistance[0]);
    
    printf("\nShortest Distance :%lf, find it in %dth generation.\n", minDistance, highFitnessIndex);
    printf("Time need: %lf second.\n", duration);
    printf("Average shortest distance: %lf\n", counter/double(MAX_GENERATION));
    printf("---------------------------------Analysis result ---------------------------------\n");
    return 0;
}

