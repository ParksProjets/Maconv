/*

Macintosh file signatures.

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

#include "formats/file_signature.h"

#include <memory>

namespace maconv {
namespace utils {


// Create a new entry in the "Unix-to-Mac" array.
#define S(type, creator, mac, helps...) \
    SignToMac(SignToMac::type, creator, mac, (const char *[])helps, \
        sizeof((const char *[])helps) / sizeof(const char *))


// File signature array.
SignToMac signs_to_mac[] = {
    S(Extension, "TVOD", "Mp3 ", { ".mp3" }),
    S(FileCmd,   "TVOD", "Midi", { "Standard MIDI" }),
    S(FileCmd,   "TVOD", "WAVE", { "WAVE audio" }),

    S(FileCmd,   "CARO", "PDF ", { "PDF document" }),
    S(FileCmd,   "ZIP ", "ZIP ", { "Zip archive data" }),
    S(Extension, "Gzip", "Gzip", { ".tar.gz", ".tgz" }),
    S(FileCmd,   "TARF", "TARF", { "tar archive" }),

    S(FileCmd,   "8BIM", "JPEG", { "JPEG image data" }),
    S(FileCmd,   "8BIM", "PNGf", { "PNG image data" }),
    S(FileCmd,   "8BIM", "BMPf", { "PC bitmap" }),
    S(FileCmd,   "8BIM", "TIFF", { "TIFF image data" }),
    S(FileCmd,   "8BIM", "GIFf", { "GIF image data" }),

    S(Extension, "PPT3", "PPT3", { ".ppt", ".pptx" }),
    S(Extension, "MSWD", "W8BN", { ".doc", ".docs" }),
    S(Extension, "XCEL", "XLS ", { ".xls", ".xlsx" }),

    // Else: UNKN, TEXT
};



// File signature array.
/*
SignToUnix signs_to_unix[] = {

};
*/


// Call Unix "file" command on a path.
static std::string CallFileCommand(const std::string &path)
{
    using PosixPtr = std::unique_ptr<FILE, decltype(&pclose)>;

    auto cmd = std::string {"file -d "} + path;
    PosixPtr pipe {popen(cmd.c_str(), "r"), pclose};
    if (!pipe)
        return nullptr;

    std::array<char, 128> buffer;
    std::string result;

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        result += buffer.data();

    return result;
}



// Try matching a file with an entry.
#define MAC_MATCH(type, func, entry, help) \
    case SignToMac::type: if (func(entry, help)) return &entry; break


// Try matching a path with the given entry.
static bool UnixMatchExtension(const SignToMac &entry, const std::string &path)
{
    auto pathlen = path.size();
    auto cpath = path.c_str() + pathlen;

    for (int i = 0; i < entry.help_len; i++) {
        auto len = strlen(entry.helps[i]);
        if (pathlen >= len && strcmp(cpath - len, entry.helps[i]) == 0)
            return true;
    }

    return false;
}


// Try matching a "file" cmd output with the given entry.
static bool UnixMatchFileCmd(const SignToMac &entry, const std::string &output)
{
    for (int i = 0; i < entry.help_len; i++) {
        if (strstr(output.c_str(), entry.helps[i]) == nullptr)
            return false;
    }

    return true;
}


// Get the MAC type (and creator) for a specific file.
const SignToMac *GetMacType(const std::string &path)
{
    auto filetxt = CallFileCommand(path);

    for (auto const &entry : signs_to_mac) {
        switch (entry.type) {
            MAC_MATCH(Extension, UnixMatchExtension, entry, path);
            MAC_MATCH(FileCmd, UnixMatchFileCmd, entry, filetxt);
        }
    }

    return nullptr;
}



} // namespace utils
} // namespace maconv
