/*

Extract files from Stuffit (v1) archives.
See docs/stuffit/Stuffit_v1.md for more information on this format.

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

#include "stuffit/stuffit.h"

#include <limits>

namespace maconv {
namespace stuffit {


// Constants.
constexpr uint8_t kTypeFolder = 32;
constexpr uint8_t kTypeEndFolder = 33;



// Return true if a file is in Stuffit (v1) format.
bool IsFileStuffit1(fs::FileReader &reader)
{
    reader.Seek(0);
    IS_COND(reader.file_size >= 22);
    IS_COND(reader.ReadByte() == 'S');

    uint8_t byte = reader.ReadByte();
    IS_COND(byte == 'T' || byte == 'I');

    reader.Skip(8);
    IS_COND(reader.ReadWordBE() == 0x724c6175);

    return true;
}



// Read Stuffit (v1) header.
static uint32_t ReadHeader(fs::FileReader &reader)
{
    reader.Seek(0);
    reader.Skip(6); // Skip magic number and number of files.
    uint32_t total_size = reader.ReadWordBE();

    reader.Skip(12); // Skip other bytes from header.
    return total_size;
}



// Read Stuffit (v1) file header.
static void ReadFileHeader(fs::FileReader &reader, StuffitEntry &ent)
{
    // Read compression methods.
    ent.res.method = reader.ReadByte();
    ent.data.method = reader.ReadByte();

    // Guess file type.
    if (ent.data.method == kTypeFolder || ent.res.method == kTypeFolder)
        ent.etype = StuffitEntryType::Folder;
    else if (ent.data.method == kTypeEndFolder || ent.res.method == kTypeEndFolder)
        ent.etype = StuffitEntryType::EndFolder;
    else
        ent.etype = StuffitEntryType::File;

    // If the entry type is "EndFolder": don't parse the rest.
    if (ent.etype == StuffitEntryType::EndFolder)
        return (void)reader.Skip(110);


    // Read file name.
    uint8_t filelength = reader.ReadByte();
    ent.name = reader.ReadString(filelength);
    reader.Skip(63 - filelength);

    // Read file type, creator and Finder flags.
    ent.type = reader.ReadWordBE();
    ent.creator = reader.ReadWordBE();
    ent.flags = reader.ReadHalfLE();

    // Read creation and modification dates.
    ent.creation_date = reader.ReadMacDate();
    ent.modif_date = reader.ReadMacDate();


    // If the entry type is "Folder": don't parse the rest.
    if (ent.etype == StuffitEntryType::Folder)
        return (void)reader.Skip(28);

    // Read data and res sizes and skip CRC-16.
    ent.res.size = reader.ReadWordBE();
    ent.data.size = reader.ReadWordBE();
    ent.res.comp_size = reader.ReadWordBE();
    ent.data.comp_size = reader.ReadWordBE();
    reader.Skip(12);

    // Set ressource and data offsets.
    ent.res.offset = reader.Tell();
    ent.data.offset = ent.res.offset + ent.res.comp_size;

    // Move to next entry.
    reader.Seek(ent.data.offset + ent.data.comp_size);
}



// Extract a Stuffit (v1) directory.
static void ExtractDirectory(fs::FileReader &reader, const std::string &dest_dir,
    uint32_t total_size)
{
    StuffitEntry ent;

    while (reader.Tell() < total_size) {
        ReadFileHeader(reader, ent);
        ExtractStuffitEntry(reader, ent, dest_dir);

        if (ent.etype == StuffitEntryType::EndFolder)
            break;
        if (ent.etype == StuffitEntryType::Folder)
            ExtractDirectory(reader, dest_dir + "/" + ent.name, total_size);
    }
}



// Extract a Stuffit (v1) archive.
void ExtractStuffit1(fs::FileReader &reader, const std::string &output)
{
    uint32_t total_size = ReadHeader(reader);
    ExtractDirectory(reader, output, total_size);
}



} // namespace maconv
} // namespace stuffit
