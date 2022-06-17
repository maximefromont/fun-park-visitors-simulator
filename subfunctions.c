//SUB FUNCTIONS//

//Generate a random number between a minimum and a maximum
int initRandom(int max, int min)
{
    return rand()%(max-min) + min;
}

//Print all informations about a visitor
void printVisitor(struct visitor vis)
{
    printf("Money : %d\n", vis.money);
    printf("Patience : %d\n", vis.patience);
}
