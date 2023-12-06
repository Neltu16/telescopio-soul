#include <iostream>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

struct Pixel {
    int green;
    int red;
    int blue;
};

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

    const int width = 10681;
    const int height = 7121;
    const int channels = 3;  // Rojo, verde y azul STB no soporta alfa

    // Abrir los archivos
    std::ifstream alfaFile(alfaFilename);
    std::ifstream rojoFile(rojoFilename);
    std::ifstream verdeFile(verdeFilename);
    std::ifstream azulFile(azulFilename);
    std::ifstream promedioFile(promedioFilename);

    if (!alfaFile.is_open() || !rojoFile.is_open() || !verdeFile.is_open() || !azulFile.is_open() || !promedioFile.is_open()) {
        std::cerr << "Error al abrir uno de los archivos\n";
        return 1;
    }
    // Crear una matriz para almacenar los píxeles de todos los canales durante la ejecución
    Pixel** allPixels = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        allPixels[i] = new Pixel[width];
    }
    // Leer los valores de los archivos y almacenarlos en la matriz única
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Leer los valores de los archivos alfa.txt, verde.txt, rojo.txt y azul.txt
            std::string alfaValue, verdeValue, rojoValue, azulValue, promedioValue;
            alfaFile >> alfaValue;
            verdeFile >> verdeValue;
            rojoFile >> rojoValue;
            azulFile >> azulValue;
            promedioFile >> promedioValue;

            // Convertir a entero y almacenar en la matriz única
            if (alfaValue == "*" || verdeValue == "*" || rojoValue == "*" || azulValue == "*") {
                // Valor perdido, utilizar el valor correspondiente de promedio.txt
                allPixels[i][j].green = std::stoi(promedioValue);
                allPixels[i][j].red = std::stoi(promedioValue);
                allPixels[i][j].blue = std::stoi(promedioValue);
            } else {
                allPixels[i][j].green = std::stoi(verdeValue);
                allPixels[i][j].red = std::stoi(rojoValue);
                allPixels[i][j].blue = std::stoi(azulValue);
            }
        }
    }

    // Cerrar los archivos
    alfaFile.close();
    rojoFile.close();
    verdeFile.close();
    azulFile.close();
    promedioFile.close();

    // Crear una matriz de píxeles para la imagen combinada
    unsigned char* imagePixels = new unsigned char[width * height * channels];
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imagePixels[(i * width + j) * channels] = static_cast<unsigned char>(allPixels[i][j].red);
            imagePixels[(i * width + j) * channels + 1] = static_cast<unsigned char>(allPixels[i][j].green);
            imagePixels[(i * width + j) * channels + 2] = static_cast<unsigned char>(allPixels[i][j].blue);
        }
    }

    // Escribir la imagen en formato PNG
    stbi_write_png("combinada_image.png", width, height, channels, imagePixels, width * channels);

    // Liberar la memoria
    for (int i = 0; i < height; ++i) {
        delete[] allPixels[i];
    }
    delete[] allPixels;
    delete[] imagePixels;

    return 0;
}




