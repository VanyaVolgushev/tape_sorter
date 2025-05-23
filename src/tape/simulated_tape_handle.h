#pragma once
#include <fstream>

#include "itape_handle.h"
#include "delay_config.h"

class SimulatedTapeHandle : public ITapeHandle {
private:
    std::string filename_;
    std::fstream stream_;
    bool temporary_ = false;
    size_t max_size_;
    size_t caret_position_ = 0;
    DelayConfig config_;

    SimulatedTapeHandle(std::string const& filename, std::string const& config_path, size_t max_size);

public:
    int32_t Read() override;

    void Write(int32_t value) override;

    bool ShiftForward() override;

    bool ShiftBackward() override;

    void Rewind() override;

    SimulatedTapeHandle(SimulatedTapeHandle&&) noexcept;
    SimulatedTapeHandle& operator=(SimulatedTapeHandle&&) noexcept;

    // Creates a tape that reads and writes to an existing file
    static SimulatedTapeHandle CreateWithFile(std::string filename, std::string config_path, size_t max_size);

    // For buffer tapes, creates an empty file in ./tmp directory
    static SimulatedTapeHandle CreateTemp(std::string config_path, size_t max_size);

    // Creates a new empty file in current directory
    static SimulatedTapeHandle Create(std::string prefix, std::string config_path, size_t max_size);

    SimulatedTapeHandle(SimulatedTapeHandle const&) = delete;             // Deleted copy constructor
    SimulatedTapeHandle& operator=(SimulatedTapeHandle const&) = delete;  // Deleted copy assignment operator

    ~SimulatedTapeHandle();
};
