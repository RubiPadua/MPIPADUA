CC=mpicc
CFLAGS=-Wall -Wextra -O3
EXEC=mpi_img

all: $(EXEC)

$(EXEC): mpi_img.c
	$(CC) $(CFLAGS) mpi_img.c -o $(EXEC)

run:
	mpirun -np 4 ./$(EXEC)

clean:
	rm -f $(EXEC)
