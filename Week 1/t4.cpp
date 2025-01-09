#include <iostream>
#include <cstring>

// Function to be called via pointer
void function() {
    std::cout << "Function executed." << std::endl;
}

void functionPointerOverflow() {
    struct {
        char buffer[10]; // Small buffer
        void (*funcPtr)(); // Function pointer directly after buffer
    } data;

    data.funcPtr = function; // Initialize function pointer
    std::cout << "Function pointer before: " << (void*)data.funcPtr << std::endl;
    
    // Overflow buffer
    const char* input = "AAAAAAAAAAAAAAAA";
    std::strcpy(data.buffer, input);

    // Attempt to call the function via pointer (now corrupted)
    std::cout << "Function pointer after: " << (void*)data.funcPtr << std::endl;
    data.funcPtr();
}

int main() {
    functionPointerOverflow(); // Fails to call function

    // -- Example output --
    // Function pointer before: 0x102beefa8
    // Function pointer after: 0x102beef00
    // zsh: illegal hardware instruction  ./t4

    return 0;
}
