#include "kway_tape_sorter.h"

#include <algorithm>
#include <queue>

// Clears buffer and fills it with new data from the unsorted tape
// Returns false if reached end of tape
bool KWayTapeSorter::FillBufferFromTape() {
    if(unsorted_tape_.GetMaxSize() == 0) {
        return false;
    }
    buffer_.clear();
    buffer_.reserve(max_buffer_size_);
    while (buffer_.size() < max_buffer_size_) {
        int32_t value = unsorted_tape_.Read();
        buffer_.push_back(value);
        if (!unsorted_tape_.ShiftForward()) {
            return false;
        }
    }
    return true;
}

void KWayTapeSorter::SortAndStoreBuffer() {
    if(buffer_.empty()) {
        return;
    }
    std::sort(buffer_.begin(), buffer_.end());
    auto temp_tape = buffer_tape_factory_.CreateTemp(buffer_.size());
    for (auto const& value : buffer_) {
        temp_tape->Write(value);
        temp_tape->ShiftForward();
    }
    temp_tapes_.push_back(std::move(temp_tape));
};

void KWayTapeSorter::Sort() {
    while (FillBufferFromTape()) {
        SortAndStoreBuffer();
    }
    SortAndStoreBuffer();
    std::priority_queue<std::pair<int32_t, ITape*>,
                        std::vector<std::pair<int32_t, ITape*>>, TapeValueCompare>
            candidate_min_heap;
    for (auto& tape : temp_tapes_) {
        tape->Rewind();
        candidate_min_heap.push({tape->Read(), tape.get()});
    }
    while (!candidate_min_heap.empty()) {
        auto min_pair = candidate_min_heap.top();
        candidate_min_heap.pop();
        out_tape_.Write(min_pair.first);
        out_tape_.ShiftForward();
        if (min_pair.second->ShiftForward()) {
            candidate_min_heap.push({min_pair.second->Read(), min_pair.second});
        } else {
            
        }
    }
    
}
