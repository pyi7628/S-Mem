CC=gcc
LIB1=-lpthread
LIB2=-lm
all:
	$(CC) -o bench ./src/main.c ./src/benchmark_routine.c ./src/zipfianGenerator.c \
		./src/common.c ./src/zipfian_random.c \
		-I./src/includes $(LIB1) $(LIB2)

