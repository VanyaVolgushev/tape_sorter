#include <iostream>
#include <string>
#include <tape/simulated_tape_handle.h>

static size_t const MEM_LIMIT_BYTES = 1024 * 1024;  // 1 MB
static size_t const MAX_BUFFER_SIZE = MEM_LIMIT_BYTES / sizeof(int32_t);

int main(int argc, char** argv) {
    // Create in and out tapes
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <unsorted tape name>" << " <output tape name>\n";
        return 1;
    }
    std::string in_file_name = argv[1];
    std::string out_file_prefix = argv[2];

    SimulatedTapeHandle in_tape = SimulatedTapeHandle::CreateWithFile(
            in_file_name, DelayConfig::GetConfigPath(), MEM_LIMIT_BYTES);
    SimulatedTapeHandle out_tape =
            SimulatedTapeHandle::Create(out_file_prefix, DelayConfig::GetConfigPath(), MEM_LIMIT_BYTES);
    
    std::vector<int32_t> buffer;
    buffer.reserve(MAX_BUFFER_SIZE);
    size_t next_unbuffered = 0;

}
