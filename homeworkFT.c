#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void ft_complex(const double complex input[], double complex results[], int n) {
	for (int k = 0; k < n; k++) {  // For each output element
		complex double sum = 0.0;
		for (int i = 0; i < n; i++) {  // For each input element
			double angle = 2 * M_PI * i * k / n;
			sum += input[t] * cexp(-angle * I);
		}
		results[k] = sum;
	}
}

void printResults(int n, double complex results[]) {
	printf("%d\n", n);
	for (int i = 0; i < n; i++)
			printf("(%g, %g) ", creal(results[i]), cimag(results[i]));
}

int main(int argc, char * argv[]){

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (fp1 == NULL || fp2 == NULL) {
        fprintf(stdout, "Failed to open at least one file.\n");
        exit(1);
    }

    int n, i;
    fscanf(in, "%d", &n);

    double complex input[] = {166.000000, 740.000000, 881.000000, 241.000000, 12.000000, 758.000000};
    double complex results[] = {0};

    for (i = 0; i < n; i++)
        fscanf(in, "%lf", input[i]);
    
    ft_complex(input, results, n);

    printResults(n, results);

    fclose(in);
    fclose(out);
    return 0;

}
