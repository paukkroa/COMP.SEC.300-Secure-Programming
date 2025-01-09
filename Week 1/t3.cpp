#include <iostream>
#include <cstring>

// Function which causes heap overflow
void causeOverflow() {
    char* buffer = new char[10];  // Dynamically allocate 10 bytes on the heap

    const char* largeInput = "Overflow here I come :D";
    
    // Unsafe copy, causes heap overflow
    std::strcpy(buffer, largeInput);  // Copies more than 10 bytes into a 10-byte buffer
    
    std::cout << "Buffer content: " << buffer << std::endl;

    delete[] buffer;  // Clean up the allocated memory
}

// Action which should be run after the overflow
void action() {
    std::cout << "Hello World!" << std::endl;
}

int main() {
    causeOverflow(); // Causes overflow but program continues
    action(); // Action runs after the overflow
    return 0;
}
