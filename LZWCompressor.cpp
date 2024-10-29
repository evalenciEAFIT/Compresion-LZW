// LZWCompressor.cpp
#include "LZWCompressor.h"
#include <algorithm> // Para std::sort

// Función para comprimir un archivo usando el algoritmo LZW
bool LZWCompressor::compress(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de entrada.\n";
        return false;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de salida.\n";
        return false;
    }

    // Inicializa el diccionario con todos los posibles caracteres únicos
    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, i)] = i;
    }

    std::string buffer;
    int dictIndex = 256;
    char symbol;

    // Lee cada símbolo del archivo de entrada
    while (inputFile.get(symbol)) {
        std::string combined = buffer + symbol;

        // Si la combinación está en el diccionario, actualiza el buffer
        if (dictionary.count(combined)) {
            buffer = combined;
        } else {
            // Escribe el código del buffer en el archivo de salida
            outputFile.write(reinterpret_cast<const char*>(&dictionary[buffer]), sizeof(int));

            // Agrega la combinación al diccionario y actualiza el índice
            dictionary[combined] = dictIndex++;
            buffer = std::string(1, symbol);
        }
    }

    // Manejo del último buffer
    if (!buffer.empty()) {
        outputFile.write(reinterpret_cast<const char*>(&dictionary[buffer]), sizeof(int));
    }

    // Muestra la tabla de códigos de palabras generada durante la compresión
    displayWordCodeTable(dictionary);

    inputFile.close();
    outputFile.close();
    return true;
}

// Función para descomprimir un archivo comprimido con LZW
bool LZWCompressor::decompress(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo comprimido.\n";
        return false;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de salida.\n";
        return false;
    }

    std::vector<std::string> dictionary(256);
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, i);
    }

    int prevCode, currCode;
    std::string entry, buffer;

    inputFile.read(reinterpret_cast<char*>(&prevCode), sizeof(int));
    buffer = dictionary[prevCode];
    outputFile << buffer;

    while (inputFile.read(reinterpret_cast<char*>(&currCode), sizeof(int))) {
        size_t codeIndex = static_cast<size_t>(currCode);

        if (codeIndex < dictionary.size()) {
            entry = dictionary[codeIndex];
        } else {
            entry = buffer + buffer[0];
        }

        outputFile << entry;

        dictionary.push_back(buffer + entry[0]);
        buffer = entry;
    }

    inputFile.close();
    outputFile.close();
    return true;
}

// Muestra la tabla de códigos de palabras generada durante la compresión
void LZWCompressor::displayWordCodeTable(const std::unordered_map<std::string, int>& dictionary) const {
    // Almacena las entradas del diccionario en un vector para ordenarlas por índice
    std::vector<std::pair<int, std::string>> sortedEntries;
    for (const auto& entry : dictionary) {
        sortedEntries.emplace_back(entry.second, entry.first);
    }

    // Ordena el vector por índice
    std::sort(sortedEntries.begin(), sortedEntries.end());

    // Imprime la tabla de códigos de palabras en formato de tupla (índice, subcadena)
    std::cout << "Tabla de Códigos de Palabras (Word Code Table):\n";
    for (const auto& entry : sortedEntries) {
        std::cout << "(" << entry.first << ", \"" << entry.second << "\")\n";
    }
}
