/*

Unpack a local file.

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

#include "formats/formats.h"
#include "commands.h"

namespace maconv {



// Unpack two files (double format).
static void UnPackDouble(fs::FileReader &reader, UnPacked &u)
{
    for (auto &format : formats_double) {
        reader.Seek(0);

        // "read" returns true if the file is actually read.
        if (format.read(reader, u))
            return;
    }
}


// Unpack a single file.
bool UnPackSingle(fs::File &file, fs::FileReader &reader)
{
    for (auto &format : formats_single) {
        // Test if the file is in this format (if true, unpack it).
        reader.Seek(0);
        if (!format.test(reader))
            continue;

        reader.Seek(0);
        file.Reset();
        format.read(reader, file);
        return true;
    }

    return false;
}



// Unpack the first local file.
static UnPacked UnPackFirstLocalFile(const std::string &input)
{
    UnPacked u;
    u.n1 = input;

    // Read the input file given.
    int size = ReadLocalFile(input, u.d1);
    if (size == -1)
        StopOnError("can't read input file %s", input.c_str());

    // Unpack one or two files.
    fs::FileReader reader {u.d1.get(), static_cast<uint32_t>(size), input};
    if (!UnPackSingle(u.file, reader))
        UnPackDouble(reader, u); // Cannot fail ("rsrc" accepts all files).

    return u;
}


// Unpack a local file (recursively).
UnPacked UnPackLocalFile(const std::string &input, bool recurs)
{
    auto u = UnPackFirstLocalFile(input);

    while (recurs) {
        fs::FileReader reader {u.file};
        if (!UnPackSingle(u.file, reader))
            break;
    }

    return u;
}



} // namespace maconv
