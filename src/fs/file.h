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

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <time.h>

namespace maconv {
namespace fs {


// An unique pointer on memory data.
using DataPtr = std::unique_ptr<uint8_t[]>;


// Store data about a Macintosh file.
struct File {

    // Reset all data from its file.
    void Reset();


    uint8_t *data; // Data fork.
    uint32_t data_size; // Size of the data fork.

    uint8_t *res; // Ressource fork.
    uint32_t res_size; // Size of the ressource fork.


    std::string filename; // Name of the file.
    uint32_t type; // File type (4 chars).
    uint32_t creator; // File creator (4 chars).
    uint16_t flags; // Finder flags (see docs for more information).

    time_t creation_date; // Creation date of the file (Unix time).
    time_t modif_date; // Modification date of the file (Unix file).


    bool is_raw; // Is this file from "raw" format.
    std::vector<DataPtr> mem_pool; // Memory pool for storing data.
};


} // namespace fs



// Read data from a local file.
int ReadLocalFile(const std::string &filename, fs::DataPtr &ptr);


// Get file infotmation from a local file.
void GetLocalInfo(const std::string &filename, fs::File &file, bool is_res = false);

// Set file infotmation to a local file.
void SetLocalInfo(fs::File &file, const std::string &filename, bool is_res = false);


} // namespace maconv
