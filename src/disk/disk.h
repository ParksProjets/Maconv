/*

Extract and pack disk images.

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
#include "formats/formats.h"

namespace maconv {
namespace disk {


// Is a file a disk file?
bool IsFileDisk(const std::string &name);

// Extract a disk file.
void ExtractDisk(UnPacked &u, const std::string &out_folder);


// Pack files into a single disk image.
void PackDiskImage(const std::string &folder, const std::string &out,
    const std::string &volname);


} // namespace disk
} // namespace maconv
