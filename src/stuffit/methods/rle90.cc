/*

RLE 90 compression algorithm.

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

#include "stuffit/methods/rle90.h"

namespace maconv {
namespace stuffit {



// Initialize the algorithm.
void Rle90Method::Initialize()
{
    count = 0;
    byte = 0;
}



// Read the next byte.
uint8_t Rle90Method::ReadNextByte()
{
    // We have a byte to repeat.
    if (count != 0)
        return (count--, byte);

    // Read the next byte from the stream.
    uint8_t next = *(data++);
    if (next != 0x90)
        return (byte = next, byte);

    // The previous byte need to be repeated (if not 0x0).
    next = *(data++);
    if (next == 0x0)
        return (byte = next, byte);

    count = next - 2;
    return next;
}



// Read the next bytes.
int32_t Rle90Method::ReadBytes(uint8_t *buffer, uint32_t length)
{
    if (data == end && count == 0)
        return -1;

    uint8_t *start = buffer;
    uint8_t *end_capacity = buffer + length;

    while ((data != end || count != 0) && buffer != end_capacity)
        *(buffer++) = ReadNextByte();

    return buffer - start;
}



} // namespace stuffit
} // namespace maconv
