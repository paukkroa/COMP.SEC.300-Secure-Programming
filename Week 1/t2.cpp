#include <iostream>
#include <cstring>

// Function which causes a off by one overflow
void causeOverflow() {
    char buffer[5] = {'A', 'B', 'C', 'D', '\0'};  // Buffer of size 5 (4 characters + null terminator)
    
    for (int i = 0; i <= 5; i++) {  // Off-by-one error: should be i < 4
        buffer[i] = 'X';  // Writes one byte past the buffer boundary on the last iteration
    }

    std::cout << "Buffer content: " << buffer << std::endl;
}

// Action which should be run after the overflow
void action() {
    std::cout << "Hello World!" << std::endl;
}

int main() {
    causeOverflow(); // Buffer content: XXXXXX (6 X's instead of 5)
    action();
    return 0;
}
