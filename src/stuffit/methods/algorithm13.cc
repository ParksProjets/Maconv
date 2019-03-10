/*

Stuffit algorithm 13: LZSS and Huffman.

The code in this file is based on TheUnarchiver.
See README.md and docs/licenses/TheUnarchiver.txt for more information.

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

#include "stuffit/methods/algorithm13.h"

#include <make_unique.hpp>
#include <string.h>

namespace maconv {
namespace stuffit {



// First code lengths (1).
static const int kFirstCodeLengths_1[321] = {
     4,  5,  7,  8,  8,  9,  9,  9,  9,  7,  9,  9,  9,  8,  9,  9,  9,  9,  9,
     9,  9,  9,  9, 10,  9,  9, 10, 10,  9, 10,  9,  9,  5,  9,  9,  9,  9, 10,
     9,  9,  9,  9,  9,  9,  9,  9,  7,  9,  9,  8,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  8,  9,  9,  8,  8,  9,  9,  9,  9,  9,  9,
     9,  7,  8,  9,  7,  9,  9,  7,  7,  9,  9,  9,  9, 10,  9, 10, 10, 10,  9,
     9,  9,  5,  9,  8,  7,  5,  9,  8,  8,  7,  9,  9,  8,  8,  5,  5,  7, 10,
     5,  8,  5,  8,  9,  9,  9,  9,  9, 10,  9,  9, 10,  9,  9, 10, 10, 10, 10,
    10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,  9,  9, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10,
    10, 10,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,  9,  9, 10, 10,  9, 10, 10, 10, 10, 10, 10,
    10,  9, 10, 10, 10,  9, 10,  9,  5,  6,  5,  5,  8,  9,  9,  9,  9,  9,  9,
    10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10,  9, 10,  9,  9,  9, 10,  9, 10,  9, 10,  9, 10,  9,
    10, 10, 10,  9, 10,  9, 10, 10,  9,  9,  9,  6,  9,  9, 10,  9,  5
};

// First code lengths (2).
static const int kFirstCodeLengths_2[321] = {
     4,  7,  7,  8,  7,  8,  8,  8,  8,  7,  8,  7,  8,  7,  9,  8,  8,  8,  9,
     9,  9,  9, 10, 10,  9, 10, 10, 10, 10, 10,  9,  9,  5,  9,  8,  9,  9, 11,
     10, 9,  8,  9,  9,  9,  8,  9,  7,  8,  8,  8,  9,  9,  9,  9,  9, 10,  9,
     9,  9, 10,  9,  9, 10,  9,  8,  8,  7,  7,  7,  8,  8,  9,  8,  8,  9,  9,
     8,  8,  7,  8,  7, 10,  8,  7,  7,  9,  9,  9,  9, 10, 10, 11, 11, 11, 10,
     9,  8,  6,  8,  7,  7,  5,  7,  7,  7,  6,  9,  8,  6,  7,  6,  6,  7,  9,
     6,  6,  6,  7,  8,  8,  8,  8,  9, 10,  9, 10,  9,  9,  8,  9, 10, 10,  9,
    10, 10,  9,  9, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 11, 10, 10, 10, 10,
    10, 10, 10, 11, 10, 11, 10, 10,  9, 11, 10, 10, 10, 10, 10, 10,  9,  9, 10,
    11, 10, 11, 10, 11, 10, 12, 10, 11, 10, 12, 11, 12, 10, 12, 10, 11, 10, 11,
    11, 11,  9, 10, 11, 11, 11, 12, 12, 10, 10, 10, 11, 11, 10, 11, 10, 10,  9,
    11, 10, 11, 10, 11, 11, 11, 10, 11, 11, 12, 11, 11, 10, 10, 10, 11, 10, 10,
    11, 11, 12, 10, 10, 11, 11, 12, 11, 11, 10, 11,  9, 12, 10, 11, 11, 11, 10,
    11, 10, 11, 10, 11,  9, 10,  9,  7,  3,  5,  6,  6,  7,  7,  8,  8,  8,  9,
     9,  9, 11, 10, 10, 10, 12, 13, 11, 12, 12, 11, 13, 12, 12, 11, 12, 12, 13,
    12, 14, 13, 14, 13, 15, 13, 14, 15, 15, 14, 13, 15, 15, 14, 15, 14, 15, 15,
    14, 15, 13, 13, 14, 15, 15, 14, 14, 16, 16, 15, 15, 15, 12, 15, 10
};

// First code lengths (3).
static const int kFirstCodeLengths_3[321] = {
     6,  6,  6,  6,  6,  9,  8,  8,  4,  9,  8,  9,  8,  9,  9,  9,  8,  9,  9,
    10,  8, 10, 10, 10,  9, 10, 10, 10,  9, 10, 10,  9,  9,  9,  8, 10,  9, 10,
     9, 10,  9, 10,  9, 10,  9,  9,  8,  9,  8,  9,  9,  9, 10, 10, 10, 10,  9,
     9,  9, 10,  9, 10,  9,  9,  7,  8,  8,  9,  8,  9,  9,  9,  8,  9,  9, 10,
     9,  9,  8,  9,  8,  9,  8,  8,  8,  9,  9,  9,  9,  9, 10, 10, 10, 10, 10,
     9,  8,  8,  9,  8,  9,  7,  8,  8,  9,  8, 10, 10,  8,  9,  8,  8,  8, 10,
     8,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 10,  9,  7,  9,  9, 10, 10,
    10, 10, 10,  9, 10, 10, 10, 10, 10, 10,  9,  9, 10, 10, 10, 10, 10, 10, 10,
    10,  9, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,  9,  9,  9,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10,
    10,  9,  8,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10,  9,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9,  9, 10, 10, 10,
    10, 10, 10,  9, 10, 10, 10, 10, 10, 10,  9,  9,  9, 10, 10, 10, 10, 10, 10,
     9,  9, 10,  9,  9,  8,  9,  8,  9,  4,  6,  6,  6,  7,  8,  8,  9,  9, 10,
    10, 10,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10,  7, 10, 10, 10,  7, 10, 10,  7,  7,  7,  7,  7,  6,  7, 10,  7,  7, 10,
     7,  7,  7,  6,  7,  6,  6,  7,  7,  6,  6,  9,  6,  9, 10,  6, 10
};

// First code lengths (4).
static const int kFirstCodeLengths_4[321] = {
     2,  6,  6,  7,  7,  8,  7,  8,  7,  8,  8,  9,  8,  9,  9,  9,  8,  8,  9,
     9,  9, 10, 10,  9,  8, 10,  9, 10,  9, 10,  9,  9,  6,  9,  8,  9,  9, 10,
     9,  9,  9, 10,  9,  9,  9,  9,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9, 10, 10,  9,  7,  7,  8,  8,  8,  8,  9,  9,  7,  8,  9, 10,
     8,  8,  7,  8,  8, 10,  8,  8,  8,  9,  8,  9,  9, 10,  9, 11, 10, 11,  9,
     9,  8,  7,  9,  8,  8,  6,  8,  8,  8,  7, 10,  9,  7,  8,  7,  7,  8, 10,
     7,  7,  7,  8,  9,  9,  9,  9, 10, 11,  9, 11, 10,  9,  7,  9, 10, 10, 10,
    11, 11, 10, 10, 11, 10, 10, 10, 11, 11, 10,  9, 10, 10, 11, 10, 11, 10, 11,
    10, 10, 10, 11, 10, 11, 10, 10,  9, 10, 10, 11, 10, 10, 10, 10,  9, 10, 10,
    10, 10, 11, 10, 11, 10, 11, 10, 11, 11, 11, 10, 12, 10, 11, 10, 11, 10, 11,
    11, 10,  8, 10, 10, 11, 10, 11, 11, 11, 10, 11, 10, 11, 10, 11, 11, 11,  9,
    10, 11, 11, 10, 11, 11, 11, 10, 11, 11, 11, 10, 10, 10, 10, 10, 11, 10, 10,
    11, 11, 10, 10,  9, 11, 10, 10, 11, 11, 10, 10, 10, 11, 10, 10, 10, 10, 10,
    10,  9, 11, 10, 10,  8, 10,  8,  6,  5,  6,  6,  7,  7,  8,  8,  8,  9, 10,
    11, 10, 10, 11, 11, 12, 12, 10, 11, 12, 12, 12, 12, 13, 13, 13, 13, 13, 12,
    13, 13, 15, 14, 12, 14, 15, 16, 12, 12, 13, 15, 14, 16, 15, 17, 18, 15, 17,
    16, 15, 15, 15, 15, 13, 13, 10, 14, 12, 13, 17, 17, 18, 10, 17,  4
};

// First code lengths (5).
static const int kFirstCodeLengths_5[321] = {
     7,  9,  9,  9,  9,  9,  9,  9,  9,  8,  9,  9,  9,  7,  9,  9,  9,  9,  9,
     9,  9,  9,  9, 10,  9, 10,  9, 10,  9, 10,  9,  9,  5,  9,  7,  9,  9,  9,
     9,  9,  7,  7,  7,  9,  7,  7,  8,  7,  8,  8,  7,  7,  9,  9,  9,  9,  7,
     7,  7,  9,  9,  9,  9,  9,  9,  7,  9,  7,  7,  7,  7,  9,  9,  7,  9,  9,
     7,  7,  7,  7,  7,  9,  7,  8,  7,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  7,  8,  7,  7,  7,  8,  8,  6,  7,  9,  7,  7,  8,  7,  5,  6,  9,
     5,  7,  5,  6,  7,  7,  9,  8,  9,  9,  9,  9,  9,  9,  9,  9, 10,  9, 10,
    10, 10,  9,  9, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10,  9, 10, 10, 10,  9, 10, 10, 10,  9,  9, 10,  9,  9,  9,  9,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,
    10, 10,  9, 10, 10, 10,  9, 10, 10, 10,  9,  9,  9, 10, 10, 10, 10, 10,  9,
    10,  9, 10, 10,  9, 10, 10,  9, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10, 10, 10, 10, 10,
    10,  9, 10,  9, 10,  9, 10, 10,  9,  5,  6,  8,  8,  7,  7,  7,  9,  9,  9,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
     9,  9,  9,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,  9, 10, 10,  5, 10,  8,  9,  8,  9
};

// First code lengths (global array).
static const int *kFirstCodeLengths[5] = {
    kFirstCodeLengths_1, kFirstCodeLengths_2, kFirstCodeLengths_3,
    kFirstCodeLengths_4, kFirstCodeLengths_5
};



// Second code lengths (1).
static const int kSecondCodeLengths_1[321] = {
     4,  5,  6,  6,  7,  7,  6,  7,  7,  7,  6,  8,  7,  8,  8,  8,  8,  9,  6,
     9,  8,  9,  8,  9,  9,  9,  8, 10,  5,  9,  7,  9,  6,  9,  8, 10,  9, 10,
     8,  8,  9,  9,  7,  9,  8,  9,  8,  9,  8,  8,  6,  9,  9,  8,  8,  9,  9,
    10,  8,  9,  9, 10,  8, 10,  8,  8,  8,  8,  8,  9,  7, 10,  6,  9,  9, 11,
     7,  8,  8,  9,  8, 10,  7,  8,  6,  9, 10,  9,  9, 10,  8, 11,  9, 11,  9,
    10,  9,  8,  9,  8,  8,  8,  8, 10,  9,  9, 10, 10,  8,  9,  8,  8,  8, 11,
     9,  8,  8,  9,  9, 10,  8, 11, 10, 10,  8, 10,  9, 10,  8,  9,  9, 11,  9,
    11,  9, 10, 10, 11, 10, 12,  9, 12, 10, 11, 10, 11,  9, 10, 10, 11, 10, 11,
    10, 11, 10, 11, 10, 10, 10,  9,  9,  9,  8,  7,  6,  8, 11, 11,  9, 12, 10,
    12,  9, 11, 11, 11, 10, 12, 11, 11, 10, 12, 10, 11, 10, 10, 10, 11, 10, 11,
    11, 11,  9, 12, 10, 12, 11, 12, 10, 11, 10, 12, 11, 12, 11, 12, 11, 12, 10,
    12, 11, 12, 11, 11, 10, 12, 10, 11, 10, 12, 10, 12, 10, 12, 10, 11, 11, 11,
    10, 11, 11, 11, 10, 12, 11, 12, 10, 10, 11, 11,  9, 12, 11, 12, 10, 11, 10,
    12, 10, 11, 10, 12, 10, 11, 10,  7,  5,  4,  6,  6,  7,  7,  7,  8,  8,  7,
     7,  6,  8,  6,  7,  7,  9,  8,  9,  9, 10, 11, 11, 11, 12, 11, 10, 11, 12,
    11, 12, 11, 12, 12, 12, 12, 11, 12, 12, 11, 12, 11, 12, 11, 13, 11, 12, 10,
    13, 10, 14, 14, 13, 14, 15, 14, 16, 15, 15, 18, 18, 18,  9, 18,  8
};

// Second code lengths (2).
static const int kSecondCodeLengths_2[321] = {
     5,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  8,  7,  8,  7,  7,  7,  8,  8,
     8,  8,  9,  8,  9,  8,  9,  9,  9,  7,  9,  8,  8,  6,  9,  8,  9,  8,  9,
     8,  9,  8,  9,  8,  9,  8,  9,  8,  8,  8,  8,  8,  9,  8,  9,  8,  9,  9,
    10,  8, 10,  8,  9,  9,  8,  8,  8,  7,  8,  8,  9,  8,  9,  7,  9,  8, 10,
     8,  9,  8,  9,  8,  9,  8,  8,  8,  9,  9,  9,  9, 10,  9, 11,  9, 10,  9,
    10,  8,  8,  8,  9,  8,  8,  8,  9,  9,  8,  9, 10,  8,  9,  8,  8,  8, 11,
     8,  7,  8,  9,  9,  9,  9, 10,  9, 10,  9, 10,  9,  8,  8,  9,  9, 10,  9,
    10,  9, 10,  8, 10,  9, 10,  9, 11, 10, 11,  9, 11, 10, 10, 10, 11,  9, 11,
     9, 10,  9, 11,  9, 11, 10, 10,  9, 10,  9,  9,  8, 10,  9, 11,  9,  9,  9,
    11, 10, 11,  9, 11,  9, 11,  9, 11, 10, 11, 10, 11, 10, 11,  9, 10, 10, 11,
    10, 10,  8, 10,  9, 10, 10, 11,  9, 11,  9, 10, 10, 11,  9, 10, 10,  9,  9,
    10,  9, 10,  9, 10,  9, 10,  9, 11,  9, 11, 10, 10,  9, 10,  9, 11,  9, 11,
     9, 11,  9, 10,  9, 11,  9, 11,  9, 11,  9, 10,  8, 11,  9, 10,  9, 10,  9,
    10,  8, 10,  8,  9,  8,  9,  8,  7,  4,  4,  5,  6,  6,  6,  7,  7,  7,  7,
     8,  8,  8,  7,  8,  8,  9,  9, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 12,
    11, 11, 12, 12, 11, 12, 12, 11, 12, 12, 12, 12, 12, 12, 11, 12, 11, 13, 12,
    13, 12, 13, 14, 14, 14, 15, 13, 14, 13, 14, 18, 18, 17,  7, 16,  9
};

// Second code lengths (3).
static const int kSecondCodeLengths_3[321] = {
     5,  6,  6,  6,  6,  7,  7,  7,  6,  8,  7,  8,  7,  9,  8,  8,  7,  7,  8,
     9,  9,  9,  9, 10,  8,  9,  9, 10,  8, 10,  9,  8,  6, 10,  8, 10,  8, 10,
     9,  9,  9,  9,  9, 10,  9,  9,  8,  9,  8,  9,  8,  9,  9, 10,  9, 10,  9,
     9,  8, 10,  9, 11, 10,  8,  8,  8,  8,  9,  7,  9,  9, 10,  8,  9,  8, 11,
     9, 10,  9, 10,  8,  9,  9,  9,  9,  8,  9,  9, 10, 10, 10, 12, 10, 11, 10,
    10,  8,  9,  9,  9,  8,  9,  8,  8, 10,  9, 10, 11,  8, 10,  9,  9,  8, 12,
     8,  9,  9,  9,  9,  8,  9, 10,  9, 12, 10, 10, 10,  8,  7, 11, 10,  9, 10,
    11,  9, 11,  7, 11, 10, 12, 10, 12, 10, 11,  9, 11,  9, 12, 10, 12, 10, 12,
    10,  9, 11, 12, 10, 12, 10, 11,  9, 10,  9, 10,  9, 11, 11, 12,  9, 10,  8,
    12, 11, 12,  9, 12, 10, 12, 10, 13, 10, 12, 10, 12, 10, 12, 10,  9, 10, 12,
    10,  9,  8, 11, 10, 12, 10, 12, 10, 12, 10, 11, 10, 12,  8, 12, 10, 11, 10,
    10, 10, 12,  9, 11, 10, 12, 10, 12, 11, 12, 10,  9, 10, 12,  9, 10, 10, 12,
    10, 11, 10, 11, 10, 12,  8, 12,  9, 12,  8, 12,  8, 11, 10, 11, 10, 11,  9,
    10,  8, 10,  9,  9,  8,  9,  8,  7,  4,  3,  5,  5,  6,  5,  6,  6,  7,  7,
     8,  8,  8,  7,  7,  7,  9,  8,  9,  9, 11,  9, 11,  9,  8,  9,  9, 11, 12,
    11, 12, 12, 13, 13, 12, 13, 14, 13, 14, 13, 14, 13, 13, 13, 12, 13, 13, 12,
    13, 13, 14, 14, 13, 13, 14, 14, 14, 14, 15, 18, 17, 18,  8, 16, 10
};

// Second code lengths (4).
static const int kSecondCodeLengths_4[321] = {
     4,  5,  6,  6,  6,  6,  7,  7,  6,  7,  7,  9,  6,  8,  8,  7,  7,  8,  8,
     8,  6,  9,  8,  8,  7,  9,  8,  9,  8,  9,  8,  9,  6,  9,  8,  9,  8, 10,
     9,  9,  8, 10,  8, 10,  8,  9,  8,  9,  8,  8,  7,  9,  9,  9,  9,  9,  8,
    10,  9, 10,  9, 10,  9,  8,  7,  8,  9,  9,  8,  9,  9,  9,  7, 10,  9, 10,
     9,  9,  8,  9,  8,  9,  8,  8,  8,  9,  9, 10,  9,  9,  8, 11,  9, 11, 10,
    10,  8,  8, 10,  8,  8,  9,  9,  9, 10,  9, 10, 11,  9,  9,  9,  9,  8,  9,
     8,  8,  8, 10, 10,  9,  9,  8, 10, 11, 10, 11, 11,  9,  8,  9, 10, 11,  9,
    10, 11, 11,  9, 12, 10, 10, 10, 12, 11, 11,  9, 11, 11, 12,  9, 11,  9, 10,
    10, 10, 10, 12,  9, 11, 10, 11,  9, 11, 11, 11, 10, 11, 11, 12,  9, 10, 10,
    12, 11, 11, 10, 11,  9, 11, 10, 11, 10, 11,  9, 11, 11,  9,  8, 11, 10, 11,
    11, 10,  7, 12, 11, 11, 11, 11, 11, 12, 10, 12, 11, 13, 11, 10, 12, 11, 10,
    11, 10, 11, 10, 11, 11, 11, 10, 12, 11, 11, 10, 11, 10, 10, 10, 11, 10, 12,
    11, 12, 10, 11,  9, 11, 10, 11, 10, 11, 10, 12,  9, 11, 11, 11,  9, 11, 10,
    10,  9, 11, 10, 10,  9, 10,  9,  7,  4,  5,  5,  5,  6,  6,  7,  6,  8,  7,
    8,  9,  9,  7,  8,  8,  10,  9, 10, 10, 12, 10, 11, 11, 11, 11, 10, 11, 12,
    11, 11, 11, 11, 11, 13, 12, 11, 12, 13, 12, 12, 12, 13, 11,  9, 12, 13,  7,
    13, 11, 13, 11, 10, 11, 13, 15, 15, 12, 14, 15, 15, 15,  6, 15,  5
};

// Second code lengths (5).
static const int kSecondCodeLengths_5[321] = {
     8, 10, 11, 11, 11, 12, 11, 11, 12,  6, 11, 12, 10,  5, 12, 12, 12, 12, 12,
    12, 12, 13, 13, 14, 13, 13, 12, 13, 12, 13, 12, 15,  4, 10,  7,  9, 11, 11,
    10,  9,  6,  7,  8,  9,  6,  7,  6,  7,  8,  7,  7,  8,  8,  8,  8,  8,  8,
     9,  8,  7, 10,  9, 10, 10, 11,  7,  8,  6,  7,  8,  8,  9,  8,  7, 10, 10,
     8,  7,  8,  8,  7, 10,  7,  6,  7,  9,  9,  8, 11, 11, 11, 10, 11, 11, 11,
     8, 11,  6,  7,  6,  6,  6,  6,  8,  7,  6, 10,  9,  6,  7,  6,  6,  7, 10,
     6,  5,  6,  7,  7,  7, 10,  8, 11,  9, 13,  7, 14, 16, 12, 14, 14, 15, 15,
    16, 16, 14, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 13, 14, 14, 16, 15, 17,
    14, 17, 15, 17, 12, 14, 13, 16, 12, 17, 13, 17, 14, 13, 13, 14, 14, 12, 13,
    15, 15, 14, 15, 17, 14, 17, 15, 14, 15, 16, 12, 16, 15, 14, 15, 16, 15, 16,
    17, 17, 15, 15, 17, 17, 13, 14, 15, 15, 13, 12, 16, 16, 17, 14, 15, 16, 15,
    15, 13, 13, 15, 13, 16, 17, 15, 17, 17, 17, 16, 17, 14, 17, 14, 16, 15, 17,
    15, 15, 14, 17, 15, 17, 15, 16, 15, 15, 16, 16, 14, 17, 17, 15, 15, 16, 15,
    17, 15, 14, 16, 16, 16, 16, 16, 12,  4,  4,  5,  5,  6,  6,  6,  7,  7,  7,
     8,  8,  8,  8,  9,  9,  9,  9,  9, 10, 10, 10, 11, 10, 11, 11, 11, 11, 11,
    12, 12, 12, 13, 13, 12, 13, 12, 14, 14, 12, 13, 13, 13, 13, 14, 12, 13, 13,
    14, 14, 14, 13, 14, 14, 15, 15, 13, 15, 13, 17, 17, 17,  9, 17,  7
};

// Second code lengths (global array).
static const int *kSecondCodeLengths[5] = {
    kSecondCodeLengths_1, kSecondCodeLengths_2, kSecondCodeLengths_3,
    kSecondCodeLengths_4, kSecondCodeLengths_5
};



// Offset code lengths (1).
static const int kOffsetCodeLengths_1[11] = {
    5, 6, 3, 3, 3, 3, 3, 3, 3, 4, 6
};

// Offset code lengths (2).
static const int kOffsetCodeLengths_2[13] = {
    5, 6, 4, 4, 3, 3, 3, 3, 3, 4, 4, 4, 6
};

// Offset code lengths (3).
static const int kOffsetCodeLengths_3[14] = {
    6, 7, 4, 4, 3, 3, 3, 3, 3, 4, 4, 4, 5, 7
};

// Offset code lengths (4).
static const int kOffsetCodeLengths_4[11] = {
    3, 6, 5, 4, 2, 3, 3, 3, 4, 4, 6
};

// Offset code lengths (5).
static const int kOffsetCodeLengths_5[11] = {
    6, 7, 7, 6, 4, 3, 2, 2, 3, 3, 6
};

// Offset code lengths (global array).
static const int *kOffsetCodeLengths[5] = {
    kOffsetCodeLengths_1, kOffsetCodeLengths_2, kOffsetCodeLengths_3,
    kOffsetCodeLengths_4, kOffsetCodeLengths_5
};

// Offset code size.
static const int kOffsetCodeSize[5] = { 11, 13, 14, 11, 11 };



// Meta codes.
static const int kMetaCodes[37] = {
    0x5D8, 0x058, 0x040, 0x0C0, 0x000, 0x078, 0x02B, 0x014, 0x00C, 0x01C, 0x01B,
    0x00B, 0x010, 0x020, 0x038, 0x018, 0x0D8, 0xBD8, 0x180, 0x680, 0x380, 0xF80,
    0x780, 0x480, 0x080, 0x280, 0x3D8, 0xFD8, 0x7D8, 0x9D8, 0x1D8, 0x004, 0x001,
    0x002, 0x007, 0x003, 0x008
};

// Meta code lengths.
static const int kMetaCodeLengths[37] = {
    11,  8,  8,  8,  8,  7,  6,  5,  5,  5,  5,  6,  5,  6,  7,  7,  9, 12, 10,
    11, 11, 12, 12, 11, 11, 11, 12, 12, 12, 12, 12,  5,  2,  2,  3,  4,  5
};




// LZSS decoder constants.
constexpr int kLzssMatch = -1;
constexpr int kLzssEnd = -2;



// Intialize LZSS.
void Algorithm13Method::InitializeLZSS()
{
    int val = *(input.data++);
    int code = ((val & 0xFF) >> 4);

    if (code == 0) {
        HuffmanDecoder metacode;
        metacode.Initialize();

        for (int i = 0; i < 37; i++)
            metacode.AddValueLF(i, kMetaCodes[i], kMetaCodeLengths[i]);
        metacode.MakeTable(true);

        ParseHuffmanCode(firstcode, 321, metacode);
        if (val & 0x08) secondcode = firstcode;
        else ParseHuffmanCode(secondcode, 321, metacode);
        ParseHuffmanCode(offsetcode, (val & 0x07) + 10, metacode);
    }
    else if (code < 6) {
        firstcode.Initialize(kFirstCodeLengths[code-1], 321, 32, true);
        secondcode.Initialize(kSecondCodeLengths[code-1], 321, 32, true);
        offsetcode.Initialize(kOffsetCodeLengths[code-1], kOffsetCodeSize[code-1], 32, true);
    }
    else {
        throw ExtractException("Algo13: invalid compressed data [code]");
    }

    currcode = &firstcode;
    firstcode.MakeTable(true);
    secondcode.MakeTable(true);
    offsetcode.MakeTable(true);
}



// Parse code of size.
void Algorithm13Method::ParseHuffmanCode(HuffmanDecoder &decoder, int numcodes,
    HuffmanDecoder &metacode)
{
    int length = 0;
    int lengths[numcodes];

    for (int i = 0; i < numcodes; i++) {
        int val = metacode.NextSymbol(input);

        switch(val) {
            case 31: length = -1; break;
            case 32: length++; break;
            case 33: length--; break;
            case 34:
                if (input.ReadBit()) lengths[i++] = length;
                break;
            case 35:
                val = input.ReadWord(3) + 2;
                while (val--) lengths[i++] = length;
                break;
            case 36:
                val = input.ReadWord(6) + 10;
                while (val--) lengths[i++] = length;
                break;
            default: length = val+1; break;
        }

        lengths[i] = length;
    }

    decoder.Initialize(lengths, numcodes, 32, true);
}



// Get the next litteral or offset.
int Algorithm13Method::NextLiteralOrOffset(int &offset, int &length)
{
    if (input.HasEnded())
        throw ExtractException("Algo13: all data read but algo has not finished");
    int val = currcode->NextSymbol(input);

    if (val < 0x100) {
        currcode = &firstcode;
        return val;
    }
    else {
        currcode = &secondcode;

        if (val < 0x13E) length = val - 0x100 + 3;
        else if (val == 0x13E) length = input.ReadWord(10) + 65;
        else if (val == 0x13F) length = input.ReadWord(15) + 65;
        else return kLzssEnd;

        int bit_length = offsetcode.NextSymbol(input);
        if (bit_length == 0) offset = 1;
        else if (bit_length == 1) offset = 2;
        else offset = (1 << (bit_length-1)) + input.ReadWord(bit_length-1) + 1;

        return kLzssMatch;
    }
}




// Constants.
constexpr int kWindowSize = 65536;
constexpr int kWindowMask = kWindowSize - 1;


// Initialize the algorithm.
void Algorithm13Method::Initialize()
{
    ended = false;
    input.Load(data, end - data);
    window_buffer = std::make_unique<uint8_t[]>(kWindowSize);

    match_length = 0; match_offset = 0;
    memset(window_buffer.get(), 0, kWindowSize);

    InitializeLZSS();
}



// Read the next byte.
int32_t Algorithm13Method::ReadNextByte(uint32_t pos)
{
    if (!match_length) {
        int offset, length;
        int val = NextLiteralOrOffset(offset, length);

        if (val >= 0) {
            window_buffer[pos & kWindowMask] = val;
            return val;
        } else if (val == kLzssEnd) {
            ended = true;
            return -1;
        } else {
            match_length = length;
            match_offset = pos - offset;
        }
    }

    match_length--;
    uint8_t byte = window_buffer[match_offset++ & kWindowMask];
    window_buffer[pos & kWindowMask] = byte;
    return byte;
}



// Read the next bytes.
int32_t Algorithm13Method::ReadBytes(uint8_t *buffer, uint32_t length)
{
    if (ended) return -1;

    uint8_t *start = buffer;
    uint8_t *end_capacity = buffer + length;

    int32_t byte;
    while (buffer != end_capacity && (byte = ReadNextByte(total_size + (buffer - start))) != -1)
        *(buffer++) = byte;

    return buffer - start;
}



} // namespace stuffit
} // namespace maconv
