#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int T; // Number of threads
int n; // Number of elements

// Filename buffers
char input_file[30];
char output_file[30];

double complex *input;
double complex *results;

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

void _fft(double complex *input, double complex *results, int step){
    
    if (step < n) {
        _fft(results, input, step * 2);
        _fft(results + step, input + step, step * 2);
 
        for (int i = 0; i < n; i += 2 * step) {
            double complex t = cexp(-I * M_PI * i / n) * results[i + step];
            input[i / 2]     = results[i] + t;
            input[(i + n)/2] = results[i] - t;
        }
    }
}

void* thread_function1(void *var){

    int step = *(int*)var;
    _fft(input, results, step);
    return NULL;
}
void* thread_function2(void *var){

    int step = *(int*)var;
    _fft(input + 1, results + 1, step);
    return NULL;
}
void* thread_function3(void *var){

    int step = *(int*)var;
    _fft(input + 2, results + 2, step);
    return NULL;
}
void* thread_function4(void *var){

    int step = *(int*)var;
    _fft(input + 3, results + 3, step);
    return NULL;
}


void twoThreads(){

    int i;
    pthread_t tid[T];
    int step = 2;
    pthread_create(&(tid[0]), NULL, thread_function1, &step);
    pthread_create(&(tid[1]), NULL, thread_function2, &step);
    
    for(i = 0; i < T; i++) {
        pthread_join(tid[i], NULL);
    }
    step = 1;
    for (int i = 0; i < n; i += 2 * step) {
        double complex t = cexp(-I * M_PI * i / n) * input[i + step];
        results[i / 2]     = input[i] + t;
        results[(i + n)/2] = input[i] - t;
    }

}

void fourThreads(){

    int i;
    pthread_t tid[T];
    int step = 4;
    pthread_create(&(tid[0]), NULL, thread_function1, &step);
    pthread_create(&(tid[1]), NULL, thread_function2, &step);
    pthread_create(&(tid[2]), NULL, thread_function3, &step);
    pthread_create(&(tid[3]), NULL, thread_function4, &step);
    
    for(i = 0; i < T; i++) {
        pthread_join(tid[i], NULL);
    }

    step = 2;

    for (int i = 0; i < n; i += 2 * step) {
        double complex t = cexp(-I * M_PI * i / n) * input[i + step];
        results[i / 2]     = input[i] + t;
        results[(i + n)/2] = input[i] - t;
    }
    input = input + 1;
    results = results + 1;

    for (int i = 0; i < n; i += 2 * step) {
        double complex t = cexp(-I * M_PI * i / n) * input[i + step];
        results[i / 2]     = input[i] + t;
        results[(i + n)/2] = input[i] - t;
    }
    step = 1;
    input = input - 1;
    results = results - 1;
    for (int i = 0; i < n; i += 2 * step) {
        double complex t = cexp(-I * M_PI * i / n) * results[i + step];
        input[i / 2]     = results[i] + t;
        input[(i + n)/2] = results[i] - t;
    }
    results = input;

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

void run(){

    if (T == 1){
        fprintf(stdout, "1 Thread \n");
        _fft(results, input, 1);
    }
    else if (T == 2){
        fprintf(stdout, "2 Threads \n");
        twoThreads();
    }
    else if (T == 4){
        fourThreads();
    }
    else {
        fprintf(stdout, "No. of threads must be 1, 2, 4!");
        exit(1);
    }
}


int main(int argc, char * argv[]){

    getArgs(argc, argv);
    
    readInput();

    int i;
    for (i = 0; i < n; i++) 
        results[i] = input[i];
    run();

    printResults();

    return 0;

}
