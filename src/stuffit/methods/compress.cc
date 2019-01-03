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

#include "stuffit/methods/compress.h"

#include <make_unique.hpp>

namespace maconv {
namespace stuffit {



// Initialize the decoder.
void CompressLzw::Initialize(int max_symbols, int reserved_symbols)
{
    this->max_symbols = max_symbols;
    this->reserved_symbols = reserved_symbols;

    nodes = std::make_unique<CompressTreeNode[]>(max_symbols);
    for (int i = 0; i < 256; i++) {
		nodes[i].chr = i;
		nodes[i].parent = -1;
	}

    ClearTable();
}


// Clear the decoder table.
void CompressLzw::ClearTable()
{
    num_symbols = 256 + reserved_symbols;
    prev_symbol = -1;
    symbol_size = 9;
}



// The the first bytes corresponding to symbol in the LZW tree.
uint8_t CompressLzw::FindFirstByte(int symbol)
{
    while (nodes[symbol].parent >= 0)
        symbol = nodes[symbol].parent;
    return nodes[symbol].chr;
}


// Get the next symbol.
void CompressLzw::NextSymbol(int symbol)
{
    if (symbol > num_symbols || (prev_symbol < 0 && symbol == num_symbols))
        throw ExtractException("Compress: invalid code");

    int parent = prev_symbol;
    prev_symbol = symbol;

    if (parent < 0) return;
    int postfix_byte = FindFirstByte(symbol == num_symbols ? parent : symbol);

    if (num_symbols == max_symbols) // Too many symbols.
        return;

    nodes[num_symbols].parent = parent;
    nodes[num_symbols].chr = postfix_byte;
    num_symbols++;

    if (num_symbols != max_symbols && ((num_symbols & (num_symbols-1)) == 0))
        symbol_size++;
}



// Calculate the number of bytes needed to write the output.
int CompressLzw::CalcOutputLength()
{
	int n = 0;
	for (int symbol = prev_symbol; symbol >= 0; n++)
		symbol = nodes[symbol].parent;
	return n;
}


// Write the ouput data to a buffer.
void CompressLzw::OutputToBuffer(int len, uint8_t *buffer)
{
    int symbol = prev_symbol;
    buffer += len;

	while (symbol >= 0) {
		*(--buffer) = nodes[symbol].chr;
		symbol = nodes[symbol].parent;
	}
}




// Initialize the algorithm.
void CompressMethod::Initialize()
{
    block_mode = (flags & 0x80) != 0;
    lzw.Initialize(1 << (flags & 0x1F), block_mode ? 1 : 0);

    input.Load(data, end - data);
    output_len = -1;
    symbol_counter = 0;
}



// Load the next block of data.
bool CompressMethod::LoadNextBlock()
{
    int symbol;
    while (true) {
        if (input.HasEnded(lzw.symbol_size - 1)) return false;

        symbol = input.ReadWord(lzw.symbol_size);
        symbol_counter++;

        if (symbol != 256 || !block_mode) break;
        if (symbol_counter % 8)
            input.IgnoreBits(lzw.symbol_size * (8 - symbol_counter % 8));

        lzw.ClearTable();
        symbol_counter = 0;
    }

    lzw.NextSymbol(symbol);
    output_len = lzw.CalcOutputLength();
    return true;
}



// Read the next bytes.
int32_t CompressMethod::ReadBytes(uint8_t *data, uint32_t length)
{
    if (output_len == -1 && !LoadNextBlock())
        return -1;
    if (output_len > length)
        return 0;

    int32_t len = output_len;
    lzw.OutputToBuffer(output_len, data);

    output_len = -1;
    return len;
}



} // namespace stuffit
} // namespace maconv
