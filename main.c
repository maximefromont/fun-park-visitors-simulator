#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

//CONSTANTS//
#define PATIENCE_MAX 30
#define PATIENCE_MIN 10

#define MONEY_MAX 1000
#define MONEY_MIN 200

#define NB_VISITORS 10
#define NB_ATTRACTIONS 1

//VISITOR//
struct visitor
{
    int id;
    int money;
    int patience;

    struct attraction* target;
};

//ATTRACTTION//
struct attraction
{
    int capacity;  // Number max of visitors in the attraction
    sem_t sem;
};

void* visitorSoul(void *arg)
{
    int* id;
    id = arg;
    printf("Birth of visitor %d\n", *id);

    
    while(1)
    {
        sem_wait(&attractions[0].sem);
        printf("Visiteur %d prend l'attraction\n", *id);
        sleep(10);
        sem_post(&attractions[0].sem);
    }
   return 0;
}

//Generate a random number between a minimum and a maximum
int initRandom(int max, int min)
{
    return rand()%(max-min) + min;
}

//Initialize an array of visitor with random attributes values using the constants
void initVisitor(struct visitor visitors[], pthread_t id[], int n) 
{
    int i;
    srand(time(NULL));
    
    for(i = 0; i < n; i++)
    {
        visitors[i].id = i;
        visitors[i].money = initRandom(MONEY_MAX, MONEY_MIN);
        visitors[i].patience = initRandom(PATIENCE_MAX, PATIENCE_MIN);
        pthread_create(&id[i], NULL, visitorSoul, (void *) i);
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

void initAttractions(struct attraction attractions[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        sem_init(&attractions[i].sem, 0, attractions[i].capacity);
    }
}

//Print all informations about a visitor
void printVisitor(struct visitor vis)
{
    printf("Money : %d\n", vis.money);
    printf("Patience : %d\n", vis.patience);
}

//Initialization
struct visitor visitors[NB_VISITORS];
struct attraction attractions[NB_ATTRACTIONS];


int main()
{
    attractions[0].capacity = 1;
    sem_init(attractions[0].sem, 0, attractions[0].capacity);
    
    pthread_t id[NB_VISITORS];
    initVisitor(visitors, id, NB_VISITORS);
    
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
