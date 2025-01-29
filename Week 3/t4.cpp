// Implement a program that encrypts and decrypts a file. If you made task 3 you may use the authentication to help encryption.

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <fstream>
#include <vector>
#include <iostream>

// Throw an error whenever there is an error :D
void handleErrors() {
    throw std::runtime_error("Encryption/Decryption error");
}

// This function handles both encryption and decryption of data
std::vector<unsigned char> performCrypto(bool encrypt, const std::vector<unsigned char>& input,
                                       const unsigned char* key, const unsigned char* iv) {
    // Create a new encryption/decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
        handleErrors();

    // Initialize the encryption/decryption operation
    if(1 != EVP_CipherInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv, encrypt))
        handleErrors();

    // Prepare output buffer (with extra space for padding)
    std::vector<unsigned char> output(input.size());
    int len;

    // For decryption, set the tag before processing data
    if (!encrypt) {
        if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, const_cast<unsigned char*>(input.data() + input.size() - 16)))
            handleErrors();
    }

    // Perform the actual encryption/decryption
    if(1 != EVP_CipherUpdate(ctx, output.data(), &len, input.data(), input.size() - (encrypt ? 0 : 16)))
        handleErrors();
    output.resize(len);  // Resize output to actual data size

    // Finalize the operation
    if(1 != EVP_CipherFinal_ex(ctx, nullptr, &len))
        handleErrors();

    // For encryption, append the tag
    if(encrypt) {
        unsigned char tag[16];
        if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
            handleErrors();
        output.insert(output.end(), tag, tag + 16);
    }

    EVP_CIPHER_CTX_free(ctx);
    return output;
}

int main() {
    // Define encryption key and initialization vector (IV)
    unsigned char key[32] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
                            '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    unsigned char iv[12] = {'0','1','2','3','4','5','6','7','8','9','0','1'};
    
    // Get filename from user
    std::string filename;
    std::cout << "Enter filename: ";
    std::getline(std::cin, filename);

    // Open and read the input file
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Cannot open input file\n";
        return 1;
    }

    // Read entire file into a vector
    std::vector<unsigned char> input((std::istreambuf_iterator<char>(inFile)),
                                   std::istreambuf_iterator<char>());
    inFile.close();

    // Ask user whether to encrypt or decrypt
    char mode;
    std::cout << "Encrypt (e) or Decrypt (d)? ";
    std::cin >> mode;

    // Check if input size is valid for decryption (needs at least tag size)
    if (mode == 'd') {
        if (input.size() < 16) {
            std::cerr << "Input file is too small to be a valid encrypted file\n";
            return 1;
        }
        // Verify the file has both data and authentication tag
        if (input.size() == 16) {
            std::cerr << "Input file contains only the authentication tag\n";
            return 1;
        }
    }

    // Perform the chosen operation
    std::vector<unsigned char> output = performCrypto(mode == 'e', input, key, iv);

    // Save the result to a new file with .enc or .dec extension
    std::ofstream outFile(filename + (mode == 'e' ? ".enc" : ".dec"), std::ios::binary);
    outFile.write(reinterpret_cast<char*>(output.data()), output.size());

    return 0;
}

// Compile with
// g++ -o t4 t4.cpp -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto