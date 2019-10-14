#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int T, n;
double complex *input;
double complex *results;

char input_file[30];
char output_file[30];

void getArgs(int argc, char **argv){
    if(argc < 4) {
        printf("Not enough paramters: ./program input_file output_file num_threads\n");
        exit(1);
    }
    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[2]);
    T = atoi(argv[3]);
}


void init(){
    input = malloc(sizeof(double complex) * n);
    results = malloc(sizeof(double complex) * n);
    if(input == NULL || results == NULL) {
        fprintf(stdout, "Malloc failed!");
        exit(1);
    }

}


void* ft_iterative(void *var){
    int thread_id = *(int*)var;
    int i, k;
    for (k = 0; k < n; k++) {  // For each output element
        complex double sum = 0.0;
        for (i = 0; i < n; i++) {  // For each input element
            double angle = 2 * M_PI * i * k / n;
            sum += input[i] * cexp(-angle * I);
        }
        results[k] = sum;
    }
    return 0;
}


void printResults() {
	FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        fprintf(stdout, "Failed to open output file.\n");
        exit(1);
    }
    fprintf(out, "%d\n", n); // Print N
    int i;
	for (i = 0; i < n; i++)
			fprintf(out, "%f %f\n", creal(results[i]), cimag(results[i]));

    fclose(out);
}

void readInput(){
    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        fprintf(stdout, "Failed to open input file.\n");
        exit(1);
    }
    int i, ret;
    ret = fscanf(in, "%d", &n);
    if (ret != 1)
        fprintf(stdout, "Failed to read N.\n");
    init();
    double aux;
    for (i = 0; i < n; i++){
        aux = 0;
        ret = fscanf(in, "%lf", &aux);
        if(ret != 1)
            fprintf(stdout, "Failed to read value no. %d.\n", i);
        input[i] = aux + 0*I;
    }
    fclose(in);
}


int main(int argc, char * argv[]){

    getArgs(argc, argv);
    readInput();

    pthread_t tid[T];
    int thread_id[T];
    int i;
    for(i = 0; i < T; i++)
        thread_id[i] = i;

    for(i = 0; i < T; i++) {
        pthread_create(&(tid[i]), NULL, ft_iterative, &(thread_id[i]));
    }

    for(i = 0; i < T; i++) {
        pthread_join(tid[i], NULL);
    }

    printResults();

    return 0;

}
