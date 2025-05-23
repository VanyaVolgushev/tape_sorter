This C++ application does k-way sorting using storage devices that are called Tapes. A Tape has a caret that can read and write into the cell it is currently hovering over, and the caret can also move along the cells of the tape up and down. Each cell stores an int32_t. In this project the only class that implements the tape interface is SimulatedTape, an object that opens a .txt file and writes integers into the line the virtual caret is hovering over. Read, write and caret shift delays of SimulatedTape are defined in config.txt. The program is only allowed to store MAX_BUFFER_SIZE bytes of integers from tapes, so temporary tapes are created with corresponding .txt files to store intermediate sorting results.

Build instructions:

Run the following in the root project directory:

mkdir ./build
cd ./build
cmake ..
make

Launch instructions:

place config.txt and example_unsorted.txt next to the binary and run:

./tape_sorter example_unsorted.txt output.txt
