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

#include "commands.h"
#include "formats/formats.h"
#include "disk/disk.h"

#include <path.hpp>

namespace maconv {



// Run convert "c" command.
void RunConvertCommand(std::string &input, std::string &output, std::string
    &format)
{
    // Read input path given in argument.
    if (!Path(input).is_file())
        StopOnError("input file doesn't exist");

    // Unpack the input file.
    auto u = UnPackLocalFile(input);


    // If format is not present: guess output format.
    bool no_out = output.empty() || Path(output).is_directory();
    if (format.empty())
        format = no_out ? "rsrc" : GuessFormatByExtension(output);

    // Get the converter for the selected format.    
    auto conv = GetConverter(format);
    if (conv.type == ConvData::NotFound)
        StopOnError("format '%s' doesn't exist", format.c_str());


    // Normalise the output directory.
    if (no_out && !output.empty())
        output = Path(output).directory().string();
    else
        output = Path(input).parent().string();

    // If output is not present: use input name if single, real name if double.
    bool is_double = (conv.type == ConvData::Double);
    if (no_out)
        output += is_double ? u.file.filename : GetFilenameFor(input, conv);


    // Now save the output file(s).
    PackLocalFile(u.file, output, conv);
}



// Run extract "e" command.
void RunExtractCommand(std::string &input, std::string &output, std::string
    &res_format)
{
    // Read input path given in argument.
    if (!Path(input).is_file())
        StopOnError("input file doesn't exist");

    // Read ressource format.
    prefered_conv = GetConverter(res_format);
    if (prefered_conv.type == ConvData::NotFound)
        StopOnError("format '%s' doesn't exist", res_format.c_str());

    // Unpack and extract the input file.
    auto u = UnPackLocalFile(input);
    if (!ExtractArchiveOrDisk(u, output))
        StopOnError("can't extract input file (unsupported format)");
}



// Run disk creation "d" command.
void RunDiskCommand(std::string &folder, std::string &output, std::string
    &name)
{
    // Read input path given in argument.
    auto abs_in = Path(folder).absolute();
    if (!abs_in.is_directory())
        StopOnError("input folder doesn't exist");

    // If no output name: use directory name.
    if (output.empty())
        output = abs_in.trim().string() + ".img";

    // If no volume name: use directory name.
    if (name.empty())
        name = abs_in.trim().filename();

    disk::PackDiskImage(abs_in.string(), output, name);
}



} // namespace maconv
