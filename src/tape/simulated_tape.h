#pragma once
#include "itape.h"

class SimulatedTape : public ITape {
private:
    bool temporary = false;

    // Constructs a tape reader that reads and writes from an existing file
    SimulatedTape(std::string const& filename);
    ~SimulatedTape();

public:
    int Read() override;

    void Write(int value) override;

    bool ShiftForward() override;

    bool ShiftBackward() override;

    void Rewind() override;

    // Creates a tape that reads and writes to an existing file
    static SimulatedTape CreateWithFile(std::string filename);

    // For buffer tapes, creates an empty file in ./tmp directory
    static SimulatedTape CreateTemp();

    // Creates a new empty file in current directory
    static SimulatedTape Create();
};
