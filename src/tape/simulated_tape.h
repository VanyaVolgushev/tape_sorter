#pragma once
#include <fstream>

#include "itape.h"

class SimulatedTape : public ITape {
private:
    std::string filename_;
    std::fstream stream_;
    bool temporary_ = false;
    size_t max_size_;
    size_t caret_position_ = 0;

    SimulatedTape(std::string const& filename, size_t max_size);

public:
    int32_t Read() override;

    void Write(int32_t value) override;

    bool ShiftForward() override;

    bool ShiftBackward() override;

    void Rewind() override;

    SimulatedTape(SimulatedTape&&) noexcept;
    SimulatedTape& operator=(SimulatedTape&&) noexcept;

    // Creates a tape that reads and writes to an existing file
    static SimulatedTape CreateWithFile(std::string filename, size_t max_size);

    // For buffer tapes, creates an empty file in ./tmp directory
    static SimulatedTape CreateTemp(size_t max_size);

    // Creates a new empty file in current directory
    static SimulatedTape Create(std::string prefix, size_t max_size);

    SimulatedTape(SimulatedTape const&) = delete;             // Deleted copy constructor
    SimulatedTape& operator=(SimulatedTape const&) = delete;  // Deleted copy assignment operator

    ~SimulatedTape();
};
