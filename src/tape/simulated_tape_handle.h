#pragma once
#include <fstream>

#include "itape_handle.h"
#include "delay_config.h"

class SimulatedTape : public ITape {
private:
    std::string filename_;
    std::fstream stream_;
    bool temporary_ = false;
    size_t max_size_;
    size_t caret_position_ = 0;
    DelayConfig config_;

    public:
    SimulatedTape(std::string const& filename, size_t max_size, bool temporary = false);
    
    int32_t Read() override;

    void Write(int32_t value) override;

    bool ShiftForward() override;

    bool ShiftBackward() override;

    void Rewind() override;

    bool PointingAtLast() const override {
        return caret_position_ + 1 >= max_size_;
    }
    
    virtual size_t GetMaxSize() const override {
        return max_size_;
    }

    std::string GetFilename() const {
        return filename_;
    }

    SimulatedTape(SimulatedTape&&) noexcept;
    SimulatedTape& operator=(SimulatedTape&&) noexcept;
    ~SimulatedTape();
};

class SimulatedTapeFactory : public ITapeFactory {
public:
    std::unique_ptr<ITape> CreateTemp(size_t max_size) const override;

    // Creates a new empty file in current directory
    std::unique_ptr<SimulatedTape> CreateNew(std::string prefix, size_t max_size) const;
};
