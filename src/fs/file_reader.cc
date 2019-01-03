/*

File Reader: helper class for reading data from a file.

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

#include "fs/file_reader.h"

#include <libhfs/data.h>

namespace maconv {
namespace fs {



// Constructor (with an istream).
FileReader::FileReader(uint8_t *d, uint32_t fs, std::string fn)
    : data{d}, file_size{fs}, filename{std::move(fn)},
      stream_buf{d, fs}, stream{&stream_buf}
{
}


// Constructor (with a MAC file).
FileReader::FileReader(File &file)
    : data{file.data}, file_size{file.data_size}, filename{file.filename},
      stream_buf{file.data, file.data_size}, stream{&stream_buf}
{
}



// Read a single byte.
uint8_t FileReader::ReadByte()
{
    return static_cast<uint8_t>(stream.get());
}


// Read a single half (16bits, big endian).
uint16_t FileReader::ReadHalfBE()
{
    uint8_t b[2];
    stream.read(reinterpret_cast<char *>(b), 2);
    return static_cast<int32_t>(b[1] | (b[0] << 8));
}

// Read a single half (16bits, little endian).
uint16_t FileReader::ReadHalfLE()
{
    uint16_t half;
    stream.read(reinterpret_cast<char *>(&half), 2);
    return half;
}


// Read a single word (32bits, big endian).
uint32_t FileReader::ReadWordBE()
{
    uint8_t b[4];
    stream.read(reinterpret_cast<char *>(b), 4);
    return static_cast<int32_t>(b[3] | (b[2] << 8) | (b[1] << 16) | (b[0] << 24));
}

// Read a single word (32bits, little endian).
uint32_t FileReader::ReadWordLE()
{
    uint32_t word;
    stream.read(reinterpret_cast<char *>(&word), 4);
    return word;
}


// Read a Macintosh date (epoch on January 1, 1904).
time_t FileReader::ReadMacDate()
{
    return d_ltime(ReadWordBE());
}

// Read a J2000 date (epoch on January 1, 2000).
time_t FileReader::ReadJ2000Date()
{
    return ReadMacDate() + 3029529600UL;
}


// Read a string (read until \0).
std::string FileReader::ReadString()
{
    std::string result;
    char c;

    while (c = stream.get(), c != '\0')
        result += c;
    return result;
}


// Read a string of |size|.
std::string FileReader::ReadString(uint32_t size)
{
    std::string result(size, '\0');
    stream.read(&result[0], size);
    return result;
}



} // namespace fs
} // namespace maconv
