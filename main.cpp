#include <iostream>
#include <fstream>
#include <chrono>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"
#include "mpi.h"

const int width = 10681;
const int height = 7121;
const int channels = 4;  // (R, G, B, A)

void closeFiles(std::ifstream& alfaFile, std::ifstream& rojoFile, std::ifstream& verdeFile, std::ifstream& azulFile, std::ifstream& promedioFile) {
    alfaFile.close();
    rojoFile.close();
    verdeFile.close();
    azulFile.close();
    promedioFile.close();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    auto start_time = std::chrono::high_resolution_clock::now();
    if (argc != 6) {
        if (rank == 0) {
            std::cerr << "Uso: " << argv[0] << " alfa.txt rojo.txt verde.txt azul.txt promedio.txt\n";
        }
        MPI_Finalize();
        return 1;
    }

    // Calcular la cantidad de filas que cada proceso MPI procesará
    int rows_per_process = height / size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;

    // Abrir los archivos directamente con los nombres obtenidos de argv
    std::ifstream alfaFile(argv[1]), rojoFile(argv[2]), verdeFile(argv[3]), azulFile(argv[4]), promedioFile(argv[5]);

    if (!(alfaFile && rojoFile && verdeFile && azulFile && promedioFile)) {
        std::cerr << "Error al abrir uno de los archivos\n";
        MPI_Finalize();
        return 1;
    }

    // Avanzar a la fila de inicio en los archivos
    for (int i = 0; i < start_row; ++i) {
        alfaFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        rojoFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        verdeFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        azulFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        promedioFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Crear un solo arreglo para almacenar los píxeles de todos los canales
    auto* imagePixels = new unsigned char[width * channels * rows_per_process];
    std::string alfaValue, rojoValue, verdeValue, azulValue, promedioValue;
    alfaValue.reserve(3);  // Ajusta el tamaño según la longitud máxima esperada
    rojoValue.reserve(3);
    verdeValue.reserve(3);
    azulValue.reserve(3);
    promedioValue.reserve(3);

    // Leer los valores de los archivos y almacenarlos directamente en el arreglo
    for (int i = 0; i < rows_per_process; ++i) {
        for (int j = 0; j < width; ++j) {
            alfaFile >> alfaValue;
            rojoFile >> rojoValue;
            verdeFile >> verdeValue;
            azulFile >> azulValue;
            promedioFile >> promedioValue;

            try {
                int index = (i * width + j) * channels;

                imagePixels[index]     = (rojoValue == "*") ? static_cast<unsigned char>(std::stoi(promedioValue) * 0.3) : static_cast<unsigned char>(std::stoi(rojoValue));
                imagePixels[index + 1] = (verdeValue == "*") ? static_cast<unsigned char>(std::stoi(promedioValue) * 0.59) : static_cast<unsigned char>(std::stoi(verdeValue));
                imagePixels[index + 2] = (azulValue == "*") ? static_cast<unsigned char>(std::stoi(promedioValue) * 0.11) : static_cast<unsigned char>(std::stoi(azulValue));
                imagePixels[index + 3] = static_cast<unsigned char>(std::stoi(alfaValue));
            } catch (const std::exception& e) {
                std::cerr << "Error convirtiendo a entero en posición (" << i << ", " << j << "): " << e.what() << std::endl;
                MPI_Finalize();
                return 1;  // Terminar el programa debido al error
            }
        }
    }

    // Cerrar los archivos
    closeFiles(alfaFile, rojoFile, verdeFile, azulFile, promedioFile);

    // Recolectar los datos de cada proceso en el proceso 0
    if (rank == 0) {
        auto* combinedPixels = new unsigned char[width * channels * height];

        MPI_Gather(imagePixels, width * channels * rows_per_process, MPI_UNSIGNED_CHAR,
                   combinedPixels, width * channels * rows_per_process, MPI_UNSIGNED_CHAR,
                   0, MPI_COMM_WORLD);

        // Escribir la imagen en formato PNG
        if (rank == 0) {
            stbi_write_png("combinada_image.png", width, height, channels, combinedPixels, width * channels);
            delete[] combinedPixels;
        }
    } else {
        // Enviar los datos del proceso actual al proceso 0
        MPI_Gather(imagePixels, width * channels * rows_per_process, MPI_UNSIGNED_CHAR,
                   nullptr, 0, MPI_UNSIGNED_CHAR,
                   0, MPI_COMM_WORLD);
    }

    // Obtener el tiempo de finalización
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    if (rank == 0) {
        std::cout << "Tiempo transcurrido: " << duration.count() << " milisegundos\n";
    }

    // Liberar la memoria
    delete[] imagePixels;

    MPI_Finalize();

    return 0;
}




