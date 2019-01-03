/*

Stuffit algorithm 13: LZSS and Huffman.

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
#include "stuffit/utils/huffman.h"

#include <memory>

namespace maconv {
namespace stuffit {


// Algorithm 13 compression algorithm.
struct Algorithm13Method : CompressionMethod {

    // Intialize LZSS.
    void InitializeLZSS();

    // Parse Huffman code given in data.
    void ParseHuffmanCode(HuffmanDecoder &decoder, int numcodes,
        HuffmanDecoder &metacode);

    // Get the next litteral or offset.
    int NextLiteralOrOffset(int &offset, int &length);


    // Initialize the algorithm.
    void Initialize() override;

    // Read the next byte.
    int32_t ReadNextByte(uint32_t pos);

    // Read the next bytes.
    int32_t ReadBytes(uint8_t *data, uint32_t length) override;


    bool ended;
    utils::BitReaderLE input;

    std::unique_ptr<uint8_t[]> window_buffer;
	int match_length, match_offset;

    HuffmanDecoder firstcode, secondcode, offsetcode;
	HuffmanDecoder *currcode;
};


} // namespace stuffit
} // namespace maconv
