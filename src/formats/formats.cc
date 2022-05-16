/*

All supported formats for file conversion.

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

#include "formats.h"
#include "conv/converters.h"
#include "disk/disk.h"
#include "stuffit/stuffit.h"

#include <path.hpp>
#include <cstring>

using namespace maconv::conv;
using namespace maconv::disk;
using namespace maconv::stuffit;

namespace maconv {


// Prefered conversion format.
ConvData prefered_conv = { ConvData::NotFound, NULL };


// All available converters.
ConvDataSingle formats_single[kNumFormatsSingle] = {
    { "macbin", ".bin", IsFileMacBinary, ReadMacBinary, WriteMacBinary },
    { "binhex", ".hqx", IsFileBinHex, ReadBinHex, WriteBinHex },
    { "applesingle", ".as", IsFileAppleSingle, ReadAppleSingle, WriteAppleSingle },
};


// Format with two files.
ConvDataDouble formats_double[kNumFormatsDouble] = {
    // { "appledouble", ReadAppleDouble, WriteAppleDouble },
    { "rsrc", ReadRsrc, WriteRsrc },
    { "data", ReadRsrc, WriteOnlyData }
};



// Get a format converter from its name.
ConvData GetConverter(const std::string &name)
{
    // Is the format a single format?
    for (auto &format : formats_single) {
        if (strcmp(format.name, name.c_str()) == 0)
            return ConvData { ConvData::Single, static_cast<void *>(&format) };
    }

    // Or a double format?
    for (auto &format : formats_double) {
        if (strcmp(format.name, name.c_str()) == 0)
            return ConvData { ConvData::Double, &format };
    }

    return ConvData { ConvData::NotFound, NULL };
}



// Guess a file format by its extension.
std::string GuessFormatByExtension(const std::string &filename)
{
    auto ext = std::string {"."} + Path(filename).extension();

    // It is a single format?
    for (auto &format : formats_single) {
        if (strcmp(format.ext, ext.c_str()) == 0)
            return format.name;
    }

    // Other special formats.
    if (ext == ".hex" || ext == ".hcx")
        return "binhex";
    return "rsrc";
}



// Get a filename for a converter.
std::string GetFilenameFor(const std::string &original, ConvData conv)
{
    auto ext = (conv.type == ConvData::Double) ? "" : conv.s->ext;
    return Path(original).filename() + ext;
}



// Extract an archive or a disk.
bool ExtractArchiveOrDisk(UnPacked &u, const std::string &output)
{
    fs::FileReader reader {u.file};

    if (IsFileDisk(u.file.filename))
        ExtractDisk(u, output);
    else if (IsFileStuffit1(reader))
        ExtractStuffit1(reader, output);
    else if (IsFileStuffit5(reader))
        ExtractStuffit5(reader, output);
    else
        return false;

    return true;
}



} // namespace maconv
