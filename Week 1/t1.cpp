#include <iostream>
#include <cstring>

// Function which causes a buffer overflow
void causeOverflow() {
    char buffer[10];  // Buffer of size 10 bytes
    std::cout << "Enter a string: ";
    std::cin >> buffer; // This might cause a buffer overflow

    std::cout << "Buffer content: " << buffer << std::endl;
}

// Action which should be run after the overflow
void action() {
    std::cout << "Hello World!" << std::endl;
}

int main() {
    causeOverflow(); // This will cause our program to abrutly exit
    action();
    return 0;
}
