#include "simulated_tape.h"

#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>

namespace fs = std::filesystem;

static constexpr auto delay_read_ms = std::chrono::milliseconds(5);
static constexpr auto delay_write_ms = std::chrono::milliseconds(5);
static constexpr auto delay_shift_ms = std::chrono::milliseconds(1);
static constexpr auto delay_rewind_ms = std::chrono::milliseconds(100);

SimulatedTape::SimulatedTape(std::string const& filename, size_t max_size)
    : filename_(filename), max_size_(max_size) {
    if (max_size_ == 0) {
        throw std::invalid_argument("Max size must be greater than 0");
    }
    stream_.open(filename, std::ios::in | std::ios::out);
    if (!stream_.is_open()) {
        // File didn’t exist
        throw std::runtime_error("File " + filename + " not found");
    }
}

SimulatedTape::~SimulatedTape() {
    stream_.close();
    if (temporary_) fs::remove(filename_);
}

int32_t SimulatedTape::Read() {
    stream_.clear();
    stream_.seekg(0, std::ios::beg);
    for (size_t i = 0; i < caret_position_; ++i) {
        stream_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int32_t value;
    stream_ >> value;
    std::this_thread::sleep_for(delay_read_ms);
    return value;
}

void SimulatedTape::Write(int32_t value) {
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
    std::this_thread::sleep_for(delay_write_ms);
}

bool SimulatedTape::ShiftForward() {
    if (caret_position_ + 1 >= max_size_) {
        return false;
    }
    ++caret_position_;
    std::this_thread::sleep_for(delay_shift_ms);
    return true;
}

bool SimulatedTape::ShiftBackward() {
    if (caret_position_ == 0) {
        return false;
    }
    --caret_position_;
    std::this_thread::sleep_for(delay_shift_ms);
    return true;
}

void SimulatedTape::Rewind() {
    caret_position_ = 0;
    std::this_thread::sleep_for(delay_rewind_ms);
}

SimulatedTape::SimulatedTape(SimulatedTape&& other) noexcept
    : filename_(std::move(other.filename_)),
      stream_(std::move(other.stream_)),
      temporary_(other.temporary_),
      max_size_(other.max_size_),
      caret_position_(other.caret_position_) {
    // Reset the moved-from object
    other.temporary_ = false;
}

SimulatedTape SimulatedTape::CreateWithFile(std::string filename, size_t max_size) {
    return SimulatedTape(std::move(filename), max_size);
}

SimulatedTape SimulatedTape::CreateTemp(size_t max_size) {
    fs::create_directories("tmp");
    auto name = "tmp/tape_" +
                std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                ".txt";
    // Create an empty file
    std::ofstream(name).close();
    // Open the file for reading and writing using SimulatedTape
    SimulatedTape tape = SimulatedTape(name, max_size);
    tape.temporary_ = true;
    return tape;
}

SimulatedTape SimulatedTape::Create(std::string prefix, size_t max_size) {
    // Ensure a unique name
    static std::atomic<uint64_t> counter{0};
    auto id = counter++;
    auto name = prefix + "_" + std::to_string(id) + ".txt";
    auto tape = SimulatedTape(name, max_size);
    tape.temporary_ = true;
    return tape;
}

SimulatedTape& SimulatedTape::operator=(SimulatedTape&& other) noexcept {
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

        // Reset the moved-from object
        other.temporary_ = false;
    }
    return *this;
}
