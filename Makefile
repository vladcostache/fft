
fft: homeworkFFT.c
		gcc -o homeworkFFT homeworkFFT.c -O3 -lpthread -lm -Wall
ft: homeworkFT.c
		gcc -o homeworkFT homeworkFT.c -O3 -lpthread -lm -Wall
compare: compareOutputs.c
		gcc -o compare compareOutputs.c -lpthread -lm -Wall
clean:
		rm homeworkFT compare