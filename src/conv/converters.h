/*

All available converters.

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
#include "formats/formats.h"

namespace maconv {
namespace conv {


// MacBinary format.
bool IsFileMacBinary(fs::FileReader &reader);
void ReadMacBinary(fs::FileReader &reader, fs::File &file);
void WriteMacBinary(fs::File &file, fs::FileWriter &writer);

// BinHex format.
bool IsFileBinHex(fs::FileReader &reader);
void ReadBinHex(fs::FileReader &reader, fs::File &file);
void WriteBinHex(fs::File &file, fs::FileWriter &writer);

// Apple Single format.
bool IsFileAppleSingle(fs::FileReader &reader);
void ReadAppleSingle(fs::FileReader &reader, fs::File &file);
void WriteAppleSingle(fs::File &file, fs::FileWriter &writer);


// Apple Double format.
bool ReadAppleDouble(fs::FileReader &reader, UnPacked &u);
void WriteAppleDouble(fs::File &file, const std::string &name);

// RSRC format (alias no format).
bool ReadRsrc(fs::FileReader &reader, UnPacked &u);
void WriteRsrc(fs::File &file, const std::string &name);
void WriteOnlyData(fs::File &file, const std::string &name);


// Extract a single AppleSingle entry.
void ExtractAppleSimpleEntry(fs::FileReader &reader, fs::File &file, uint32_t id,
    uint32_t length);


} // namespace maconv
} // namespace conv
