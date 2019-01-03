/*

Huffman decoder.

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

#include "stuffit/utils/huffman.h"
#include "stuffit/methods.h"

#include <make_unique.hpp>
#include <limits>

namespace maconv {
namespace stuffit {



// Reverse a 32bits integer.
static uint32_t Reverse32(uint32_t val)
{
	val = ((val >> 1) & 0x55555555) | ((val & 0x55555555) << 1);
	val = ((val >> 2) & 0x33333333) | ((val & 0x33333333) << 2);
	val = ((val >> 4) & 0x0F0F0F0F) | ((val & 0x0F0F0F0F) << 4);
	val = ((val >> 8) & 0x00FF00FF) | ((val & 0x00FF00FF) << 8);
	return (val >> 16) | (val << 16);
}


// Reverse a Nbits integer.
static uint32_t ReverseN(uint32_t val, int length)
{
	return Reverse32(val) >> (32 - length);
}



// Copy an Huffman decoder.
HuffmanDecoder &HuffmanDecoder::operator=(const HuffmanDecoder &other)
{
    tree = other.tree;
    num_entries = other.num_entries;
    min_length = other.min_length;
    max_length = other.max_length;
    return *this;
}


// Initialize the decoder.
void HuffmanDecoder::Initialize()
{
    NewNode();
    num_entries = 1;

    min_length = std::numeric_limits<int>::max();
    max_length = std::numeric_limits<int>::min();
}


// Initialize the decoder (with some lengths).
void HuffmanDecoder::Initialize(const int *lengths, int numsymbols, int maxcodelength,
    bool zeros)
{
    Initialize();
    int code = 0, symbolsleft = numsymbols;

    for (int length = 1; length <= maxcodelength; length++) {
        for (int i = 0; i < numsymbols; i++) {
            if (lengths[i] != length) continue;

            AddValue(i, zeros ? code : ~code, length);
            code++;

            if (--symbolsleft == 0) return; // Early exit if all codes have been handled.
        }

        code <<= 1;
    }
}



// Add a new value in the decoder.
void HuffmanDecoder::AddValue(int value, uint32_t code, int length)
{
    AddValue(value, code, length, length);
}


// Add a new value in the decoder.
void HuffmanDecoder::AddValue(int value, uint32_t code, int length, int repeat_pos)
{
    if (length > max_length) max_length = length;
    if (length < min_length) min_length = length;

    repeat_pos = length - 1 - repeat_pos;
    int last_node = 0;

    int codest = ((code >> (repeat_pos - 1)) & 3);
    if (repeat_pos == 0 || (repeat_pos >= 0 && (codest == 0 || codest == 3)))
        throw ExtractException("Huffman: invalid repeat position");

    for (int bitpos = length - 1; bitpos >= 0; bitpos--) {
        int bit = (code >> bitpos) & 1;

        if (IsLeafNode(last_node))
            throw ExtractException("Huffman: prefix already exists");

        if (bitpos == repeat_pos) {
            if (!IsOpenBranch(last_node, bit))
                throw ExtractException("Huffman: invalid repeating code");

            int repeat_node = NewNode();
            int next_node = NewNode();

            SetBranch(last_node, bit, repeat_node);
            SetBranch(repeat_node, bit, repeat_node);
            SetBranch(repeat_node, bit ^ 1, next_node);
            last_node = next_node;

            bitpos++; // Terminating bit already handled, skip it.
        }
        else {
            if (IsOpenBranch(last_node, bit))
                SetBranch(last_node, bit, NewNode());
            last_node = Branch(last_node, bit);
        }
    }

    if (!IsEmptyNode(last_node))
        throw ExtractException("Huffman: prefix already exists");
	SetLeafValue(last_node, value);
}



// Add a new value in the decoder (low bit first).
void HuffmanDecoder::AddValueLF(int value, uint32_t code, int length)
{
    AddValue(value, ReverseN(code, length), length, length);
}


// Add a new value in the decoder (low bit first).
void HuffmanDecoder::AddValueLF(int value, uint32_t code, int length, int repeat_pos)
{
    AddValue(value, ReverseN(code, length), length, repeat_pos);
}



// Get the next symbol from a bit reader.
int HuffmanDecoder::NextSymbol(utils::BitReader &input)
{
    if (!table)
        throw ExtractException("Huffman: search table not built");

	int bits = input.ReadWord(table_size, false);
	int length = table[bits].length;
	int value = table[bits].value;

	if (length < 0)
        throw ExtractException("Huffman: invalid prefix code when getting next symbol [length]");

	if (length <= table_size) {
        input.SkipBits(length);
		return value;
	}

    input.SkipBits(table_size);
	int node = value;

	for (int bit; !IsLeafNode(node); node = Branch(node, bit)) {
		bit = input.ReadBit();
		if (IsOpenBranch(node, bit))
            throw ExtractException("Huffman: invalid prefix code when getting next symbol [code]");
	}

	return LeafValue(node);
}



// Make search table (Little Endian).
void HuffmanDecoder::MakeTableRecursLE(int node, HuffmanTableEntry *table,
    int depth)
{
    int curr_table_size = (1 << (table_size - depth));
	int curr_stride = (1 << depth);

	if (IsLeafNode(node)) {
		for (int i = 0; i < curr_table_size; i++) {
			table[i * curr_stride].length = depth;
			table[i * curr_stride].value = LeafValue(node);
		}
	}
	else if (IsInvalidNode(node)) {
		for (int i = 0; i < curr_table_size; i++)
            table[i * curr_stride].length = -1;
	}
	else {
		if (depth == table_size) {
			table[0].length = table_size + 1;
			table[0].value = node;
		} else {
			MakeTableRecursLE(LeftBranch(node), table, depth + 1);
			MakeTableRecursLE(RightBranch(node), table + curr_stride, depth + 1);
		}
	}
}


// Make search table (Big Endian).
void HuffmanDecoder::MakeTableRecursBE(int node, HuffmanTableEntry *table,
    int depth)
{
    int curr_table_size = (1 << (table_size - depth));

	if (IsInvalidNode(node)) {
		for (int i = 0; i < curr_table_size; i++)
            table[i].length = -1;
	}
	else if (IsLeafNode(node)) {
		for(int i = 0; i < curr_table_size; i++) {
			table[i].length = depth;
			table[i].value = LeafValue(node);
		}
	}
	else {
		if (depth == table_size) {
			table[0].length = table_size + 1;
			table[0].value = node;
		} else {
			MakeTableRecursBE(LeftBranch(node), table, depth + 1);
			MakeTableRecursBE(RightBranch(node), table + curr_table_size/2, depth + 1);
		}
	}
}


// Make the search table.
void HuffmanDecoder::MakeTable(bool is_LE)
{
    constexpr int kMaxTableSize = 10;

    if (max_length < min_length) table_size = kMaxTableSize;
	else if (max_length >= kMaxTableSize) table_size = kMaxTableSize;
	else table_size = max_length;

    table = std::make_unique<HuffmanTableEntry[]>(1 << table_size);

    if (is_LE) MakeTableRecursLE(0, table.get(), 0);
    else MakeTableRecursBE(0, table.get(), 0);
}



} // namespace stuffit
} // namespace maconv
