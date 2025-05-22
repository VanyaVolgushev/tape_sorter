#pragma once
#include <fstream>

#include "itape.h"

class SimulatedTape : public ITape {
private:
    std::string filename_;
    std::fstream stream_;
    bool temporary_ = false;
    size_t max_size;
    size_t caret_position = 0;


    SimulatedTape(std::string const& filename);
    
    public:
    int32_t Read() override;

    void Write(int32_t value) override;

    bool ShiftForward() override;

    bool ShiftBackward() override;

    void Rewind() override;

    SimulatedTape(SimulatedTape&&) noexcept;
    SimulatedTape& operator=(SimulatedTape&&) noexcept;

    // Creates a tape that reads and writes to an existing file
    static SimulatedTape CreateWithFile(std::string filename);

    // For buffer tapes, creates an empty file in ./tmp directory
    static SimulatedTape CreateTemp();

    // Creates a new empty file in current directory
    static SimulatedTape Create();

    SimulatedTape(SimulatedTape const&) = delete;             // Deleted copy constructor
    SimulatedTape& operator=(SimulatedTape const&) = delete;  // Deleted copy assignment operator

    ~SimulatedTape();
};
