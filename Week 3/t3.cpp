// Implement a program that authenticates a user with a user name and password. Hashes of passwords are stored in a file. Please notice to use random salt.

#include <iostream>     
#include <fstream>      //  File operations
#include <string> 
#include <unordered_map> 
#include <random>       // Generating random salt
#include <iomanip>      
#include <sstream>      
#include <openssl/evp.h>    // Cryptographic operations

// Structure to store user information
// Each user has a unique salt and password hash
struct UserData {
    std::string salt;           // Random value added to password before hashing
    std::string passwordHash;    // The final hashed password
    
    UserData() = default;       // Default constructor
    UserData(std::string s, std::string h) : salt(s), passwordHash(h) {}
};

// Function to create a random salt
// Salt makes each password hash unique even if users have the same password
std::string generateSalt(size_t length = 16) {
    std::random_device rd;      // Hardware random number generator
    std::mt19937 gen(rd());     // Random number engine
    std::uniform_int_distribution<> dis(0, 255);    // Distribution range
    
    // Create salt string by generating random characters
    std::string salt;
    for (size_t i = 0; i < length; ++i) {
        salt += static_cast<char>(dis(gen));
    }
    return salt;
}

// Function to create a hash from password and salt
// This makes the password secure by converting it to an unreadable string
std::string hashPassword(const std::string& password, const std::string& salt) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();    // Create new hashing context
    const EVP_MD* md = EVP_sha256();        // Use SHA256 hashing algorithm
    
    unsigned char hash[EVP_MAX_MD_SIZE];     // Buffer for hash result
    unsigned int hashLen;

    // Perform the actual hashing
    EVP_DigestInit_ex(ctx, md, nullptr);
    EVP_DigestUpdate(ctx, salt.c_str(), salt.length());
    EVP_DigestUpdate(ctx, password.c_str(), password.length());
    EVP_DigestFinal_ex(ctx, hash, &hashLen);
    
    EVP_MD_CTX_free(ctx);    // Clean up

    // Convert hash to readable hexadecimal string
    std::stringstream ss;
    for(unsigned int i = 0; i < hashLen; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

int main() {
    // Store users in memory (username -> {salt, passwordHash})
    std::unordered_map<std::string, UserData> users;
    std::string username, password;
    char choice;
    // File for storing user data
    const std::string USER_FILE = "t3_hashes.txt";

    // Try to load existing users from file
    std::ifstream inFile(USER_FILE);
    if (inFile.is_open()) {
        std::string line, storedUsername, storedSalt, storedHash;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            if (ss >> storedUsername >> storedSalt >> storedHash) {
                users[storedUsername] = UserData(storedSalt, storedHash);
            }
        }
        inFile.close();
    }

    // Function to save users to file
    auto saveUsers = [&users, &USER_FILE]() {
        std::ofstream outFile(USER_FILE);
        if (outFile.is_open()) {
            for (const auto& user : users) {
                outFile << user.first << " " << user.second.salt << " " 
                       << user.second.passwordHash << "\n";
            }
            outFile.close();
        }
    };
    
    // Main program loop
    while (true) {
        // Show menu and get user choice
        std::cout << "1. Register\n2. Login\n3. Exit\nChoice: ";
        std::cin >> choice;
        std::cin.ignore();

        // Option 1: Register new user
        if (choice == '1') {
            // Get username and password
            std::cout << "Username: ";
            std::getline(std::cin, username);
            
            // Check if username already exists
            if (users.find(username) != users.end()) {
                std::cout << "Username already exists!\n";
                continue;
            }
            
            std::cout << "Password: ";
            std::getline(std::cin, password);

            // Create new user with unique salt and hashed password
            std::string salt = generateSalt();
            UserData userData = UserData(salt, hashPassword(password, salt));
            users[username] = userData;
            
            // Save users
            saveUsers();

            std::cout << "User registered successfully!\n";
        }
        // Option 2: Login existing user
        else if (choice == '2') {
            // Get username and password
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << "Password: ";
            std::getline(std::cin, password);

            // Save current users before login attempt
            saveUsers();

            // Verify memory users with file users
            std::unordered_map<std::string, UserData> fileUsers;
            std::ifstream verifyFile(USER_FILE);
            if (verifyFile.is_open()) {
                std::string line, storedUsername, storedSalt, storedHash;
                while (std::getline(verifyFile, line)) {
                    std::stringstream ss(line);
                    if (ss >> storedUsername >> storedSalt >> storedHash) {
                        fileUsers[storedUsername] = UserData(storedSalt, storedHash);
                    }
                }
                verifyFile.close();
            }

            // Verify that stored hash matches file hash
            if (fileUsers.find(username) == fileUsers.end() || 
                users[username].passwordHash != fileUsers[username].passwordHash) {
                std::cout << "Passwords do not match! Hash file might have been tampered with!\n";
                return 1;
            }

            // Check if user exists and verify password
            if (users.find(username) != users.end()) {
                const UserData& userData = users[username];
                if (hashPassword(password, userData.salt) == userData.passwordHash) {
                    std::cout << "Login successful!\n";
                } else {
                    std::cout << "Invalid credentials!\n";
                }
            } else {
                std::cout << "User not found!\n";
            }
        }
        // Option 3: Exit program
        else if (choice == '3') {
            break;
        }
    }
    return 0;
}

// Compile command
// g++ -std=c++11 -o t3 t3.cpp -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto