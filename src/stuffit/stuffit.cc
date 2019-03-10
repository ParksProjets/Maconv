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

#include "stuffit/stuffit.h"
#include "stuffit/methods.h"
#include "formats/formats.h"
#include "commands.h"

#include <path.hpp>
#include <cstdarg>
#include <algorithm>

namespace maconv {
namespace stuffit {



// Warn the user that a fork couldn't be extracted.
static void WarnForkError(StuffitEntry &ent, bool is_res, const char *msg, ...)
{
    va_list args;
    fprintf(stderr, "\033[1m\033[33mWARNING: ");
    fprintf(stderr, is_res ? "ressource fork " : "data fork ");
    fprintf(stderr, "of '%s' couldn't be extracted (", ent.name.c_str());

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, ")\033[0m\n");
}



// Extract a single fork.
static void ExtractFork(StuffitEntry &ent, bool is_res, fs::File &file,
    uint8_t *data)
{
    const StuffitCompInfo &info = is_res ? ent.res : ent.data;

    // Select the compression handler and extract the fork.
    auto ptr = GetCompressionMethod(info.method);
    if (!ptr)
        return (void)WarnForkError(ent, is_res, "compression method %u not supported", info.method);

    // Try extracting the fork.
    LogDebug("  Extracting %s fork using algo %d", (is_res ? "ressource" : "data"), info.method);
    try {
        ptr->Extract(info, data, file.mem_pool);
    } catch (ExtractException &e) {
        return (void)WarnForkError(ent, is_res, e.what());
    }

    // Fill file information.
    if (is_res) {
        file.res = ptr->uncompressed;
        file.res_size = ptr->total_size;
    } else {
        file.data_size = ptr->total_size;
        file.data = ptr->uncompressed;
    }
}



// Extract a file.
static void ExtractFile(fs::FileReader &reader, StuffitEntry &ent,
    const std::string &dest_folder)
{
    // Copy extracted data to file object.
    fs::File file;
    file.Reset();

    file.type = ent.type;
    file.creator = ent.type;
    file.flags = ent.type;
    file.creation_date = ent.type;
    file.modif_date = ent.type;
    file.filename = ent.name;

    // Log information to user.
    std::string filename = dest_folder + "/" + GetFilenameFor(ent.name, prefered_conv);
    filename.erase(std::remove(filename.begin(), filename.end(), '\r'), filename.end());
    LogDebug("Extracting %s ...", filename.c_str());

    // Uncompress forks (if not empty).
    if (ent.data.comp_size > 0)
        ExtractFork(ent, false, file, reader.data);
    if (ent.res.comp_size > 0)
        ExtractFork(ent, true, file, reader.data);

    // Save the file.
    PackLocalFile(file, filename, prefered_conv);
}



// Extract a directory.
static void ExtractDirectory(StuffitEntry &ent, const std::string &dest_folder)
{
    std::string dirname = dest_folder + "/" + ent.name;
    Path::makedirs(dirname);

    // TODO: set mofitication date.
}



// Extract a Stuffit entry.
void ExtractStuffitEntry(fs::FileReader &reader, StuffitEntry &ent,
    const std::string &dest_folder)
{
    if (ent.etype == StuffitEntryType::File)
        ExtractFile(reader, ent, dest_folder);
    else if (ent.etype == StuffitEntryType::Folder)
        ExtractDirectory(ent, dest_folder);
}



} // namespace stuffit
} // namespace maconv
