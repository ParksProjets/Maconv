/*

Extract files from Stuffit (v5) archives.
See docs/stuffit/Stuffit_v5.md for more information on this format.

The code in this file is based on TheUnarchiver.
See README.md and docs/licenses/TheUnarchiver.txt for more information.

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
#include "commands.h"

#include <unordered_map>

namespace maconv {
namespace stuffit {


// Stuffit (v5) entity flags.
constexpr uint8_t kFlagDirectory = 0x40;
constexpr uint8_t kFlagCrypted = 0x20;
constexpr uint8_t kFlagHasRessource = 0x1;



// Return true if a file is in Stuffit (v5) format.
bool IsFileStuffit5(fs::FileReader &reader)
{
    reader.Seek(0);
    IS_COND(reader.file_size >= 100);

    auto first = reader.ReadString(16);
    IS_COND(first == "StuffIt (c)1997-");

    reader.Skip(4);
    auto sec = reader.ReadString(60);
    IS_COND(sec == " Aladdin Systems, Inc., http://www.aladdinsys.com/StuffIt/\x0d\x0a");

    return true;
}



// Read Stuffit (v5) header.
static uint16_t ReadHeader(fs::FileReader &reader)
{
    reader.Seek(0);
    reader.Skip(83); // Skip magic string.

    // Make sure that the archive is not encrypted.
    if (reader.ReadByte() & 0x80)
        StopOnError("Stuffit archive is encrypted");

    // Read entry offset and number of files.
    reader.Skip(8);
    uint16_t num_files = reader.ReadHalfBE();
    uint32_t offset = reader.ReadWordBE();

    reader.Seek(offset);
    return num_files;
}



// Read Stuffit (v5) file header.
static void ReadFileHeader(fs::FileReader &reader, StuffitEntry &ent)
{
    reader.Skip(4); // Skip magic number.

    // Read version and header size.
    uint8_t version = reader.ReadByte();
    reader.Skip(1);
    uint16_t header_size = reader.ReadHalfBE();
    reader.Skip(1);

    // Read flags for knowing if entry is a file or a folder.
    int flags = reader.ReadByte();
    ent.etype = (flags & kFlagDirectory) ? StuffitEntryType::Folder
        : StuffitEntryType::File;

    // Read creation and modification dates.
    ent.creation_date = reader.ReadMacDate();
    ent.modif_date = reader.ReadMacDate();
    reader.Skip(8);

    // Entity and parent offsets.
    ent.entity_off = reader.Tell() - 26;
    ent.parent_off = reader.ReadWordBE();

    // Read name and data lengths.
    uint16_t name_length = reader.ReadHalfBE();
    reader.Skip(2);  // Skip header CRC.
    ent.data.size = reader.ReadWordBE();
    ent.data.comp_size = reader.ReadWordBE();
    reader.Skip(4);  // Skip data CRC.


    // The entry is a folder: read the number of files.
    if (ent.etype == StuffitEntryType::Folder) {
        ent.num_files = reader.ReadHalfBE();

        // This folder is not a real one and must be skipped.
        if (ent.data.size == 0XFFFFFFFF) {
            ReadFileHeader(reader, ent);
            return (void)ent.num_files++;
        }
    }

    // The entry is a file: read compression method.
    else {
        ent.num_files = 0;
        ent.data.method = reader.ReadByte();
        reader.Skip(1);  // Skip password length (as archive is not encrypted).
    }


    // Read file/folder name.
    ent.name = reader.ReadString(name_length);

    // Skip comment (if exist).
    if (reader.Tell() < ent.entity_off + header_size) {
        uint16_t comment_length = reader.ReadHalfBE();
        reader.Skip(comment_length + 2);
    }


    // Read second flags: if 0x1, there is a ressource fork.
    bool has_res = reader.ReadHalfBE() & kFlagHasRessource;
    reader.Skip(2);

    // Read file type, creator and flags.
    ent.type = reader.ReadWordBE();
    ent.creator = reader.ReadWordBE();
    ent.flags = reader.ReadHalfBE();
    reader.Skip(version == 0x1 ? 22 : 18);


    // Read ressource data (if exists).
    if (has_res) {
        ent.res.size = reader.ReadWordBE();
        ent.res.comp_size = reader.ReadWordBE();
        reader.Skip(4);  // Skip ressource CRC.
        ent.res.method = reader.ReadByte();
        reader.Skip(1);  // Skip password length (as archive is not encrypted).
    } else {
        ent.res.size = 0;
        ent.res.comp_size = 0;
    }

    // If the entry is a folder, return here.
    if (ent.etype == StuffitEntryType::Folder)
        return;


    // Set data and res offsets.
    ent.res.offset = reader.Tell();
    ent.data.offset = ent.res.offset + ent.res.comp_size;

    // Seek to the next entry (if it's a file).
    reader.Seek(ent.data.offset + ent.data.comp_size);
}



// Extract a directoty.
void ExtractStuffit5(fs::FileReader &reader, const std::string &output)
{
    uint16_t num_files = ReadHeader(reader);
    std::unordered_map<uint32_t, std::string> folders;

    StuffitEntry ent;
    std::string dest_folder;

    for (uint16_t i = 0; i < num_files; i++) {
        ReadFileHeader(reader, ent);

        // Get the parent folder of the file.
        auto folder = folders.find(ent.parent_off);
        dest_folder = (folder != folders.end()) ? folder->second : output;

        ExtractStuffitEntry(reader, ent, dest_folder);
        num_files += ent.num_files;

        // Add this directory to folders map.
        if (ent.etype == StuffitEntryType::Folder)
            folders[ent.entity_off] = dest_folder + "/" + ent.name;
    }
}



} // namespace maconv
} // namespace stuffit
