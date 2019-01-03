/*

Compress Algorithm (LZW, block mode).

The code in this file is based on TheUnarchiver.
See README.md and docs/licenses/TheUnarchiver.txt for more information.

Copyright (C) 2019, Guillaume Gonnet

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#pragma once

#include "stuffit/methods.h"
#include "utils/bit_reader.h"

#include <memory>

namespace maconv {
namespace stuffit {


struct CompressTreeNode {
    uint8_t chr;
    int parent;
};


// LZW (Lempel-Ziv-Welch) decoder.
struct CompressLzw {

    // Initialize the decoder.
    void Initialize(int max_symbols,int reserved_symbols);

    // Clear the decoder table.
    void ClearTable();


    // The the first bytes corresponding to symbol in the LZW tree.
    uint8_t FindFirstByte(int symbol);

    // Read the next symbol.
    void NextSymbol(int symbol);


    // Calculate the number of bytes needed to write the output.
    int CalcOutputLength();

    // Write the ouput data to a buffer.
    void OutputToBuffer(int len, uint8_t *buffer);


    int num_symbols, max_symbols, reserved_symbols;
    int prev_symbol;
    int symbol_size;

    std::unique_ptr<CompressTreeNode[]> nodes;
};



// Compress algorithm.
struct CompressMethod : CompressionMethod {

    CompressMethod(int flags_ = 0x8E) : flags{flags_} {}


    // Initialize the algorithm.
    void Initialize() override;

    // Load the next block of data.
    bool LoadNextBlock();

    // Read the next bytes.
    int32_t ReadBytes(uint8_t *data, uint32_t length) override;


    int flags;
    bool block_mode;

    int symbol_counter;
	int output_len;

    utils::BitReaderLE input;
    CompressLzw lzw;
};


} // namespace stuffit
} // namespace maconv
