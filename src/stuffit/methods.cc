/*

All Stuffit compression methods.

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

#include "stuffit/methods.h"
#include "stuffit/methods/rle90.h"
#include "stuffit/methods/compress.h"
#include "stuffit/methods/algorithm13.h"
#include "stuffit/methods/arsenic.h"

#include <make_unique.hpp>
#include <stdlib.h>

namespace maconv {
namespace stuffit {



// Get a compression method (from a method number).
CompMethodPtr GetCompressionMethod(uint8_t method)
{
    switch (method) {
        case 0: // No compression.
            return std::make_unique<NoneMethod>();
        case 1: // RLE 90 algorithm.
            return std::make_unique<Rle90Method>();
        case 2: // Compress algorithm (LZW).
            return std::make_unique<CompressMethod>();
        case 13: // LZSS and Huffman.
            return std::make_unique<Algorithm13Method>();
        case 15: // Arsenic algorithm.
            return std::make_unique<ArsenicMethod>();
        default:
            return nullptr;
    }
}



// Extract data from the compressed fork.
void CompressionMethod::Extract(const StuffitCompInfo &info, uint8_t *data,
    std::vector<fs::DataPtr> &mem_pool)
{
    uint32_t capacity = info.size ? info.size : info.comp_size;
    this->data = data + info.offset;
    this->end = this->data + info.comp_size;

    Initialize();
    std::unique_ptr<uint8_t[]> buffer;

    buffer.reset((uint8_t *)malloc(capacity));
    total_size = 0;

    // Uncompress the data chunk by chunks.
    for (int32_t len = 0; true;) {
        len = ReadBytes(buffer.get() + total_size, capacity - total_size);
        if (len == -1)
            break;

        total_size += len;
        if (len == 0 || total_size == capacity) {
            capacity = (capacity * 3) / 2;
            buffer.reset((uint8_t *)realloc((void *)buffer.release(), capacity));
        }
    }

    // Store the uncompressed buffer in the mempool.
    uncompressed = buffer.get();
    mem_pool.emplace_back(std::move(buffer));
}



// Extract data from the compressed fork.
void NoneMethod::Extract(const StuffitCompInfo &info, uint8_t *data,
    std::vector<fs::DataPtr> &/* mem_pool */)
{
    total_size = info.size ? info.size : info.comp_size;
    uncompressed = data + info.offset;
}



} // namespace stuffit
} // namespace maconv
