#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    int number;
    uint8_t counter;
} PTE;

/*This function creates a new page table if one doesnt exist and returns the initialized table*/
PTE* create_pte(int number, PTE** pt) {
    if (pt[number] != NULL) {
        return pt[number];
    }

    PTE *new_pte = (PTE *)malloc(sizeof(PTE));
    new_pte->number = number;
    new_pte->counter = 2;
    pt[number] = new_pte;
    return new_pte;
}

/*This function updates the counter for a page table*/
void update_counter(PTE *pte, int taken) {
    if (taken) {
        if (pte->counter < 3) {
            pte->counter++;
        }
    } else {
        if (pte->counter > 0) {
            pte->counter--;
        }
    }
}

/*This function updates the global history buffer by shifting right and updating the MSB*/
uint32_t update_gbhr(uint32_t gbhr, int taken, int N){
    if (N == 0){
        
        return 0;
    }
    
    gbhr = gbhr >> 1;

    gbhr = gbhr | (taken << (N - 1));

    return gbhr;
}

/*This function gets the index by comparing the gbhr and the PC with XOR*/
uint32_t get_index(uint32_t address, int M, int N, uint32_t GBHR) {
    
    uint32_t mask = (1 << M) - 1;

    uint32_t index = (address >> 2) & mask;

    uint32_t history = GBHR << (M-N);

    return index ^ history;

}

/*This function simply returns if the prediction was correct*/
int predict(PTE *pte) {
    return pte->counter >= 2;
}

int main(int argc, char *argv[]) {
    
    if (argc != 5) {
        printf("Usage: ./SIM gshare <GPB> <RB> <trace_file>\n");
        return 1;
    }

    //Argument handling
    int M = atoi(argv[2]);
    int N = atoi(argv[3]);
    char *trace_file = argv[4];

    //This is an array for each page table that makes it easy to organize them
    PTE **pt = calloc(1 << M, sizeof(PTE*));

    FILE *fp = fopen(trace_file, "r");

    uint64_t PC;
    uint64_t branches = 0; //Number of branches
    uint64_t miss = 0; //Number of misspredictions
    uint32_t GBHR = 0;

    char out; //Character representation of the outcome
    int taken = 0;

    // Loops till EOF
    // Will get the index, make a pte if necessary, predict its outcome, and update relevant counters
    while (fscanf(fp, "%" SCNx64 " %c", &PC, &out) == 2) {

        taken = (out == 't');

        uint32_t index = get_index((uint32_t)PC, M, N, GBHR);

        PTE *pte = create_pte(index, pt);

        if (predict(pte) != taken){
            miss++;
        }

        update_counter(pte, taken);

        GBHR = update_gbhr(GBHR, taken, N);

        branches++;
    }

    //Calculates the ratio and displays relevant data
    double ratio = 0.0;

    ratio = (double)miss/branches;

    printf("%d %d %f", M, N, ratio);

    //Closes files and frees all allocated memory

    fclose(fp);

    for (int i = 0; i < (1 << M); i++){
        free(pt[i]);
    }
    free(pt);

    return 0;
}