#include <filesystem>

#include <gtest/gtest.h>

#include "tape/simulated_tape_handle.h"

TEST(TapeTest, ReadWriteSingleElement) {
    auto tape_factory = SimulatedTapeFactory();
    auto tape = tape_factory.CreateTemp(3);
    tape->Write(42);
    tape->Rewind();
    EXPECT_EQ(tape->Read(), 42);
}

TEST(TapeTest, ReadWriteInSuccession) {
    size_t tape_size = 100;
    auto tape_factory = SimulatedTapeFactory();
    auto tape = tape_factory.CreateTemp(tape_size);
    for (int32_t i = 0; i < tape_size; ++i) {
        tape->Write(i);
        tape->ShiftForward();
    }
    tape->Rewind();
    for (int32_t i = 0; i < tape_size; ++i) {
        EXPECT_EQ(tape->Read(), i);
        if (i < tape_size - 1) {
            tape->ShiftForward();
        }
    }
}

TEST(TapeTest, ReadWriteNegative) {
    size_t tape_size = 100;
    auto tape_factory = SimulatedTapeFactory();
    auto tape = tape_factory.CreateTemp(tape_size);
    for (int32_t i = 0; i < tape_size; ++i) {
        tape->Write(-i);
        tape->ShiftForward();
    }
    tape->Rewind();
    for (int32_t i = 0; i < tape_size; ++i) {
        EXPECT_EQ(tape->Read(), -i);
        if (i < tape_size - 1) {
            tape->ShiftForward();
        }
    }
}
TEST(TapeTest, RightBoundaryTest) {
    size_t tape_size = 100;
    auto tape_factory = SimulatedTapeFactory();
    auto tape = tape_factory.CreateTemp(tape_size);
    for (int32_t i = 0; i < tape_size; ++i) {
        tape->Write(i);
        tape->ShiftForward();
    }
    EXPECT_FALSE(tape->ShiftForward());  // Should not be able to shift forward
}

TEST(TapeTest, LeftBoundaryTest) {
    size_t tape_size = 100;
    auto tape_factory = SimulatedTapeFactory();
    auto tape = tape_factory.CreateTemp(tape_size);
    for (int32_t i = 0; i < tape_size; ++i) {
        tape->Write(i);
        tape->ShiftForward();
    }
    tape->Rewind();
    EXPECT_FALSE(tape->ShiftBackward());  // Should not be able to shift backward
}
