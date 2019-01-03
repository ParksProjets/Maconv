/*

Run the selected command from the CLI.

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

namespace maconv {


// Is the verbose mode enabled?
extern bool verbose;


// Log debug message (if verbose mode is enabled).
extern "C" void LogDebug(const char *fmt, ...);

// Stop the program on an error.
extern "C" void StopOnError(const char *fmt, ...);


// Run convert "c" command.
void RunConvertCommand(std::string &input, std::string &output,
    std::string &format);

// Run extract "e" command.
void RunExtractCommand(std::string &input, std::string &output,
    std::string &res_format);

// Run disk creation "d" command.
void RunDiskCommand(std::string &folder, std::string &output,
    std::string &name);


} // namespace maconv
