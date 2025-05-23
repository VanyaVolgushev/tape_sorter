#include "simulated_tape_handle.h"

#include <atomic>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>

namespace fs = std::filesystem;

SimulatedTapeHandle::SimulatedTapeHandle(std::string const& filename, size_t max_size,
                                         bool temporary)
    : filename_(filename),
      temporary_(temporary),
      max_size_(max_size),
      config_(DelayConfig::GetConfigPath()) {
    if (max_size_ == 0) {
        throw std::invalid_argument("Max size must be greater than 0");
    }
    stream_.open(filename, std::ios::in | std::ios::out);
    if (!stream_.is_open()) {
        // File didn’t exist
        throw std::runtime_error("File " + filename + " not found");
    }
}

SimulatedTapeHandle::~SimulatedTapeHandle() {
    stream_.close();
    if (temporary_) fs::remove(filename_);
}

int32_t SimulatedTapeHandle::Read() {
    stream_.clear();
    stream_.seekg(0, std::ios::beg);
    for (size_t i = 0; i < caret_position_; ++i) {
        stream_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int32_t value;
    stream_ >> value;
    std::this_thread::sleep_for(config_.read_delay);
    return value;
}

void SimulatedTapeHandle::Write(int32_t value) {
    stream_.clear();
    stream_.seekg(0, std::ios::beg);
    std::ostringstream buffer;

    // Read all existing values into a buffer
    for (size_t i = 0; i < caret_position_; ++i) {
        std::string line;
        std::getline(stream_, line);
        buffer << line << '\n';
    }

    // Write the new value at the caret position
    buffer << value << '\n';

    // Skip the current value in the file
    std::string line;
    std::getline(stream_, line);

    // Append the remaining values from the file
    while (std::getline(stream_, line)) {
        buffer << line << '\n';
    }

    // Rewrite the file with the updated content
    stream_.close();
    std::ofstream out(filename_, std::ios::trunc);
    out << buffer.str();
    out.close();

    // Reopen the file for reading and writing
    stream_.open(filename_, std::ios::in | std::ios::out);

    // Simulate write delay
    std::this_thread::sleep_for(config_.write_delay);
}

bool SimulatedTapeHandle::ShiftForward() {
    if (caret_position_ + 1 >= max_size_) {
        return false;
    }
    ++caret_position_;
    std::this_thread::sleep_for(config_.shift_delay);
    return true;
}

bool SimulatedTapeHandle::ShiftBackward() {
    if (caret_position_ == 0) {
        return false;
    }
    --caret_position_;
    std::this_thread::sleep_for(config_.shift_delay);
    return true;
}

void SimulatedTapeHandle::Rewind() {
    caret_position_ = 0;
    std::this_thread::sleep_for(config_.rewind_delay);
}

SimulatedTapeHandle::SimulatedTapeHandle(SimulatedTapeHandle&& other) noexcept
    : filename_(std::move(other.filename_)),
      stream_(std::move(other.stream_)),
      temporary_(other.temporary_),
      max_size_(other.max_size_),
      caret_position_(other.caret_position_),
      config_(std::move(other.config_)) {
    // Reset the moved-from object
    other.temporary_ = false;
}

SimulatedTapeHandle& SimulatedTapeHandle::operator=(SimulatedTapeHandle&& other) noexcept {
    if (this != &other) {
        // Close the current stream if open
        if (stream_.is_open()) {
            stream_.close();
        }

        // Move resources from the other object
        filename_ = std::move(other.filename_);
        stream_ = std::move(other.stream_);
        temporary_ = other.temporary_;
        max_size_ = other.max_size_;
        caret_position_ = other.caret_position_;
        config_ = std::move(other.config_);

        other.temporary_ = false;
    }
    return *this;
}

std::unique_ptr<ITapeHandle> SimulatedTapeHandleFactory::CreateTemp(size_t max_size) const {
    fs::create_directories("tmp");
    auto name = "tmp/tape_" +
                std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                ".txt";
    std::ofstream(name).close();  // Create an empty file
    return std::make_unique<SimulatedTapeHandle>(name, max_size, true);
}

std::unique_ptr<SimulatedTapeHandle> SimulatedTapeHandleFactory::CreateNew(std::string prefix,
                                                                   size_t max_size) const {
    static std::atomic<uint64_t> counter{0};
    auto id = counter++;
    auto name = prefix + "_" + std::to_string(id) + ".txt";
    std::ofstream(name).close();  // Create an empty file
    return std::make_unique<SimulatedTapeHandle>(name, max_size);
}
