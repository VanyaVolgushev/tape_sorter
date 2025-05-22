#pragma once

#include <fstream>

class ITape {
public:
    virtual int Read();            // Read the pointed-to sector from the tape
    virtual void Write(int value); // Write to the pointed-to sector from the tape
    virtual bool ShiftForward();   // Move the tape forward a sector
    virtual bool ShiftBackward();  // Move the tape backward a sector
    virtual void Rewind();         // Rewind the tape to the beginning
};
