/*

Convert files from and to Apple Double format.

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

namespace maconv {
namespace conv {



// Get the name of the other file.
static bool GetOtherName(const std::string &input, std::string &other)
{
    bool is_res = input.size() >= 4 && input.compare(input.size() - 4, 4, ".rsrc") == 0;

    if (is_res)
        other = input.substr(0, input.size() - 4);
    else
        other = input + ".rsrc";

    return is_res;
}



static bool IsFileRessource(const std::string &input)
{
    auto filename = Path(input).filename();
    return (filename.substr(0, 2) == "._") || (filename.substr(0, 1) == "%") ||
        (input.size() >= 4 && input.compare(input.size() - 4, 4, ".rsrc") == 0);
}



static bool GetRessourceFile(const std::string &input)
{
    auto filename = Path(input).filename();
    auto folder = input.substr(0, input.size() - filename.size());

    // The file given is a ressource file.
    bool is_res =
        

    if (is_res) {

    }

    // Serach for a ressource file.
    std::string other;
    if (Path(folder + "._" + filename).is_file())
        other = folder + "._" + filename;
    else if (Path(folder + "%" + filename).is_file())
        other = folder + "%" + filename;
    else if (Path(input + ".rsrc").is_file())
        other = input + ".rsrc";
    else
        return false;
}



// Read and decode a Apple Double file.
bool ReadAppleDouble(fs::FileReader &reader, UnPacked &u)
{
    // If the file given it's a not a ressource file: read the res.
    if (!IsFileRessource(reader.filename)) {
        IS_COND(GetRessourceFile(u.n2));
        int size = ReadLocalFile(u.n2, u.d2);

        fs::FileReader res_reader {u.d2.get(), size, u.n2};
        IS_COND(IsAppleDouble(res_reader));

        ReadRessourceInfo(res_reader, u.file);
        ReadDataFork(reader, u.file);
    }

    // The file given it's a ressource file.
    else {
        IS_COND(IsAppleDouble(reader));
        ReadRessourceInfo(reader, u.file);

        if (!GetDataFile(u.n2)) return true;
        int size = ReadLocalFile(u.n2, u.d2);

        fs::FileReader data_reader {u.d2.get(), size, u.n2};
        ReadDataFork(data_reader, u.file);
    }

    return true;
}



// Write a Apple Double file.
void WriteAppleDouble(fs::File &file, const std::string &name)
{
    // TODO.
}



} // namespace maconv
} // namespace conv
