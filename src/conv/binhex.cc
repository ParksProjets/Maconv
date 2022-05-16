/*

Convert files from and to BinHex format.
See docs/formats/BinHex.md for more information on this format.

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



// Return true if a file is in BinHex format.
bool IsFileBinHex(fs::FileReader &/* reader */)
{
    // TODO.

    return false;
}



// Read and decode a BinHex file.
void ReadBinHex(fs::FileReader &/* reader */, fs::File &/* file */)
{
    // TODO.
}



// Write a BinHex file.
void WriteBinHex(fs::File &/* file */, fs::FileWriter &/* writer */)
{
    // TODO.
}



} // namespace maconv
} // namespace conv
