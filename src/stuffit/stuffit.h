/*

Extract Stuffit archives.
See docs/stuffit/Stuffit.md for more information on this format.

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

#include "fs/file.h"
#include "fs/file_reader.h"
#include "fs/file_writer.h"

#include <string>

namespace maconv {
namespace stuffit {


// Stuffit entry type.
enum class StuffitEntryType : uint8_t {
    Folder, File, EndFolder, Unknown
};


// Information about a compressed fork.
struct StuffitCompInfo {
    uint8_t method; // The compression method.
    uint32_t offset; // Compressed data offset in the archive.
    uint32_t size; // Fork size (uncompressed).
    uint32_t comp_size; // Fork size (compressed).
};


// An entry in a Stuffit archive.
struct StuffitEntry {
    StuffitEntryType etype; // Entry type (folder, file, end folder).
    std::string name; // Name of the file/directory.

    uint32_t entity_off; // Entity offset.
    uint32_t parent_off; // Parent offset.
    uint16_t num_files; // Number of files (folder only).


    time_t creation_date; // Creation date of the file (Unix time).
    time_t modif_date; // Modification date of the file (Unix file).

    uint32_t type; // File type (4 chars).
    uint32_t creator; // File creator (4 chars).
    uint32_t flags; // Finder flags.


    StuffitCompInfo data; // Info about data fork.
    StuffitCompInfo res; // Info about res fork.
};



// Stuffit (v1) functions.
bool IsFileStuffit1(fs::FileReader &reader);
void ExtractStuffit1(fs::FileReader &reader, const std::string &output);

// Stuffit (v5) functions.
bool IsFileStuffit5(fs::FileReader &reader);
void ExtractStuffit5(fs::FileReader &reader, const std::string &output);


// Extract a Stuffit entry.
void ExtractStuffitEntry(fs::FileReader &reader, StuffitEntry &ent,
    const std::string &dest_folder);


} // namespace stuffit
} // namespace maconv
