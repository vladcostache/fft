#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double *input;
    double *results;
}args;

args a;

int n = 9;

void init(){
	a.input = malloc(sizeof(double) * 10);
	a.results = malloc(sizeof(double) * 10);
}

void* thread(void *ptr){

	args a = *(args*)ptr;
	//int thread_id = *(int*)var;


	int i;
	for(i = 0; i < n; i++)
        printf("%lf\n", a.input[i]);


	return 0;
}

int main(){


	init();
	
	int T = 2;
	int i;
	for(i = 0; i < 10; i++){
		a.input[i] = i - 10;
	}

	for(i = 0; i < 10; i++){
		a.results[i] = i + 10;
	}

	pthread_t tid[T];
    // int thread_id[T];

    // for(i = 0; i < T; i++)
    //     thread_id[i] = i;

    for(i = 0; i < T; i++) {
        pthread_create(&(tid[i]), NULL, thread, (void *)&a);
    }

    for(i = 0; i < T; i++) {
        pthread_join(tid[i], NULL);
    }


    for(i = 0; i <= n; i++){
    	printf("%lf\n", a.input[i]);
    }



  

  	return 0;

}