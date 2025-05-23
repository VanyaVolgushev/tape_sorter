#include <iostream>
#include <string>
#include <tape/simulated_tape.h>

static size_t const MEM_LIMIT_BYTES = 1024 * 1024;  // 1 MB

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <unsorted tape name>" << " <output tape name>\n";
        return 1;
    }
    std::string in_file_name = argv[1];
    std::string out_file_name = argv[2];

    SimulatedTape tape = SimulatedTape::CreateWithFile(in_file_name, DelayConfig::GetConfigPath(),
                                                       MEM_LIMIT_BYTES);

    return 0;
}
