# Tape Sorter

A C++ application that performs k-way sorting using virtual tape storage devices.

## Overview

The application simulates tape storage devices with these characteristics:
- Movable caret for reading and writing operations
- Cells store `int32_t` values
- Single cell access at a time
- Bi-directional caret movement along the tape

## Implementation Details

- `SimulatedTape` class implements tape functionality
- Tapes stored as .txt files
- Configurable delays for read/write and caret movement in `config.txt`
- Memory operations limited by `MAX_BUFFER_SIZE`
- Uses temporary tapes for intermediate sorting steps

## Build Instructions

```bash
mkdir ./build
cd ./build
cmake ..
make
```

## Running the Application

1. Place `config.txt` and input file in the binary's directory
2. Execute:
```bash
./tape_sorter example_unsorted.txt out
```
