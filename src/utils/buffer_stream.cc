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

#include "utils/buffer_stream.h"

namespace maconv {
namespace utils {



// "RawDataStreamBuf" constructor.
RawDataStreamBuf::RawDataStreamBuf(uint8_t *data, uint32_t length)
{
    SetData(data, length);
}


// Set the data and length for this stream buffer.
void RawDataStreamBuf::SetData(uint8_t *data, uint32_t length)
{
    char *start = reinterpret_cast<char *>(data);
    char *end = reinterpret_cast<char *>(data + length);
    setg(start, start, end);
    setp(start, end);
}



// Set the position indicator relative to some other position.
auto RawDataStreamBuf::seekoff(off_type off, std::ios_base::seekdir dir,
    std::ios_base::openmode which) -> pos_type
{
    // If dir is beginning or end: set an absolute position directly.
    switch (dir) {
        case std::ios_base::beg:
            return seekpos(off, which);            
        case std::ios_base::end:
            return seekpos(off + (egptr() - eback()), which);
    }

    // Set position from cursor (return twice if both in and out are set).
    if (which & std::ios_base::in)
        seekpos(off + (gptr() - eback()), std::ios_base::in);
    if (which & std::ios_base::out)
        seekpos(off + (pptr() - eback()), std::ios_base::out);
    return ((which & std::ios_base::in) ? gptr() : pptr()) - eback();
}


// Set the position indicator to an absolute position.
auto RawDataStreamBuf::seekpos(pos_type pos, std::ios_base::openmode which)
    -> pos_type
{
    if (which & std::ios_base::in)
        setg(eback(), eback() + pos, egptr());
    if (which & std::ios_base::out)
        pbump(pos - static_cast<pos_type>((pptr() - eback())));

    return pos;
}



// Get the number of characters available for input.
std::streamsize RawDataStreamBuf::showmanyc()
{
    return egptr() - gptr();
}


// Reads count characters from the input sequence.
std::streamsize RawDataStreamBuf::xsgetn(char *p, std::streamsize n)
{
    std::streamsize length = std::min(n, egptr() - gptr());
    std::copy(gptr(), gptr() + length, p);
    gbump(length);
    return length;
}


// Write count characters to the output sequence.
std::streamsize RawDataStreamBuf::xsputn(const char *p, std::streamsize n)
{
    std::streamsize length = std::min(n, epptr() - pptr());
    std::copy(p, p + length, pptr());
    pbump(length);
    return length;
}




// "BufferStreamBuf" constructor.
BufferStreamBuf::BufferStreamBuf(uint32_t length)
    : buffer{new uint8_t[length]}
{
    SetData(buffer.get(), length);
}


} // namespace utils
} // namespace maconv
