/*

Pack a local file.

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

#include <path.hpp>

namespace maconv {



// Pack a single file.
static void PackSingleFile(fs::File &file, const std::string &filename,
    ConvDataSingle *conv)
{
    std::ofstream fstream {filename, std::ios::binary};
    fs::FileWriter writer {fstream};

    conv->write(file, writer);
}



// Pack a local file.
void PackLocalFile(fs::File &file, const std::string &filename, ConvData data)
{
    // Create destination directory.
    Path::makedirs(Path(filename).parent());

    // Pack this file in one or two files.
    if (data.type == ConvData::Single)
        PackSingleFile(file, filename, data.s);
    else
        data.d->write(file, filename);
}



} // namespace maconv
