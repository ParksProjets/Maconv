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

#pragma once

#include <cstdint>
#include <ostream>
#include <string>

namespace maconv {
namespace fs {


// Helper class for writing data to a file.
struct FileWriter {

    FileWriter(std::ostream &stream);


    // Write some bytes.
    void Write(uint8_t *bytes, uint32_t length);

    // Fill |length| bytes of |byte|.
    void Fill(uint8_t byte, uint32_t length);


    // Write a single byte.
    void WriteByte(uint8_t byte);

    // Write a single short (16bits) (BE = big endian, LE = little endian).
    void WriteHalfBE(uint16_t half);
    void WriteHalfLE(uint16_t half);

    // Write a single word (32bits) (BE = big endian, LE = little endian).
    void WriteWordBE(uint32_t word);
    void WriteWordLE(uint32_t word);

    // Write a Macintosh date or a J2000 date (epoch on January 1, 2000).
    void WriteMacDate(time_t date);
    void WriteJ2000Date(time_t date);

    // Write a string.
    void WriteString(const std::string &str);


    std::ostream &stream; // Output stream for writing data.
};


} // namespace fs
} // namespace maconv
