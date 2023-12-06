#include <iostream>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

const int width = 10681;
const int height = 7121;
const int channels = 3;

unsigned char getValue(const std::string& value, const std::string& promedioValue) {
    return (value == "*") ? static_cast<unsigned char>(std::stoi(promedioValue)) : static_cast<unsigned char>(std::stoi(value));
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Uso: " << argv[0] << " alfa.txt rojo.txt verde.txt azul.txt promedio.txt\n";
        return 1;
    }

    const char* alfaFilename = argv[1];
    const char* rojoFilename = argv[2];
    const char* verdeFilename = argv[3];
    const char* azulFilename = argv[4];
    const char* promedioFilename = argv[5];

    // Abrir los archivos
    std::ifstream alfaFile(alfaFilename), rojoFile(rojoFilename), verdeFile(verdeFilename), azulFile(azulFilename), promedioFile(promedioFilename);

    if (!(alfaFile && rojoFile && verdeFile && azulFile && promedioFile)) {
        std::cerr << "Error al abrir uno de los archivos\n";
        return 1;
    }

    // Crear un solo arreglo para almacenar los píxeles de todos los canales
    auto* imagePixels = new unsigned char[width * height * channels];
    // Leer los valores de los archivos y almacenarlos directamente en el arreglo unidimensional
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::string verdeValue, rojoValue, azulValue, promedioValue;

            verdeFile >> verdeValue;
            rojoFile >> rojoValue;
            azulFile >> azulValue;
            promedioFile >> promedioValue;

            // Convertir a entero y almacenar directamente en el arreglo unidimensional
            imagePixels[(i * width + j) * channels] = getValue(rojoValue, promedioValue);
            imagePixels[(i * width + j) * channels + 1] = getValue(verdeValue, promedioValue);
            imagePixels[(i * width + j) * channels + 2] = getValue(azulValue, promedioValue);
        }
    }

    // Cerrar los archivos
    alfaFile.close();
    rojoFile.close();
    verdeFile.close();
    azulFile.close();
    promedioFile.close();

    // Escribir la imagen en formato PNG
    stbi_write_png("combinada_image.png", width, height, channels, imagePixels, width * channels);

    // Liberar la memoria
    delete[] imagePixels;

    return 0;
}



