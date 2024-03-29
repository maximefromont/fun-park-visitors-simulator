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

#define NB_VISITORS 7

#define NB_ATTRACTIONS 6
#define CAPACITY_MAX 5
#define CAPACITY_MIN 1

#define OPEN_TIME 30
//---------------------------------------------//


//------------------STRUCTURES------------------//
typedef struct
{
    int id;
    char* name;
    int money;
    int patience;

    struct attraction* target;
}visitor;

typedef struct
{
    char* name;
    int capacity;  // Max number of visitors in the attraction
    int duration;
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
    printf("Id : %d\n", vis.id);
    printf("Name : %s\n", vis.name);
    printf("Money : %d\n", vis.money);
    printf("Patience : %d\n", vis.patience);
}

void printVisitorAttraction(attraction att) //Print all informations about an attraction
{
    printf("Name : %s\n", att.name);
    printf("Capacity : %d\n", att.capacity);
    printf("Duration : %d\n", att.duration);
}
//------------------------------------------------//


//------------------GLOBAL INITIALIZATION------------------//
visitor visitors[NB_VISITORS];
attraction attractions[NB_ATTRACTIONS];
int turnover = 0; // Profit + Growth = Grofit
int closing = 0; // False
//---------------------------------------------------------//


//------------------VISITOR FUNCTIONS------------------//
void* visitorSoul(void *arg)
{
    visitor* vis = (visitor*) arg;
    printf("Creation du visiteur %s\n", vis->name);
    vis->money -= 30;
    turnover += 30;

    while(1)
    {
        if (closing == 1) // On pourrait mettre seulement if(closing) mais la c est plus clair
        {
            break;
        }

        int nextAttrId;
        nextAttrId = randomBetween(NB_ATTRACTIONS, 0);
        attraction nextAttr = attractions[nextAttrId];
        sem_wait(&nextAttr.sem);

        printf("%s %d entre dans \"%s\" (capacite %d)\n", vis->name, vis->id, nextAttr.name, nextAttr.capacity);
        sleep(nextAttr.duration);
        
        sem_post(&nextAttr.sem);
        printf("%s %d sort de l'attraction \"%s\" (apres %d sec de fun)\n", vis->name, vis->id, nextAttr.name, nextAttr.duration);
    }
    printf("%s %d se dirige vers la sortie apres une bonne journee au parc\n", vis->name, vis->id);
    
   return 0;
}

//Initialize an array of visitor with random attributes values using the constants
void initVisitor(visitor visitors[], pthread_t id[], int n) 
{
    //Initialization
    char* names[] = {"Garry", "Larry", "Harry", "Macy", "Ivy", "Henry", "Poppy", "Daisy", "Ricky", "Morty"};
    int i;
    srand(time(NULL));
    
    for(i = 0; i < n; i++)
    {
        visitors[i].name = names[i%10];
        visitors[i].id = i;
        visitors[i].money = randomBetween(MONEY_MAX, MONEY_MIN);
        visitors[i].patience = randomBetween(PATIENCE_MAX, PATIENCE_MIN);
        pthread_create(&id[i], NULL, visitorSoul, &visitors[i]);
    }
}

//Wait for all visitors to end their routine
void waitVisitor(pthread_t id[], int n)
{
    //Initialization
    int i;
    
    for(i = 0; i < n; i++)
    {
        pthread_join(id[i], NULL);
    }
}
//-----------------------------------------------------//


//------------------ATTRACTIONS FUNCTIONS------------------//
//Initialize an array of attractions with random attributes values using the constants
void initAttractions(attraction attractions[], int n)
{
    //Initialization
    char *a[] = {"Le tournis de la mort", "La grande chute", "Aled en folie", "La maison hantee", "RoBoTroN X12 simulator", "Youngling slayer 2000", "La spirale infernale", "No Juridic Respondability ULTRA FUN", "Github mental sanity nightmare"};
    int i;
    
    attractions[0].name = "l'allee du parc";
    attractions[0].capacity = NB_VISITORS;
    attractions[0].duration = 5;
    sem_init(&attractions[0].sem, 0, attractions[0].capacity);
    for(i = 1; i < n; i++)
    {
        attractions[i].name = a[(i-1)%n];
        if (randomBetween(100, 0) > 30) // Attractions normales
        {
            attractions[i].capacity = randomBetween(CAPACITY_MAX, CAPACITY_MIN);
        }
        else // Attractions libres
        {
            attractions[i].capacity = NB_VISITORS;
        }
        attractions[i].duration = randomBetween(8, 2);
        sem_init(&attractions[i].sem, 0, attractions[i].capacity);
    }
}
//---------------------------------------------------------//


//------------------MAIN------------------//
int main()
{
    //Initialization
    pthread_t id[NB_VISITORS];
    //int i;
    
    initVisitor(visitors, id, NB_VISITORS);
    initAttractions(attractions, NB_ATTRACTIONS);
    
    //Test print visitors
    /*
    for(i = 0; i < NB_VISITORS; i++)
    {
        printVisitor(visitors[i]);
    }
    */
    
    //Test print attractions 
    /*
    for(i = 0; i < NB_ATTRACTIONS; i++)
    {
        prinAttraction(attractions[i]);
    }
    */

   sleep(OPEN_TIME);
   closing = 1;
    
    waitVisitor(id, NB_VISITORS);

    printf("Argent gagne par le parc apres une bonne journee de travail ayant duree %d secondes : %d euros\n", OPEN_TIME, turnover);

    return 0;
}
//----------------------------------------//
