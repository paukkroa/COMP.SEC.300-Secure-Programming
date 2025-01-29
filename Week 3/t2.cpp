// Please implement a programs with C/C++ which writes crypto safe random data and writes it on a file.

#include <iostream>
#include <fstream>
#include <openssl/rand.h>

int main() {
    // Set the size of random data we want
    const int dataSize = 64;
    unsigned char buffer[dataSize];

    // Generate random data using OpenSSL
    if (RAND_bytes(buffer, dataSize) != 1) {
        std::cerr << "Error generating crypto-safe random data.\n";
        return 1;
    }

    // Open a file to write the random data
    std::ofstream outFile("random_output.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    // Write the random data to the file and close it
    outFile.write(reinterpret_cast<const char*>(buffer), dataSize);
    outFile.close();
    return 0;
}

// Compile command
// g++ -o t2 t2.cpp -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto