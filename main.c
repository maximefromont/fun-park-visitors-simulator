#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

//------------------CONSTANTS------------------//
#define PATIENCE_MAX 30
#define PATIENCE_MIN 10

#define MONEY_MAX 1000
#define MONEY_MIN 200

#define NB_VISITORS 10
#define NB_ATTRACTIONS 1
//---------------------------------------------//


//------------------STRUCTURES------------------//
typedef struct
{
    int id;
    int money;
    int patience;

    struct attraction* target;
}visitor;

typedef struct
{
    char* name;
    int capacity;  // Max number of visitors in the attraction
    sem_t sem;
}attraction;
//----------------------------------------------//


//------------------SUB FUNCTIONS------------------//
int randomBetween(int max, int min) //Generate a random number between a minimum and a maximum
{
    return rand()%(max-min) + min;
}

void printVisitor(visitor vis) //Print all informations about a visitor
{
    printf("Money : %d\n", vis.money);
    printf("Patience : %d\n", vis.patience);
}
//------------------------------------------------//


//------------------GLOBAL INITIALIZATION------------------//
visitor visitors[NB_VISITORS];
attraction attractions[NB_ATTRACTIONS];
//---------------------------------------------------------//


//------------------VISITOR FUNCTIONS------------------//
void* visitorSoul(void *arg)
{
    visitor* vis = (visitor*) arg;
    printf("Birth of visitor %d\n", vis->id);

    while(1)
    {
        sem_wait(&attractions[0].sem);
        printf("Visiteur %d prend l'attraction\n", vis->id);
        sleep(10);
        sem_post(&attractions[0].sem);
    }
    
   return 0;
}

//Initialize an array of visitor with random attributes values using the constants
void initVisitor(visitor visitors[], pthread_t id[], int n) 
{
    int i;
    srand(time(NULL));
    
    for(i = 0; i < n; i++)
    {
        visitors[i].id = i;
        visitors[i].money = randomBetween(MONEY_MAX, MONEY_MIN);
        visitors[i].patience = randomBetween(PATIENCE_MAX, PATIENCE_MIN);
        pthread_create(&id[i], NULL, visitorSoul, &visitors[i]);
    }
}

//Wait for all visitors to end their routine
void waitVisitor(pthread_t id[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        pthread_join(id[i], NULL);
    }
}
//-----------------------------------------------------//


//------------------ATTRACTIONS FUNCTIONS------------------//
void initAttractions(attraction attractions[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        sem_init(&attractions[i].sem, 0, attractions[i].capacity);
    }
}
//---------------------------------------------------------//


//------------------MAIN------------------//
int main()
{
    //Initialization
    pthread_t id[NB_VISITORS];
    
    attractions[0].capacity = 1;

 
    //Arguments : semaphore, 0 to be shared with threads, valeur
    sem_init(&attractions[0].sem, 0, attractions[0].capacity);
    

    initVisitor(visitors, id, NB_VISITORS); //Probleme dans là dedans apparament
    
    //Test print
    /**
    int i;
    for(i = 0; i < NB_VISITORS; i++)
    {
        printVisitor(visitors[i]);
    }**/
    
    waitVisitor(id, NB_VISITORS);

    return 0;
}
//----------------------------------------//
