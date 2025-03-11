all:
	gcc main.c -Wall -o main -fopenmp
	gcc pollute.c -Wall -o pollute -fopenmp
clean:
	rm main
	rm pollute
