/*

File Reader: helper class for writing data to a file.

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

#include "fs/file_writer.h"

#include <libhfs/data.h>

namespace maconv {
namespace fs {



// Constructor.
FileWriter::FileWriter(std::ostream &s)
    : stream{s}
{
}



// Write some bytes.
void FileWriter::Write(uint8_t *bytes, uint32_t length)
{
    stream.write(reinterpret_cast<char *>(bytes), length);
}


// Fill |length| bytes of |byte|.
void FileWriter::Fill(uint8_t byte, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
        stream.put(byte);
}



// Write a single byte.
void FileWriter::WriteByte(uint8_t byte)
{
    stream.put(byte);
}


// Write a single short (16bits, big endian).
void FileWriter::WriteHalfBE(uint16_t half)
{
    uint8_t b[2] = {
        static_cast<uint8_t>(half >> 8),
        static_cast<uint8_t>(half & 0xFF)
    };

    stream.write(reinterpret_cast<char *>(b), 2);
}

// Write a single short (16bits, little endian).
void FileWriter::WriteHalfLE(uint16_t half)
{
    stream.write(reinterpret_cast<char *>(&half), 2);
}


// Write a single word (32bits, big endian).
void FileWriter::WriteWordBE(uint32_t w)
{
    uint8_t b[4] = {
        static_cast<uint8_t>(w >> 24),
        static_cast<uint8_t>((w >> 16) & 0xFF),
        static_cast<uint8_t>((w >> 8) & 0xFF),
        static_cast<uint8_t>(w & 0xFF)
    };

    stream.write(reinterpret_cast<char *>(b), 4);
}

// Write a single word (32bits, little endian).
void FileWriter::WriteWordLE(uint32_t word)
{
    stream.write(reinterpret_cast<char *>(&word), 4);
}


// Write a Macintosh date (epoch on January 1, 1904).
void FileWriter::WriteMacDate(time_t date)
{
    WriteWordLE(d_mtime(date));
}

// Write a J2000 date (epoch on January 1, 2000).
void FileWriter::WriteJ2000Date(time_t date)
{
    return WriteMacDate(date - 3029529600UL);
}


// Write a string.
void FileWriter::WriteString(const std::string &str)
{
    stream << str;
}



} // namespace fs
} // namespace maconv
