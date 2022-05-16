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

#pragma once

#include "fs/file.h"
#include "utils/buffer_stream.h"

#include <sstream>
#include <string>
#include <time.h>

namespace maconv {
namespace fs {


// Helper class for reading data from a file.
struct FileReader {

    FileReader(uint8_t *data, uint32_t size, std::string filename = "");
    FileReader(File &file);


    // Set the position in the stream.
    void Seek(uint32_t pos) { stream.seekg(pos); }
    void Seek(uint32_t off, std::ios_base::seekdir dir) { stream.seekg(off, dir); }

    // Get the absolute position.
    uint32_t Tell() { return stream.tellg(); }

    // Skip from reading a number of bytes.
    void Skip(uint32_t length) { stream.ignore(length); }


    // Read a single byte.
    uint8_t ReadByte();

    // Read a single short (16bits) (BE = big endian, LE = little endian).
    uint16_t ReadHalfBE();
    uint16_t ReadHalfLE();

    // Read a single word (32bits) (BE = big endian, LE = little endian).
    uint32_t ReadWordBE();
    uint32_t ReadWordLE();

    // Read a Macintosh date or a J2000 date (epoch on January 1, 2000).
    time_t ReadMacDate();
    time_t ReadJ2000Date();

    // Read a string of |size| (if no size, read until \0).
    std::string ReadString();
    std::string ReadString(uint32_t size);


    uint8_t *data; // Input data.
    uint32_t file_size; // Total size of the file.
    std::string filename; // Name of the file to read.

    utils::RawDataStreamBuf stream_buf; // Stream buffer from raw data buffer.
    std::istream stream; // Stream for reading data.
};



// Test a condtion and return false if the condition is false.
#define IS_COND(cond) if (!(cond)) return false


} // namespace fs
} // namespace maconv
