#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "kway_tape_sorter.h"
#include "tape/simulated_tape_handle.h"

class TapeSorterTest : public ::testing::Test {
protected:
    static size_t const MAX_BUFFER_SIZE = 128;  // Buffer size for testing
    SimulatedTapeFactory tape_factory;
};

TEST_F(TapeSorterTest, EmptyTapeTest) {
    // Create empty input tape
    auto input_tape = tape_factory.CreateTemp(0);
    auto output_tape = tape_factory.CreateTemp(0);

    KWayTapeSorter sorter(*input_tape, *output_tape, tape_factory, MAX_BUFFER_SIZE);
    EXPECT_NO_THROW(sorter.Sort());

    // Verify output tape is also empty
    output_tape->Rewind();
    EXPECT_TRUE(output_tape->PointingAtLast());
}

TEST_F(TapeSorterTest, ShortTapeTest) {
    // Create input tape with 5 numbers
    std::vector<int32_t> input = {42, 15, 8, 23, 16};
    std::vector<int32_t> expected = {8, 15, 16, 23, 42};

    auto input_tape = tape_factory.CreateTemp(input.size());
    auto output_tape = tape_factory.CreateTemp(input.size());

    // Write input data
    for (auto value : input) {
        input_tape->Write(value);
        input_tape->ShiftForward();
    }
    input_tape->Rewind();

    // Sort
    KWayTapeSorter sorter(*input_tape, *output_tape, tape_factory, MAX_BUFFER_SIZE);
    sorter.Sort();

    auto name = static_cast<SimulatedTape*>(input_tape.get())->GetFilename();

    // Verify output
    output_tape->Rewind();
    std::vector<int32_t> result;
    do {
        result.push_back(output_tape->Read());
    } while (output_tape->ShiftForward());

    EXPECT_EQ(result, expected);
}

TEST_F(TapeSorterTest, LongRandomTapeTest) {
    // Create input tape with 400 random numbers
    size_t const tape_size = 400;
    std::vector<int32_t> input;
    input.reserve(tape_size);

    // Generate random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(-10000, 10000);

    for (size_t i = 0; i < tape_size; ++i) {
        input.push_back(dis(gen));
    }

    // Create expected sorted result
    std::vector<int32_t> expected = input;
    std::sort(expected.begin(), expected.end());

    // Create and fill input tape
    auto input_tape = tape_factory.CreateTemp(tape_size);
    auto output_tape = tape_factory.CreateTemp(tape_size);

    for (auto value : input) {
        input_tape->Write(value);
        input_tape->ShiftForward();
    }
    input_tape->Rewind();

    // Sort
    KWayTapeSorter sorter(*input_tape, *output_tape, tape_factory, MAX_BUFFER_SIZE);
    sorter.Sort();

    // Verify output
    output_tape->Rewind();
    std::vector<int32_t> result;
    do {
        result.push_back(output_tape->Read());
    } while (output_tape->ShiftForward());

    EXPECT_EQ(result, expected);
}