#pragma once
#include <cstdint>
#include <memory>

class ITape {
public:
    virtual int32_t Read() = 0;             // Read the pointed-to sector from the tape
    virtual void Write(int32_t value) = 0;  // Write to the pointed-to sector from the tape
    virtual bool ShiftForward() = 0;        // Move the tape forward a sector
    virtual bool ShiftBackward() = 0;       // Move the tape backward a sector
    virtual void Rewind() = 0;              // Rewind the tape to the beginning
    virtual bool EndOfTape() const = 0;     // Check if the tape has reached the end
    virtual ~ITape() = default;

    ITape() = default;
    ITape(ITape const&) = delete;             // Deleted copy constructor
    ITape& operator=(ITape const&) = delete;  // Deleted copy assignment operator
};

// Might be overkill for this project, but in theory there could be all kinds of tape handles
// (network, etc.)
class ITapeFactory {
public:
    virtual std::unique_ptr<ITape> CreateTemp(size_t max_size) const = 0;
    virtual ~ITapeFactory() = default;
};
