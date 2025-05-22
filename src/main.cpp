#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <unsorted>\n";
        return 1;
    }

    std::string input = argv[1];
    std::cout << "You passed in: " << input << "\n";
    return 0;
}
