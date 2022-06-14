#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define PATIENCE_MAX 30
#define PATIENCE_MIN 10

#define MONEY_MAX 1000
#define MONEY_MIN 200

struct visitor
{
    int money;
    int patience;

    struct attraction* target;
};

struct attraction
{
    // Beware semaphore inside
};

int initRandom(int max, int min)
{
    return rand()%(max-min) + min;
}

void initVisitor(struct visitor visitors[], int n) 
{
    int i;
    srand(time(NULL));
    
    for(i = 0; i < n; i++)
    {
        visitors[i].money = initRandom(MONEY_MAX, MONEY_MIN);
        visitors[i].patience = initRandom(PATIENCE_MAX, PATIENCE_MIN);
    }
}

void printVisitor(struct visitor vis)
{
    printf("Money : %d\n", vis.money);
    printf("Patience : %d\n", vis.patience);
}

int main()
{
    
    //Initialization
    int nbVisitors = 20;
    struct visitor visitors[nbVisitors];
    int i;
    
    initVisitor(visitors, nbVisitors);
    
    for(i = 0; i < nbVisitors; i++)
    {
        printVisitor(visitors[i]);
    }
    
    printf("EHEHEHAH\n");

    return 0;
}
