#pragma once
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>
#include <filesystem>

class DelayConfig {
public:
    explicit DelayConfig(std::string const& configPath) {
        std::ifstream config(configPath);
        if (!config.is_open()) {
            throw std::runtime_error("Could not open config file: " + configPath);
        }

        int read_ms, write_ms, shift_ms, rewind_ms;
        config >> read_ms >> write_ms >> shift_ms >> rewind_ms;

        read_delay = std::chrono::milliseconds(read_ms);
        write_delay = std::chrono::milliseconds(write_ms);
        shift_delay = std::chrono::milliseconds(shift_ms);
        rewind_delay = std::chrono::milliseconds(rewind_ms);
    }

    std::chrono::milliseconds read_delay;
    std::chrono::milliseconds write_delay;
    std::chrono::milliseconds shift_delay;
    std::chrono::milliseconds rewind_delay;

    static std::string GetConfigPath() {
        return (std::filesystem::current_path()/"config.txt").string();
    }
};
