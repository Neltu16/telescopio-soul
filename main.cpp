#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <mpi.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image_write.h"
#include "lib/stb_image.h"

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

void combineImages(const std::vector<unsigned char>& imagePixels, const std::string& outputFile) {
    int total_height = height;

    stbi_write_png(outputFile.c_str(), width, total_height, channels, imagePixels.data(), width * channels);
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

    // Crear un solo vector para almacenar los píxeles de todos los canales
    std::vector<unsigned char> imagePixels(width * channels * rows_per_process);
    std::string alfaValue, rojoValue, verdeValue, azulValue, promedioValue;
    alfaValue.reserve(3);  // Ajusta el tamaño según la longitud máxima esperada
    rojoValue.reserve(3);
    verdeValue.reserve(3);
    azulValue.reserve(3);
    promedioValue.reserve(3);

    // Leer los valores de los archivos y almacenarlos directamente en el vector
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

    // Crear un vector para almacenar los píxeles recopilados en el proceso 0
    std::vector<unsigned char> receivedPixels(width * channels * rows_per_process * size);

    // Enviar la información al proceso 0 para que combine las imágenes
    MPI_Gather(imagePixels.data(), imagePixels.size(), MPI_UNSIGNED_CHAR,
               receivedPixels.data(), imagePixels.size(), MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    // Obtener el tiempo de finalización
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Proceso " << rank << ": Tiempo transcurrido: " << duration.count() << " milisegundos\n";

    // Liberar la memoria
    MPI_Finalize();

    // Combina las imágenes en el proceso 0 y genera el archivo final
    if (rank == 0) {
        combineImages(receivedPixels, "combined_image.png");
    }

    return 0;
}





