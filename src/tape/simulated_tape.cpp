#include "simulated_tape.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <exception>

namespace fs = std::filesystem;

static constexpr auto delay_read_ms = std::chrono::milliseconds(5);
static constexpr auto delay_write_ms = std::chrono::milliseconds(5);
static constexpr auto delay_shift_ms = std::chrono::milliseconds(1);
static constexpr auto delay_rewind_ms = std::chrono::milliseconds(100);

SimulatedTape::SimulatedTape(std::string const& filename) : filename_(filename) {
    stream_.open(filename, std::ios::binary | std::ios::in | std::ios::out);
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
    int32_t value;
    stream_.read(reinterpret_cast<char*>(&value), sizeof(value));
    std::this_thread::sleep_for(delay_read_ms);
    return value;
}

void SimulatedTape::Write(int32_t value) {
    stream_.write(reinterpret_cast<char*>(&value), sizeof(value));
    // Simulate write delay
    std::this_thread::sleep_for(delay_write_ms);
}

bool SimulatedTape::ShiftForward() {
    stream_.seekg(sizeof(int32_t), std::ios::cur);
    stream_.seekp(sizeof(int32_t), std::ios::cur);
    std::this_thread::sleep_for(delay_shift_ms);
    return static_cast<bool>(stream_);
}

bool SimulatedTape::ShiftBackward() {
    stream_.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
    stream_.seekp(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
    std::this_thread::sleep_for(delay_shift_ms);
    return static_cast<bool>(stream_);
}

void SimulatedTape::Rewind() {
    stream_.seekg(0);
    stream_.seekp(0);
    std::this_thread::sleep_for(delay_rewind_ms);
}

SimulatedTape SimulatedTape::CreateWithFile(std::string filename) {
    return SimulatedTape(std::move(filename));
}

SimulatedTape SimulatedTape::CreateTemp() {
    fs::create_directories("tmp");
    auto name = "tmp/tape_" +
                std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                ".bin";
    // Create an empty file
    std::ofstream(name, std::ios::binary).close();
    // Open the file for reading and writing using SimulatedTape
    SimulatedTape tape = SimulatedTape(name);
    tape.temporary_ = true;
    return tape;
}

SimulatedTape SimulatedTape::Create() {
    auto name = "tape_" +
                std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                ".bin";
    auto tape = SimulatedTape(name);
    tape.temporary_ = true;
    return tape;
}
SimulatedTape::SimulatedTape(SimulatedTape&& other) noexcept
    : filename_(std::move(other.filename_)),
      stream_(std::move(other.stream_)),
      temporary_(other.temporary_) {
    // Reset the moved-from object
    other.temporary_ = false;
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

        // Reset the moved-from object
        other.temporary_ = false;
    }
    return *this;
}
