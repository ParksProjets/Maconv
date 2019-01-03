/*

Convert files from and to AppleSingle format.
See docs/formats/AppleSingle.md for more information on this format.

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



// Return true if a file is in AppleSingle format.
bool IsFileAppleSingle(fs::FileReader &reader)
{
    IS_COND(reader.file_size >= 26);

    IS_COND(reader.ReadWordBE() == 0x00051600);
    IS_COND(reader.ReadWordBE() == 0x00020000);

    return true;
}



// Extract a single AppleSingle entry.
void ExtractAppleSimpleEntry(fs::FileReader &reader, fs::File &file, uint32_t id,
    uint32_t length)
{
    switch (id) {
        // Data fork.
        case 1: {
            file.data = reader.data + reader.Tell();
            file.data_size = length;
        } break;

        // Ressource fork.
        case 2: {
            file.res = reader.data + reader.Tell();
            file.res_size = length;
        } break;

        // Ressource fork.
        case 3: {
            file.filename = reader.ReadString(length);
        } break;

        // File creation and modification dates.
        case 8: {
            file.creation_date = reader.ReadJ2000Date();
            file.modif_date = reader.ReadJ2000Date();
        } break;

        // Finder information.
        case 9: {
            file.type = reader.ReadWordBE();
            file.creator = reader.ReadWordBE();
            file.flags = reader.ReadHalfBE();
        } break;
    }
}


// Read and decode an AppleSingle file.
void ReadAppleSingle(fs::FileReader &reader, fs::File &file)
{
    reader.Skip(26);
    int num_entries = reader.ReadHalfBE();

    // Extract each entries.
    for (int i = 0; i < num_entries; i++) {
        reader.Seek(26 + i * 12);

        uint32_t id = reader.ReadWordBE();
        uint32_t offset = reader.ReadWordBE();
        uint32_t length = reader.ReadWordBE();

        reader.Seek(offset);
        ExtractAppleSimpleEntry(reader, file, id, length);
    }


    // If the filename is not provided: use the input filename.
    if (file.filename.empty())
        file.filename = Path(file.filename).filename();

    // Add missing information.
    GetLocalInfo(file.filename, file);
}



// Write an AppleSingle file.
void WriteAppleSingle(fs::File &file, fs::FileWriter &writer)
{
    // Write file header.
    writer.WriteWordBE(0x00051600);
    writer.WriteWordBE(0x00020000);
    writer.Fill(0x0, 16);
    writer.WriteHalfBE(5); // Number of entries.


    // Write filename entry.
    uint32_t offset = 24 + 5 * 12;
    writer.WriteWordBE(3); // ID.
    writer.WriteWordBE(offset); // Offset.
    writer.WriteWordBE(file.filename.size()); // Length.

    // Write date entry.
    offset += file.filename.size();
    writer.WriteWordBE(3); // ID.
    writer.WriteWordBE(offset); // Offset.
    writer.WriteWordBE(12); // Length.

    // Write Finder information entry.
    offset += 12;
    writer.WriteWordBE(9); // ID.
    writer.WriteWordBE(offset); // Offset.
    writer.WriteWordBE(32); // Length.

    // Write data fork entry.
    offset += 32;
    writer.WriteWordBE(1); // ID.
    writer.WriteWordBE(offset); // Offset.
    writer.WriteWordBE(file.data_size); // Length.

    // Write ressource fork entry.
    offset += file.data_size;
    writer.WriteWordBE(2); // ID.
    writer.WriteWordBE(offset); // Offset.
    writer.WriteWordBE(file.res_size); // Length.


    // Write filename.
    writer.WriteString(file.filename);

    // Write creation and modification dates.
    writer.WriteJ2000Date(file.creation_date);
    writer.WriteJ2000Date(file.modif_date);
    writer.WriteWordBE(0x80000000);
    writer.WriteWordBE(file.modif_date);

    // Write Finder information.
    writer.WriteWordBE(file.type);
    writer.WriteWordBE(file.creator);
    writer.WriteHalfBE(file.flags);
    writer.Fill(0x0, 22);

    // Write data fork.
    writer.Write(file.data, file.data_size);

    // Wrire ressource fork.
    writer.Write(file.res, file.res_size);
}



} // namespace maconv
} // namespace conv
