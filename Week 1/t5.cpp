#include <iostream>
#include <cstdio>
#include <cstring>

void printUserInput(const char* userInput) {
    printf(userInput);  // Print user input without safety checks
}

int main() {
    char buffer[100];
    
    std::cout << "Enter text: ";
    std::cin.getline(buffer, sizeof(buffer));

    printUserInput(buffer);  // Pass user input to vulnerable function

    // Example output:
    //
    // Enter text: AAAAA %x  
    // AAAAA 6d5231e4%  

    return 0;
}
