#pragma once
#include <cstddef>

#include "tape/itape_handle.h"


class KWayTapeSorter {
private:
    ITapeHandle& unsorted_tape_;
    ITapeHandle& out_tape_;
    size_t buffer_size_;

    bool FillBufferFromTape();

public:
    KWayTapeSorter(ITapeHandle& unsorted_tape, ITapeHandle& out_tape, size_t buffer_size) : 
        unsorted_tape_(unsorted_tape),
        out_tape_(out_tape),
        buffer_size_(buffer_size) {};
    void Sort();
};

