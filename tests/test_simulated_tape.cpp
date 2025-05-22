#include <gtest/gtest.h>
#include "tape/simulated_tape.h"

TEST(TapeTest, ReadWriteSingleElement) {
    SimulatedTape tape("tmp/test_tape.bin");
    tape.Write(42);
    tape.Rewind();
    EXPECT_EQ(tape.Read(), 42);
}
