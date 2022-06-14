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

#define NB_VISITORS 3

//VISITOR//
struct visitor
{
    int money;
    int patience;

    struct attraction* target;
};

void* visitorSoul()
{
    printf("Je suis né\n");
}

//ATTRACTTION//
struct attraction
{
    // Beware semaphore inside
};

//Generate a random number between a minimum and a maximum
int initRandom(int max, int min)
{
    return rand()%(max-min) + min;
}

//Initialize an array of visitor with random attributes values using the constants
void initVisitor(struct visitor visitors[], int n) 
{
    int i;
    srand(time(NULL));
    pthread_t id[NB_VISITORS];
    
    for(i = 0; i < n; i++)
    {
        visitors[i].money = initRandom(MONEY_MAX, MONEY_MIN);
        visitors[i].patience = initRandom(PATIENCE_MAX, PATIENCE_MIN);
        pthread_create(&id[i], NULL, visitorSoul, NULL);
    }
}

//Print all informations about a visitor
void printVisitor(struct visitor vis)
{
    printf("Money : %d\n", vis.money);
    printf("Patience : %d\n", vis.patience);
}

int main()
{
    
    //Initialization
    struct visitor visitors[NB_VISITORS];
    int i;
    
    //Waiting for a thread (example)
    //pthread_join(idOfTheThread, &valueReturnedAsPointer)
    
    initVisitor(visitors, NB_VISITORS);
    
    //Test print
    for(i = 0; i < NB_VISITORS; i++)
    {
        printVisitor(visitors[i]);
    }
    
    printf("EHEHEHAH\n");

    return 0;
}
