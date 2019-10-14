#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void ft_complex(int n, double complex input[], double complex results[]) {
	for (int k = 0; k < n; k++) {  // For each output element
		complex double sum = 0.0;
		for (int i = 0; i < n; i++) {  // For each input element
			double angle = 2 * M_PI * i * k / n;
			sum += input[i] * cexp(-angle * I);
		}
		results[k] = sum;
	}
}

void printResults(char *filename, int n, double complex results[]) {
	FILE *out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stdout, "Failed to open output file.\n");
        exit(1);
    }
    fprintf(out, "%d\n", n); // Print N
	for (int i = 0; i < n; i++)
			fprintf(out, "%f %f\n", creal(results[i]), cimag(results[i]));

    fclose(out);
}

int main(int argc, char * argv[]){

    FILE *in = fopen(argv[1], "r");

    if (in == NULL) {
        fprintf(stdout, "Failed to open input file.\n");
        exit(1);
    }

    int n, i, ret;
    ret = fscanf(in, "%d", &n);
    if (ret != 1)
    	fprintf(stdout, "Failed to read N.\n");

    double complex input[5000] = {0};
    double complex results[5000] = {0};
    
    double aux;

    for (i = 0; i < n; i++){
        aux = 0;
        ret = fscanf(in, "%lf", &aux);
        if(ret != 1)
        	fprintf(stdout, "Failed to read value no. %d.\n", i);
        input[i] = aux + 0*I;
    }
    
    ft_complex(n, input, results);

    printResults(argv[2], n, results);

    fclose(in);
    return 0;

}
