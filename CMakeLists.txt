#
# Build Maconv.
#
# Copyright (C) 2019, Guillaume Gonnet
# License GPL3

cmake_minimum_required(VERSION 3.2)
project(maconv)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Compile vendor libraries.
add_subdirectory("vendors/libhfs")


# Source files.
set(MACONV_SRC
    "src/fs/file.h"
    "src/fs/file.cc"
    "src/fs/file_reader.h"
    "src/fs/file_reader.cc"
    "src/fs/file_writer.h"
    "src/fs/file_writer.cc"

    "src/conv/converters.h"
    # "src/conv/appledouble.cc"
    "src/conv/applesingle.cc"
    "src/conv/macbinary.cc"
    "src/conv/binhex.cc"
    "src/conv/rsrc.cc"

    "src/disk/disk.h"
    "src/disk/extract.cc"
    "src/disk/pack.cc"

    "src/stuffit/stuffit.h"
    "src/stuffit/stuffit.cc"
    "src/stuffit/stuffit_v1.cc"
    "src/stuffit/stuffit_v5.cc"
    "src/stuffit/utils/bwt.h"
    "src/stuffit/utils/bwt.cc"
    "src/stuffit/utils/crc.h"
    "src/stuffit/utils/crc.cc"
    "src/stuffit/utils/huffman.h"
    "src/stuffit/utils/huffman.cc"
    "src/stuffit/methods.h"
    "src/stuffit/methods.cc"
    "src/stuffit/methods/rle90.h"
    "src/stuffit/methods/rle90.cc"
    "src/stuffit/methods/compress.h"
    "src/stuffit/methods/compress.cc"
    "src/stuffit/methods/algorithm13.h"
    "src/stuffit/methods/algorithm13.cc"
    "src/stuffit/methods/arsenic.h"
    "src/stuffit/methods/arsenic.cc"

    "src/formats/file_signature.h"
    "src/formats/file_signature.cc"
    "src/formats/formats.h"
    "src/formats/formats.cc"
    "src/formats/unpack.cc"
    "src/formats/pack.cc"

    "src/utils/buffer_stream.h"
    "src/utils/buffer_stream.cc"
    "src/utils/bit_reader.h"
    "src/utils/bit_reader.cc"

    "src/commands.h"
    "src/commands.cc"
    "src/main.cc"
)

# Include "src" and "vendors" folders for resolving #include.
include_directories("src" "vendors")


# Create the executable.
add_executable(maconv ${MACONV_SRC})

# Link "libhfs" library.
target_link_libraries(maconv hfs)


# Install rules for Maconv.
install(TARGETS maconv RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(FILES "src/maconv.1" DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1")
