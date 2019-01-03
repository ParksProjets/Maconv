/*

Create a stream from a buffer.

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

#include <streambuf>
#include <memory>

namespace maconv {
namespace utils {


// A stream buffer from raw memory data.
struct RawDataStreamBuf : std::streambuf {

    RawDataStreamBuf() = default;
    RawDataStreamBuf(uint8_t *data, uint32_t length);

    // Set the data and length for this stream buffer.
    void SetData(uint8_t *data, uint32_t length);

protected:

    // Get the number of characters available for input.
    std::streamsize showmanyc() override;

    // Set the position indicator relative to some other position.
    pos_type seekoff(off_type off, std::ios_base::seekdir dir,
        std::ios_base::openmode which) override;

    // Set the position indicator to an absolute position.
    pos_type seekpos(pos_type pos, std::ios_base::openmode which) override;

    // Read count characters from the input sequence.
    std::streamsize xsgetn(char *p, std::streamsize n) override;

    // Write count characters to the output sequence.
    std::streamsize xsputn(const char *s, std::streamsize n) override;
};


// A stream buffer from a buffer.
struct BufferStreamBuf : RawDataStreamBuf {

    BufferStreamBuf(uint32_t length);

    std::unique_ptr<uint8_t[]> buffer;
};


} // namespace utils
} // namespace maconv
