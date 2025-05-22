#include <gtest/gtest.h>

#include "tape/simulated_tape.h"

TEST(TapeTest, ReadWriteSingleElement) {
    SimulatedTape tape = SimulatedTape::CreateTemp();
    tape.Write(42);
    tape.Rewind();
    EXPECT_EQ(tape.Read(), 42);
}

TEST(TapeTest, ReadWriteInSuccession) {
    SimulatedTape tape = SimulatedTape::CreateTemp();
    for (int32_t i = 0; i < 100; ++i) {
        tape.Write(i);
        tape.ShiftForward();
    }
    tape.Rewind();
    for (int32_t i = 0; i < 100; ++i) {
        EXPECT_EQ(tape.Read(), i);
        if (i < 99) {
            tape.ShiftForward();
        }
    }
}
