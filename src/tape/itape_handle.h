#pragma once
#include <cstdint>

class ITapeHandle {
public:
    virtual int32_t Read() = 0;             // Read the pointed-to sector from the tape
    virtual void Write(int32_t value) = 0;  // Write to the pointed-to sector from the tape
    virtual bool ShiftForward() = 0;        // Move the tape forward a sector
    virtual bool ShiftBackward() = 0;       // Move the tape backward a sector
    virtual void Rewind() = 0;              // Rewind the tape to the beginning
    virtual ~ITapeHandle() = default;             // Virtual destructor
};
