// LZWCompressor.h
#ifndef LZWCOMPRESSOR_H
#define LZWCOMPRESSOR_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

class LZWCompressor {
public:
    // Comprime el archivo de entrada y guarda el resultado en el archivo de salida
    bool compress(const std::string& inputFilePath, const std::string& outputFilePath);

    // Descomprime el archivo comprimido y guarda el resultado en el archivo de salida
    bool decompress(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    // Muestra la tabla de códigos generada durante la compresión
    void displayWordCodeTable(const std::unordered_map<std::string, int>& dictionary) const;
};

#endif // LZWCOMPRESSOR_H
