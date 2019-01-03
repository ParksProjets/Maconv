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

#pragma once

#include <path.hpp>
#include <string>

namespace maconv {
namespace utils {


// An entry in the "Unix-to-Mac" array.
struct SignToMac {

    // Maching type: extension or file command.
    enum Type { Extension, FileCmd } type;

    // Help strings (and count)
    int help_len;
    const char **helps;

    // MAC creator and type.
    const char *creator;
    const char *mac;

    constexpr SignToMac(Type t, const char *c, const char *m, const char **hs, int hl)
        : type{t}, creator{c}, mac{m}, help_len{hl}, helps{hs} {}
};


// An entry in the "Mac-to-Unix" array.
struct SignToUnix {
    const char *mac;
    const char *extension;
};


// File signature array.
extern SignToMac signs_to_mac[];

// File signature array.
extern SignToUnix signs_to_unix[];



// Get the MAC type (and creator) for a specific file.
const SignToMac *GetMacType(const std::string &path);


} // namespace utils
} // namespace maconv
