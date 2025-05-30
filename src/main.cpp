#include <iostream>
#include <string>
#include <filesystem>

#include "kway_tape_sorter.h"
#include "tape/simulated_tape_handle.h"

static size_t const MEM_LIMIT_BYTES = 512;  // 0.5 KB
static size_t const MAX_BUFFER_SIZE = MEM_LIMIT_BYTES / sizeof(int32_t);

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <unsorted tape name>" << " <output tape prefix>\n";
        return 1;
    }
    if(!fs::exists(argv[1])) {
        std::cerr << "Error: Input file does not exist.\n";
        return 1;
    }
    std::string in_file_name = argv[1];
    std::string out_file_prefix = argv[2];

    std::ifstream in_file(in_file_name);
    size_t in_file_lines = std::count(std::istreambuf_iterator<char>(in_file),
                                     std::istreambuf_iterator<char>(), '\n');
    in_file.close();

    SimulatedTape in_tape = SimulatedTape(in_file_name, in_file_lines);
    SimulatedTapeFactory sim_tape_factory;
    auto out_tape_ptr = sim_tape_factory.CreateNew(out_file_prefix, MEM_LIMIT_BYTES);

    KWayTapeSorter sorter(in_tape, *out_tape_ptr, sim_tape_factory, MAX_BUFFER_SIZE);
    sorter.Sort();
    std::cout << "Sorting completed. Output written to " << out_tape_ptr->GetFilename() << ".\n";
}
