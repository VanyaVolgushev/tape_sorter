#pragma once
#include <cstddef>
#include <queue>
#include <vector>

#include "tape/itape_handle.h"

class KWayTapeSorter {
private:
    struct TapeValueCompare {
        bool operator()(std::pair<int32_t, ITapeHandle*> const& a,
                        std::pair<int32_t, ITapeHandle*> const& b) {
            return a.first > b.first;
        }
    };

    ITapeHandle& unsorted_tape_;
    ITapeHandle& out_tape_;
    std::vector<int32_t> buffer_;
    ITapeHandleFactory& buffer_tape_factory_;
    size_t max_buffer_size_;

    std::vector<std::unique_ptr<ITapeHandle>> temp_tapes_;

    bool FillBufferFromTape();
    void SortAndStoreBuffer();

public:
    KWayTapeSorter(ITapeHandle& unsorted_tape, ITapeHandle& out_tape,
                   ITapeHandleFactory& buffer_tape_factory, size_t max_buffer_size)
        : unsorted_tape_(unsorted_tape),
          out_tape_(out_tape),
          buffer_tape_factory_(buffer_tape_factory),
          max_buffer_size_(max_buffer_size) {};
    void Sort();
};
