#include <iostream>
#include <fstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

const int width = 10681;
const int height = 7121;
const int channels = 3;

// Retorna un valor dependiendo de dos casos para cada canal, si es "*" retonará promedio sino el mismo valor del canal de color
unsigned char getValue(const std::string& value, const std::string& promedioValue) {
    return (value == "*") ? static_cast<unsigned char>(std::stoi(promedioValue)) : static_cast<unsigned char>(std::stoi(value));
}

void closeFiles(std::ifstream& alfaFile, std::ifstream& rojoFile, std::ifstream& verdeFile, std::ifstream& azulFile, std::ifstream& promedioFile) {
    alfaFile.close();
    rojoFile.close();
    verdeFile.close();
    azulFile.close();
    promedioFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Uso: " << argv[0] << " alfa.txt rojo.txt verde.txt azul.txt promedio.txt\n";
        return 1;
    }

    // Abrir los archivos directamente con los nombres obtenidos de argv
    std::ifstream alfaFile(argv[1]), rojoFile(argv[2]), verdeFile(argv[3]), azulFile(argv[4]), promedioFile(argv[5]);

    if (!(alfaFile && rojoFile && verdeFile && azulFile && promedioFile)) {
        std::cerr << "Error al abrir uno de los archivos\n";
        return 1;
    }

    // Crear un solo arreglo para almacenar los píxeles de todos los canales, movimiento a través de files, columnas y su color
    auto* imagePixels = new unsigned char[width * height * channels];

    // Leer los valores de los archivos y almacenarlos directamente en el arreglo
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::string verdeValue, rojoValue, azulValue, promedioValue;

            verdeFile >> verdeValue;
            rojoFile >> rojoValue;
            azulFile >> azulValue;
            promedioFile >> promedioValue;

            // Convertir a entero y almacenar directamente en el arreglo unidimensional (R,G,B) y se repite...
            int index = (i * width + j) * channels;
            imagePixels[index] = getValue(rojoValue, promedioValue);
            imagePixels[index + 1] = getValue(verdeValue, promedioValue);
            imagePixels[index + 2] = getValue(azulValue, promedioValue);
        }
    }

    // Cerrar los archivos
    closeFiles(alfaFile, rojoFile, verdeFile, azulFile, promedioFile);

    // Escribir la imagen en formato PNG
    stbi_write_png("combinada_image.png", width, height, channels, imagePixels, width * channels);

    // Liberar la memoria
    delete[] imagePixels;

    return 0;
}



