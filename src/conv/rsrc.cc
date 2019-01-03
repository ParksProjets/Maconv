/*

Convert files from and to rsrc (raw) format.

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

#include "conv/converters.h"

#include <path.hpp>

namespace maconv {
namespace conv {



// Get the name of the other file.
static bool GetOtherName(const std::string &input, std::string &other)
{
    bool is_res = input.size() >= 4 && input.compare(input.size() - 4, 4, ".rsrc") == 0;

    if (is_res)
        other = input.substr(0, input.size() - 4);
    else
        other = input + ".rsrc";

    return is_res;
}



// Read "rsrc" from two files (data and ressource).
static void ReadRsrcDouble(UnPacked &u, const std::string &other, bool is_res)
{
    uint32_t size = ReadLocalFile(other, u.d2);

    if (is_res) {
        u.file.data = u.d2.get();
        u.file.data_size = size;
    } else {
        u.file.res = u.d2.get();
        u.file.res_size = size;
    }
}


// Read rsrc (raw) file(s).
bool ReadRsrc(fs::FileReader &reader, UnPacked &u)
{
    u.file.Reset();

    // Get the name of the other file to read.
    std::string other;
    bool is_res = GetOtherName(reader.filename, other);

    // Read a raw file with one or two files.
    bool is_double = Path(other).is_file();
    if (is_double)
        ReadRsrcDouble(u, other, is_res);

    // Set input fork size.
    if (is_res) {
        u.file.filename = Path(other).filename();
        u.file.res = u.d1.get();
        u.file.res_size = reader.file_size;
    } else {
        u.file.filename = Path(reader.filename).filename();
        u.file.data = u.d1.get();
        u.file.data_size = reader.file_size;
    }

    // Find file information from the local file.
    u.file.is_raw = true;
    GetLocalInfo(is_double && is_res ? other : reader.filename, u.file);
    return true;
}



// Write a single fork into a file.
static void WriteFork(fs::File &file, const std::string &name, uint8_t *data,
    uint32_t length)
{
    std::ofstream fstream {name, std::ios::binary};

    fstream.write(reinterpret_cast<char *>(data), length);
    fstream.close();

    SetLocalInfo(file, name, file.res == data);
}


// Write raw (rsrc) file(s).
void WriteRsrc(fs::File &file, const std::string &name)
{
    // Get the name of the other file to write.
    std::string other;
    bool is_res = GetOtherName(name, other);

    // Write forks (write only if size is > 0).
    if (file.data_size)
        WriteFork(file, is_res ? other : name, file.data, file.data_size);
    if (file.res_size)
        WriteFork(file, is_res ? name : other, file.res, file.res_size);
}


// Write only the data fork (if exists).
void WriteOnlyData(fs::File &file, const std::string &name)
{
    file.res_size = 0;
    WriteRsrc(file, name);
}



} // namespace maconv
} // namespace conv
