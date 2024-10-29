
# Compresor LZW en C++

Este proyecto implementa el algoritmo de compresión LZW en C++ para comprimir y descomprimir archivos en modo binario. El algoritmo LZW es una técnica de compresión sin pérdida que utiliza un diccionario para almacenar y reutilizar subcadenas, lo que ayuda a reducir el tamaño de los archivos al eliminar redundancias.

## Descripción del Algoritmo LZW

El algoritmo LZW (Lempel-Ziv-Welch) es una mejora del algoritmo LZ78. Trabaja con un diccionario de subcadenas, asociando cada subcadena a un código único. Cada vez que se encuentra una nueva subcadena, se almacena en el diccionario y se asigna un código único. Durante la compresión, cada subcadena se reemplaza por su código correspondiente en el diccionario, logrando así la reducción del tamaño del archivo.

## Estructura del Proyecto

- **LZWCompressor.h**: Define la clase `LZWCompressor` con los métodos de compresión y descompresión.
- **LZWCompressor.cpp**: Implementa los métodos `compress` y `decompress`, y muestra la tabla de códigos generada durante la compresión.
- **main.cpp**: Programa principal que permite la interacción con el usuario a través de la línea de comandos para comprimir o descomprimir.
- **Makefile**: Archivo para compilar el proyecto en Linux.

## Uso del Programa

Para comprimir o descomprimir un archivo, el programa acepta los siguientes argumentos en la línea de comandos:

```bash
./lzw_program -c <archivo_entrada> <archivo_salida>  # Comprimir un archivo
./lzw_program -d <archivo_comprimido> <archivo_salida>  # Descomprimir un archivo
```

## Pseudocódigos de las Funciones

### Función de Compresión

La función `compress` toma un archivo de entrada y genera un archivo comprimido utilizando el algoritmo LZW.

#### Pseudocódigo

```
function compress(inputFilePath, outputFilePath):
    abrir archivo de entrada en modo binario
    si archivo de entrada no está abierto:
        mostrar error y terminar

    abrir archivo de salida en modo binario
    si archivo de salida no está abierto:
        mostrar error y terminar

    inicializar dictionary con todos los caracteres únicos
    inicializar buffer como cadena vacía
    inicializar dictIndex en 256

    mientras leer símbolo del archivo de entrada:
        combined = buffer + símbolo

        si combined está en dictionary:
            buffer = combined
        si no:
            escribir dictionary[buffer] en archivo de salida
            dictionary[combined] = dictIndex
            incrementar dictIndex
            buffer = símbolo

    si buffer no está vacío:
        escribir dictionary[buffer] en archivo de salida

    cerrar archivo de entrada
    cerrar archivo de salida
    mostrar la tabla de códigos
```

### Función de Descompresión

La función `decompress` toma un archivo comprimido y lo descomprime utilizando el algoritmo LZW.

#### Pseudocódigo

```
function decompress(inputFilePath, outputFilePath):
    abrir archivo de entrada en modo binario
    si archivo de entrada no está abierto:
        mostrar error y terminar

    abrir archivo de salida en modo binario
    si archivo de salida no está abierto:
        mostrar error y terminar

    inicializar dictionary con caracteres únicos
    leer el primer código y escribir la subcadena en el archivo de salida
    inicializar buffer con la subcadena del primer código

    mientras leer código del archivo de entrada:
        si el código está en dictionary:
            entrada = dictionary[código]
        si no:
            entrada = buffer + buffer[0]

        escribir entrada en archivo de salida
        dictionary[dictIndex] = buffer + entrada[0]
        incrementar dictIndex
        buffer = entrada

    cerrar archivo de entrada
    cerrar archivo de salida
```

## Código de las Funciones

### Función `compress`

```cpp
bool LZWCompressor::compress(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de entrada.
";
        return false;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de salida.
";
        return false;
    }

    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, i)] = i;
    }

    std::string buffer;
    int dictIndex = 256;
    char symbol;

    while (inputFile.get(symbol)) {
        std::string combined = buffer + symbol;

        if (dictionary.count(combined)) {
            buffer = combined;
        } else {
            outputFile.write(reinterpret_cast<const char*>(&dictionary[buffer]), sizeof(int));
            dictionary[combined] = dictIndex++;
            buffer = std::string(1, symbol);
        }
    }

    if (!buffer.empty()) {
        outputFile.write(reinterpret_cast<const char*>(&dictionary[buffer]), sizeof(int));
    }

    displayWordCodeTable(dictionary);

    inputFile.close();
    outputFile.close();
    return true;
}
```

### Función `decompress`

```cpp
bool LZWCompressor::decompress(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo comprimido.
";
        return false;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de salida.
";
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
```

## Compilación y Ejecución

Para compilar el programa en Linux, utiliza el `Makefile` proporcionado:

```bash
make  # Compila el proyecto
make clean  # Limpia los archivos compilados
```

Ejecuta el programa utilizando los comandos indicados en la sección "Uso del Programa".

## Notas Importantes

- El archivo comprimido incluye los códigos en binario, lo que lo hace ilegible sin descompresión.
- La implementación muestra la tabla de códigos al final de la compresión, para observar cómo se forman las subcadenas.

## Referencias

- [Wikipedia - LZW](https://en.wikipedia.org/wiki/Lempel–Ziv–Welch): Explicación detallada del algoritmo LZW y su implementación.
