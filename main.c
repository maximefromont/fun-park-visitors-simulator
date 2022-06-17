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

#define NB_ATTRACTIONS 5
#define CAPACITY_MAX 5
#define CAPACITY_MIN 1
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

void prinAttraction(attraction att) //Print all informations about an attraction
{
    printf("Name : %s\n", att.name);
    printf("Capacity : %d\n", att.capacity);
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
        int nextAttrId;
        nextAttrId = randomBetween(NB_ATTRACTIONS, 0);
        attraction nextAttr = attractions[0];
        sem_wait(&nextAttr.sem);
        printf("Visiteur %d prend l'attraction %d\n", vis->id, nextAttrId);
        sleep(5);
        sem_post(&nextAttr.sem);
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
    
    char *a[] = {"Le tournis de la mort", "La grande chute", "Aled en folie", "Liberez moi monsieur svp", "War crime simulator", "Youngling slayer 2000", "1 minute c'est 100 secondes"};
    
    int i;
    for(i = 0; i < n; i++)
    {
        attractions[i].name = a[i%7];
        attractions[i].capacity = randomBetween(CAPACITY_MAX, CAPACITY_MIN);
        sem_init(&attractions[i].sem, 0, attractions[i].capacity);
    }
}
//---------------------------------------------------------//


//------------------MAIN------------------//
int main()
{
    //Initialization
    pthread_t id[NB_VISITORS];
    int i;
    
    initVisitor(visitors, id, NB_VISITORS);
    initAttractions(attractions, NB_ATTRACTIONS);
    
    //Test print visitors
    /**
    for(i = 0; i < NB_VISITORS; i++)
    {
        printVisitor(visitors[i]);
    }**/
    
    //Test print attractions
    for(i = 0; i < NB_ATTRACTIONS; i++)
    {
        prinAttraction(attractions[i]);
    }
    
    waitVisitor(id, NB_VISITORS);

    return 0;
}
//----------------------------------------//
