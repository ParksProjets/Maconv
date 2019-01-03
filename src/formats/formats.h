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

#pragma once

#include "fs/file.h"
#include "fs/file_reader.h"
#include "fs/file_writer.h"

#include <fstream>

namespace maconv {


// Data for unpacked files.
struct UnPacked {
    fs::DataPtr d1, d2; // Data from opened files.
    fs::File file; // The unpacked file.
    std::string n1, n2; // Input file names.
};


// Convertion data with a single input file.
struct ConvDataSingle {
    using TestF = bool (*)(fs::FileReader &);
    using ReaderF = void (*)(fs::FileReader &, fs::File &);
    using WriterF = void (*)(fs::File &, fs::FileWriter &);

    const char *name; // Converter name.
    const char *ext; // File extension.
    TestF test;
    ReaderF read;
    WriterF write;
};


// Convertion data with two input files.
struct ConvDataDouble {
    using ReaderF = bool (*)(fs::FileReader &reader, UnPacked &u);
    using WriterF = void (*)(fs::File &, const std::string &);

    const char *name; // Converter name.
    ReaderF read;
    WriterF write;
};


// Store data about a convertion format.
struct ConvData {
    enum { NotFound, Single, Double } type;
    union { void *v; ConvDataSingle *s; ConvDataDouble *d; };
};



// Prefered conversion format.
extern ConvData prefered_conv;

// Single converters (converters that need only one file).
constexpr int kNumFormatsSingle = 3;
extern ConvDataSingle formats_single[kNumFormatsSingle];

// Double converters (converters that need two files).
constexpr int kNumFormatsDouble = 2;
extern ConvDataDouble formats_double[kNumFormatsDouble];



// Get a format converter from its name.
ConvData GetConverter(const std::string &name);

// Guess a file format by its extension.
std::string GuessFormatByExtension(const std::string &filename);

// Get a filename for a converter.
std::string GetFilenameFor(const std::string &original, ConvData conv);


// Unpack a single file.
bool UnPackSingle(fs::File &file, fs::FileReader &reader);

// Unpack a local file (recursively).
UnPacked UnPackLocalFile(const std::string &input, bool recurs = true);


// Extract an archive or a disk.
bool ExtractArchiveOrDisk(UnPacked &u, const std::string &output);


// Pack a local file.
void PackLocalFile(fs::File &file, const std::string &filename, ConvData data);


} // namespace maconv
