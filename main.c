#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

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

int main()
{
    printf("Hello World");

    return 0;
}