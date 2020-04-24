CC=gcc
LIB1=-lpthread
LIB2=-lm
all:
	$(CC) -o bench_test ./src/main.c ./src/benchmark_routine.c \
		./src/common.c ./src/zipfian_random.c \
		-I./src/includes $(LIB1) $(LIB2)

