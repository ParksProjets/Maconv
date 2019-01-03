/*

Store a Macintosh file.

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

#include "fs/file.h"

#include <fstream>
#include <make_unique.hpp>
#include <sys/types.h>
#include <utime.h>

namespace maconv {



// Reset all data from its file.
void fs::File::Reset()
{
    data = nullptr;
    res = nullptr;
    data_size = 0;
    res_size = 0;

    creation_date = 0;
    modif_date = 0;

    creator = 0x63636363; // ????
    type = 0x63636363; // ????

    filename.clear();
    is_raw = false;
}



// Read data from a local file.
int ReadLocalFile(const std::string &filename, fs::DataPtr &ptr)
{
    std::ifstream file {filename, std::ios::binary};
    if (!file)
        return -1;

    // Get file size and allocate this size.
    file.seekg(0, std::ios_base::end);
    int size = file.tellg();
    ptr = std::make_unique<uint8_t[]>(size);

    // Read the file entirely.
    file.seekg(0, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(ptr.get()), size);

    return size;
}



// Get file infotmation from a local file.
void GetLocalInfo(const std::string &filename, fs::File &file, bool is_res)
{
    // TODO.
}



// Set file infotmation to a local file.
void SetLocalInfo(fs::File &file, const std::string &filename, bool is_res)
{
    auto date = is_res ? file.creation_date : file.modif_date;

    utimbuf buf { date, date };
    utime(filename.c_str(), &buf);
}



} // namespace maconv
