#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void compute_integral_image(int** image, int** integral_image, int start_col, int end_col, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = start_col; j < end_col; j++) {
            if (j == start_col) {
                integral_image[i][j] = image[i][j];
            } else {
                integral_image[i][j] = integral_image[i][j - 1] + image[i][j];
            }
        }
        for (int j = start_col; j < end_col; j++) {
            if (i == 0) {
                continue;
            }
            integral_image[i][j] += integral_image[i - 1][j];
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int sizes[] = {100, 200, 400, 800};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int s = 0; s < num_sizes; s++) {
        int width = sizes[s];
        int height = sizes[s];
        int local_width = width / size;
        int start_col = rank * local_width;
        int end_col = start_col + local_width;

        if (rank == 0) {
            printf("Procesando tamaño: %d x %d\n", width, height);
        }

        // Reservar memoria para la imagen y la imagen integral
        int** image = (int**)malloc(height * sizeof(int*));
        int** integral_image = (int**)malloc(height * sizeof(int*));
        for (int i = 0; i < height; i++) {
            image[i] = (int*)malloc(width * sizeof(int));
            integral_image[i] = (int*)malloc(width * sizeof(int));
        }

        // Inicializar la imagen con valores aleatorios de píxeles
        srand(12345);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                image[i][j] = rand() % 256;
            }
        }

        // Calcular la imagen integral y medir el tiempo de ejecución
        double start = MPI_Wtime();
        compute_integral_image(image, integral_image, start_col, end_col, height);
        double end = MPI_Wtime();

        // Mostrar el tiempo de ejecución
        if (rank == 0) {
            printf("Tamaño: %d x %d, tiempo de ejecución: %f segundos\n", width, height, end - start);
        }

        // Liberar memoria
        for (int i = 0; i < height; i++) {
            free(image[i]);
            free(integral_image[i]);
        }
        free(image);
        free(integral_image);
    }

    MPI_Finalize();
    return 0;
}
