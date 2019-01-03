#
# Build libHFS.
#
# Copyright (C) 2017, Guillaume Gonnet
# License GPL3

project(libhfs)

# Note: this CMake file is an adaptation of the default autoconf file from
# libhfs.


# Check that a function exists.
include(CheckFunctionExists)
check_function_exists(mktime HAVE_MKTIME)

# Add "HAVE_CONFIG_H" definition.
add_definitions(-DHAVE_CONFIG_H)

# Configure "config.h" file.
configure_file(config.h.in config.h)


# Source files.
set(LIBHFS_SRC
    "block.h" "block.c"
    "btree.h" "btree.c"
    "data.h" "data.c"
    "file.h" "file.c"
    "low.h" "low.c"
    "medium.h" "medium.c"
    "node.h" "node.c"
    "os.h" "os-unix.c"
    "record.h" "record.c"
    "version.h" "version.c"
    "volume.h" "volume.c"
    "hfs.h" "hfs.c"
    "apple.h"
    "config.h"
)

# Include all headers from this folder.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create the library.
add_library(hfs STATIC ${LIBHFS_SRC})
