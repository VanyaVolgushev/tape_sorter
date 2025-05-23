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

    public:
    SimulatedTapeHandle(std::string const& filename, size_t max_size, bool temporary = false);
    
    int32_t Read() override;

    void Write(int32_t value) override;

    bool ShiftForward() override;

    bool ShiftBackward() override;

    void Rewind() override;

    bool EndOfTape() const override {
        return caret_position_ >= max_size_;
    }

    std::string GetFilename() const {
        return filename_;
    }

    SimulatedTapeHandle(SimulatedTapeHandle&&) noexcept;
    SimulatedTapeHandle& operator=(SimulatedTapeHandle&&) noexcept;
    ~SimulatedTapeHandle();
};

class SimulatedTapeHandleFactory : public ITapeHandleFactory {
public:
    std::unique_ptr<ITapeHandle> CreateTemp(size_t max_size) const override;

    // Creates a new empty file in current directory
    std::unique_ptr<SimulatedTapeHandle> CreateNew(std::string prefix, size_t max_size) const;
};
