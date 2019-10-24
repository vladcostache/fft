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

// Thread function args structure
typedef struct {
    int index;
    int step;
}inds;

inds args;

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


// void* _fft(double complex *input, double complex *results, int step)
// {
//     if (step < n) {
//         _fft(results, input, step * 2);
//         _fft(results + step, input + step, step * 2);
//         int i;
//         for (i = 0; i < n; i += 2 * step) {
//             double complex t = cexp(-I * M_PI * i / n) * results[i + step];
//             input[i / 2]     = results[i] + t;
//             input[(i + n)/2] = results[i] - t;
//         }
//     }
// }

void* _fft(void* ptr){

    inds args = *(inds*)ptr;
    if (args.step < n) {
        _fft((void *)&args);
        _fft(results + args.step, input + args.step, args.step * 2);
        int i;
        for (i = 0; i < n; i += 2 * args.step) {
            double complex t = cexp(-I * M_PI * i / n) * results[i + args.step];
            input[i / 2]     = results[i] + t;
            input[(i + n)/2] = results[i] - t;
        }
    }
    return NULL;

}


void twoThreads(){



    _fft(results, input, step * 2);
    _fft(results + step, input + step, step * 2);
}

void fourThreads(){

    

    _fft(results, input, step * 2);
    _fft(results + step, input + step, step * 2);
}








    // _fft(results, input, step * 2);
    // _fft(results + step, input + step, step * 2);
    // _fft(results + step + 1, input + step + 1, step * 2);
    // _fft(results + step + 2, input + step + 2, step * 2);
 

void printResults() {
	FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        fprintf(stdout, "Failed to open output file.\n");
        exit(1);
    }
    fprintf(out, "%d\n", n); // Print N
    int i;
	for (i = 0; i < n; i++)
			fprintf(out, "%f %f\n", creal(input[i]), cimag(input[i]));

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
    int i;
    for (i = 0; i < n; i++) 
        results[i] = input[i];
    args.step = 1;
    
    // int thread_id[T];
    // pthread_barrier_init(&barrier, NULL, T);

    pthread_t tid[T];

    pthread_create(&(tid[0]), NULL, _fft, (void *)&args);

    // for(i = 0; i < T; i++) {
    //     pthread_create(&(tid[i]), NULL, ft_iterative, &(thread_id[i]));
    // }

    pthread_join(tid[0], NULL);

    // for(i = 0; i < T; i++) {
    //     pthread_join(tid[i], NULL);
    // }

    // pthread_barrier_destroy(&barrier);

    
    printResults();

    return 0;

}
